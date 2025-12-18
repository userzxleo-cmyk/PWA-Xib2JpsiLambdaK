#include <cuda_runtime.h>
#include <math.h>
#include <stdio.h>

#include "../RooTDalitz/Resonances.h"
#include "../RooTDalitz/cuComplex.h"
#include "../RooTDalitz/cuMatrix.h"
#include "../RooTDalitz/cuSolve4x4.h"
#include "../RooTDalitz/cu_DPFPropogator.h"

const float m_lb = 5.61951f;
const float m_k = 0.493677f;
const float m_p = 0.938272046f;
const float m_jpsi = 3.096916f;
const float m_chic1 = 3.51067f;
// const float _FFB = 3.f;
// const float _FFR = 3.f;
const float NN = 10000.f;

// const float _mres0 = -0.114064 ; //wmz debug
// const float _mres1 =  0.0699014;
// const float _mres2 = -0.00855841;
// const float _mres0 = -0.134064 ;
// const float _mres1 =  0.0599014;
// const float _mres2 = -0.00655841;

// const int nset  = 6;

__device__ float gauss_res(float delta, float sigma) {
    return exp(-0.5f * delta * delta / sigma / sigma) / sqrt(3.14159265f * 2.f) / sigma;
}

__device__ float getp(float M, float m1, float m2) {
    float M2 = M * M;
    return sqrt(fabs((M2 - powf(m1 + m2, 2)) * (M2 - powf(m1 - m2, 2)))) / 2.f / M;
}

__device__ float2 getrho(float M2, float m1, float m2, int L, float _FFB) {
    float rho2 = (M2 - powf(m1 + m2, 2)) * (M2 - powf(m1 - m2, 2));
    float rho = sqrtf(fabs(rho2)) / M2 * getFBW2(M2, m1, m2, L, _FFB);
    //    if(M2-20.489294 > -0.0001 && M2-20.489294 < 0.0001 &&
    //    m1-2.008555<0.001 && m1-2.008555>-0.001 && m1-2.517955<0.01 &&
    //    m1-2.517955>-0.001) {printf("wmzwmz try debug getrho s=%f, part1=%f,
    //    part2=%f, rho2=%f, rho=%f \n", M2, (M2-powf(m1+m2,2)),
    //    (M2-powf(m1-m2,2)), rho2, rho);}
    if (rho2 >= 0) return make_cuFloatComplex(rho, 0);
    return make_cuFloatComplex(0, rho);
}

__device__ float2 getrho_complexMass(float M2, float2 m1, float2 m2, int L, float _FFB) {
    float2 part1 = cuCaddf(cuCmulf(cuCaddf(m1, m2), cuCaddf(m1, m2)), make_cuFloatComplex(-1.f * M2, 0.f));
    float2 part2 = cuCaddf(cuCmulf(cuCaddf(m1, cuCmulfc(-1.f, m2)), cuCaddf(m1, cuCmulfc(-1.f, m2))),
                           make_cuFloatComplex(-1.f * M2, 0.f));
    float2 rho2 = cuCmulf(part1, part2);
    //        printf("wmz debug rho2 = %f+i%f, cuCsqrtf(rho2) = %f+i%f", rho2.x,
    //        rho2.y, cuCsqrtf(rho2).x, cuCsqrtf(rho2).y);
    float2 rho =
        cuCmulf(cuCmulf(cuCsqrtf(rho2), make_cuFloatComplex(1.f / M2, 0.f)), getFBW2_complexMass(M2, m1, m2, L, _FFB));
    //        if( M2-20.489294 > -0.000001 && M2-20.489294 < 0.000001 &&
    //        m1.x-2.008555<0.00001 && m1.x-2.008555>-0.00001 &&
    //        m2.x-2.517955<0.0001 && m2.x-2.517955>-0.00001) {printf("wmzwmz
    //        try debug getrho s=%f, part1.x=%f, part1.y=%f, part2.x=%f,
    //        part2.y=%f, rho2.x=%f, rho2.y=%f, rho.x=%f, rho.y=%f \n", M2,
    //        part1.x, part1.y, part2.x, part2.y, rho2.x, rho2.y, rho.x,
    //        rho.y);}
    return rho;
}

__device__ float F_BW(float r, int L, float p, float p0) {
    float z = powf(r * p, 2);
    float z0 = powf(r * p0, 2);
    float c = 1.0f;
    switch (L) {
        case 1:
            c = sqrt((1.f + z0) / (1.f + z));
            break;

        case 2:
            c = sqrt((z0 * z0 + 3.f * z0 + 9.f) / (z * z + 3.f * z + 9.f));
            break;

        case 3:
            c = sqrt((z0 * z0 * z0 + 6 * z0 * z0 + 45 * z0 + 225) / (z * z * z + 6 * z * z + 45 * z + 225));
            break;

        case 4:
            c = sqrt((powf(z0, 4) + 10 * powf(z0, 3) + 135 * z0 * z0 + 1575 * z0 + 11025) /
                     (powf(z, 4) + 10 * powf(z, 3) + 135 * z * z + 1575 * z + 11025));
            break;

        case 5:
            c = sqrt((powf(z0, 5) + 15 * powf(z0, 4) + 315 * powf(z0, 3) + 6300 * powf(z0, 2) + 99225 * z0 + 893025) /
                     (powf(z, 5) + 15 * powf(z, 4) + 315 * powf(z, 3) + 6300 * powf(z, 2) + 99225 * z + 893025));
            break;
    }
    return c;
}

__device__ float F_BW2(float r, int L, float p2, float p20) {
    float z = r * r * p2;
    float z0 = r * r * fabs(p20);
    float c = 1.0f;
    switch (L) {
        case 1:
            c = ((1.f + z0) / (1.f + z));
            break;

        case 2:
            c = ((z0 * z0 + 3 * z0 + 9.f) / (z * z + 3 * z + 9.f));
            break;

        case 3:
            c = ((z0 * z0 * z0 + 6 * z0 * z0 + 45 * z0 + 225) / (z * z * z + 6 * z * z + 45 * z + 225));
            break;

        case 4:
            c = ((pow(z0, 4) + 10 * pow(z0, 3) + 135 * z0 * z0 + 1575 * z0 + 11025) /
                 (pow(z, 4) + 10 * pow(z, 3) + 135 * z * z + 1575 * z + 11025));
            break;

        case 5:
            c = ((pow(z0, 5) + 15 * pow(z0, 4) + 315 * pow(z0, 3) + 6300 * pow(z0, 2) + 99225 * z0 + 893025) /
                 (pow(z, 5) + 15 * pow(z, 4) + 315 * pow(z, 3) + 6300 * pow(z, 2) + 99225 * z + 893025));
            break;
    }
    return c;
}

__device__ float getFBW2(float M2, float m1, float m2, int L, float _FFB) {
    if (L == 0) return 1.0f;
    float p2 = (M2 - powf(m1 + m2, 2)) * (M2 - powf(m1 - m2, 2)) / M2 / 4.0f;
    float M20 = 25.0f;
    float p20 = (M20 - powf(m1 + m2, 2)) * (M20 - powf(m1 - m2, 2)) / M20 / 4.0f;
    return pow(p2 / p20, L) * F_BW2(_FFB, L, p2, p20);
}

__device__ float2 getFBW2_complexMass(float M2, float2 m1, float2 m2, int L, float _FFB) {
    // if(L==0) return make_cuFloatComplex(1.f, 0.f);
    return make_cuFloatComplex(1.f, 0.f);
    //        float2 part1 = cuCaddf(cuCmulf(cuCaddf(m1,m2), cuCaddf(m1,m2)),
    //        make_cuFloatComplex(-1.f*M2, 0)); float2 part2 =
    //        cuCaddf(cuCmulf(cuCaddf(m1,cuCmulfc(-1.f, m2)),
    //        cuCaddf(m1,cuCmulfc(-1.f, m2))), make_cuFloatComplex(-1.f*M2, 0));
    //        float2 rho2 = cuCmulf(part1, part2);
    //        float2 p2=cuCmulfc(1.f/M2/4.0f, rho2);
    //        float M20=25.0f;
    //        float2 part10 = cuCaddf(cuCmulf(cuCaddf(m1,m2), cuCaddf(m1,m2)),
    //        make_cuFloatComplex(-1.f*M20, 0)); float2 part20 =
    //        cuCaddf(cuCmulf(cuCaddf(m1,cuCmulfc(-1.f, m2)),
    //        cuCaddf(m1,cuCmulfc(-1.f, m2))), make_cuFloatComplex(-1.f*M20,
    //        0)); float2 rho20 = cuCmulf(part1, part2); float2
    //        p20=cuCmulfc(1.f/M20/4.0f, rho2);
}

__device__ float BTerm(float J, float mpp, float mr, float _FFB) {
    int LB = (J <= 1) ? 0 : (int)(J - 1.5f);

    //**** to change for NR to include Blatt-W
    float pB = getp(m_lb, m_jpsi, mpp);
    // for NR
    //   if(mr<-0.) return powf(pB/m_lb, LB);
    float pB0 = pB;
    if (mr < -0.f) {
        float mmin = m_k + m_p;
        float mmax = m_lb - m_jpsi;
        float mreff = (mmin + mmax) / 2.f;
        pB0 = getp(m_lb, m_jpsi, mreff);
    }
    if (mr > 1e-10f && mr + m_jpsi < m_lb) pB0 = getp(m_lb, m_jpsi, mr);
    if (mr + m_jpsi > m_lb) {
        float mmin = m_k + m_p;
        float mmax = m_lb - m_jpsi;
        float mreff = mmin + 0.5f * (mmax - mmin) * (1.0f + tanh((mr - (mmin + mmax) / 2.f) / (mmax - mmin)));
        pB0 = getp(m_lb, m_jpsi, mreff);
    }

    auto amplitude = (LINESHAPE_LBPLUSONE)
                         ? F_BW(_FFB, LB + 1, pB, pB0) * powf(pB / m_lb, LB) * powf(m_lb / pB0, LB) * (pB / pB0)
                         : F_BW(_FFB, LB, pB, pB0) * powf(pB / m_lb, LB) * powf(m_lb / pB0, LB);
    return amplitude;
}

__device__ float BTermZ(float J, float mpp, float mr, float _FFB) {
    int LB = (int)(J - 0.5f);
    //  if(LB<0) LB = 0;

    float pB = getp(m_lb, m_k, mpp);
    float pB0 = pB;
    // NR
    if (mr < -0.f) {
        float mmin = m_jpsi + m_p;
        float mmax = m_lb - m_k;
        float mreff = (mmin + mmax) / 2.f;
        pB0 = getp(m_lb, m_k, mreff);
        //    return powf(pB/m_lb, LB);
    }
    if (mr > 1e-10f && mr + m_k < m_lb) pB0 = getp(m_lb, m_k, mr);

    // Use L+1 as a systematic
    auto amplitude = (LINESHAPE_LBPLUSONE) ? F_BW(_FFB, LB + 1, pB, pB0) * powf(pB / m_lb, LB) * (pB / pB0)
                                           : F_BW(_FFB, LB, pB, pB0) * powf(pB / m_lb, LB);
    return amplitude;
}

__device__ float BTermX(float J, float mpp, float mr, float _FFB) {
    int LB = (int)(J - 0.5);
    //  if(LB<0) LB = 0;

    float pB = getp(m_lb, m_p, mpp);
    float pB0 = pB;
    // NR
    if (mr < -0) return pow(pB / m_lb, LB);
    if (mr > 1e-10 && mr + m_p < m_lb) pB0 = getp(m_lb, m_p, mr);

    //  float phi00 = ((RooAbsReal&)ires[ihel*2+1]).getVal()*deg2rad;
    return F_BW(_FFB, LB, pB, pB0) * pow(pB / m_lb, LB);
}

__device__ float2 breit_wigner(float m, float gamma, float e2) {
    float de2 = m * m - e2;
    float gamma_m = gamma * m;
    float d = (de2 * de2 + gamma_m * gamma_m);
    return make_cuFloatComplex(de2 / d, gamma_m / d);
}

__device__ float2 BW_AMPX(float m_r, float gamma_r, float mpp, int LR, float _FFR) {
    float m2pp = mpp * mpp;

    float pR = getp(mpp, m_k, m_jpsi);
    float pR0 = pR;
    if (m_r > m_k + m_jpsi) pR0 = getp(m_r, m_k, m_jpsi);  // sqrt(m_r*m_r-4*m2_pi)/2.;

    float FR = F_BW(_FFR, LR, pR, pR0);

    float gamma = gamma_r;
    // if(with_dep!=0) gamma = gamma_r*pow((pR/pR0),2*LR+1)*(m_r/mpp)*FR*FR;
    if (m_r > m_k + m_jpsi) gamma = gamma_r * pow((pR / pR0), 2 * LR + 1) * (m_r / mpp) * FR * FR;
    // charged mpp->mr
    //   float factor = FR*pow(pR/mpp,LR);

    // float factor = FR*pow((pR/m_r),LR);
    float kr = sqrt(m_r * m_r + gamma_r * gamma_r);
    float factor = FR * pow((pR / pR0), LR) * 0.948850 * m_r * sqrt(gamma_r * kr / sqrt(m_r * (m_r + kr)));
    float2 bw = breit_wigner(m_r, gamma, m2pp);
    return cuCmulfc(factor, bw);
}

__device__ float2 BW_AMP(float m_r, float gamma_r, float mpp, int LR, float _FFR) {
    float m2pp = mpp * mpp;
    float pR = getp(mpp, m_p, m_k);
    // For NR
    float pR0 = pR;
    if (m_r < -0) {
        //    return make_cuFloatComplex(powf(pR,LR), 0);
        float mmin = m_k + m_p;
        float mmax = m_lb - m_jpsi;
        float mreff = (mmin + mmax) / 2.f;
        pR0 = getp(mreff, m_p, m_k);
    }
    if (m_r > m_p + m_k) {
        pR0 = getp(m_r, m_p, m_k);  // sqrt(m_r*m_r-4*m2_pi)/2.;
    }
    if (m_r <= m_p + m_k && m_r > 1e-10f) {
        float mmin = m_k + m_p;
        float mmax = m_lb - m_jpsi;
        float mreff = mmin + 0.5f * (mmax - mmin) * (1.0f + tanh((m_r - (mmin + mmax) / 2.f) / (mmax - mmin)));
        pR0 = getp(mreff, m_p, m_k);
    }
    float FR = F_BW(_FFR, LR, pR, pR0);
    if (m_r < -0) {
        return make_cuFloatComplex(powf(pR / pR0, LR), 0);
    }
    float gamma = gamma_r;
    // if(with_dep!=0) gamma = gamma_r*powf((pR/pR0),2*LR+1)*(m_r/mpp)*FR*FR;
    if (m_r > m_p + m_k) {
        gamma = gamma_r * powf((pR / pR0), 2 * LR + 1) * (m_r / mpp) * FR * FR;
    } else if (m_r < 1.43f && m_r > 0) {
        /*
        //where LR = 0
        float msig = 1.192642;
        float mpi0 = 0.1349766;
        float pRSigPi = getp(mpp, msig, mpi0);
        float rhosigpi = 2.0*pRSigPi/mpp;
        float rhokp = 2.0*pR/mpp;
        gamma = gamma_r * (rhosigpi+rhokp)*9.0645/2.;
        //    std::cout << "gamma_r " << gamma_r << " " << gamma << std::endl;
         */
        float msig = 1.18937f;     // 1.192642;
        float mpi0 = 0.13957018f;  // 0.1349766;
        float pRSigPi = getp(mpp, msig, mpi0);
        //    float rhosigpi = 2.0*pRSigPi/mpp;
        //    float rhokp = 2.0*pR/mpp;
        // mine
        //    gamma = gamma_r *
        //    (rhosigpi+rhokp)*m_r/2./getp(m_r,msig,mpi0);//9.0645/2.;

        // wehre LR = 0;
        gamma = 0.0f;
        pR0 = getp(m_r, msig, mpi0);
        FR = 1.0f;
        gamma += gamma_r * powf((pR / pR0), 2 * LR + 1) * (m_r / mpp) * FR * FR;
        FR = 1.0f;
        gamma += 0.0505f * powf((pRSigPi / pR0), 2 * LR + 1) * (m_r / mpp) * FR * FR;
    }

    // charged 4/17 mpp->mr
    //   float factor = FR*powf(pR/mpp,LR);
    //   float factor = FR*powf((pR/m_r),LR);
    float kr = sqrt(m_r * m_r + gamma_r * gamma_r);
    float factor = FR * powf((pR / pR0), LR) * 0.948850f * m_r * sqrt(gamma_r * kr / sqrt(m_r * (m_r + kr)));
    float2 bw = breit_wigner(m_r, gamma, m2pp);
    // printf("wmzwmz try debug m_r=%f, pR0=%f, LR=%d \n",m_r, pR0, LR);
    return cuCmulfc(factor, bw);
}

__device__ float2 BW_AMPZ(float m_r, float gamma_r, float mpp, int LR, float _FFR) {
    float m2pp = mpp * mpp;

    float pR = getp(mpp, m_p, m_jpsi);
    float pR0 = pR;
    if (m_r > m_p + m_jpsi) pR0 = getp(m_r, m_p, m_jpsi);  // sqrt(m_r*m_r-4*m2_pi)/2.;

    float FR = F_BW(_FFR, LR, pR, pR0);

    float gamma = gamma_r;
    // if(with_dep!=0) gamma = gamma_r*powf((pR/pR0),2*LR+1)*(m_r/mpp)*FR*FR;
    if (m_r > m_p + m_jpsi) gamma = gamma_r * powf((pR / pR0), 2 * LR + 1) * (m_r / mpp) * FR * FR;
    // charged mpp->mr
    //   float factor = FR*powf(pR/mpp,LR);

    // float factor = FR*powf((pR/m_r),LR);
    float kr = sqrt(m_r * m_r + gamma_r * gamma_r);
    float factor = FR * powf((pR / pR0), LR) * 0.948850f * m_r * sqrt(gamma_r * kr / sqrt(m_r * (m_r + kr)));
    float2 bw = breit_wigner(m_r, gamma, m2pp);
    return cuCmulfc(factor, bw);
}

__device__ float2 BW_AMPZSMR(float m_r, float gamma_r, float mpp, int LR, float _FFR)
// mres is function of mpp
//			      float mres)
{
    float mres = _mres0 + mpp * (_mres1 + mpp * _mres2);
#ifdef RESPLUS
    mres = mres * 1.1;
#endif
#ifdef RESMINUS
    mres = mres * 0.9;
#endif
    float min = m_jpsi + m_p;
    float max = m_lb - m_k;
    float mbin = (max - min) / NN;

    float dxmax, smpp, dmpp, xmpp, w;

    smpp = mres;
    //+/- 5 * resolution region;
    dxmax = (int)(5.f * smpp / mbin);
    smpp = smpp * sqrt(2.f);
    float sum(0), sumcos(0), sumsin(0);
    float2 bw;
    for (int dx = -dxmax; dx <= dxmax; ++dx) {
        dmpp = mbin * (float)dx;
        xmpp = dmpp + mpp;
        if (xmpp >= min && xmpp <= max) {  // in the range;
            dmpp = -powf((dmpp / smpp), 2);
            if (dmpp > -9.f) {
                // resolutoin gaus.
                w = exp(dmpp);
                bw = BW_AMPZ(m_r, gamma_r, xmpp, LR, _FFR);
                //	aa = cuCabs2(bw);
                // ph = atan2(bw_i,bw_r);
                sum += w * cuCabsf2(bw);
                sumsin += w * cuCimagf(bw);  // sqrt(aa)*sin(ph);
                sumcos += w * cuCrealf(bw);  // sqrt(aa)*cos(ph);
            }
        }
    }

    float Y = sqrt(sum * (mbin / mres) / sqrt(2.f * 3.14159265f));
    float Z = sqrt(sumsin * sumsin + sumcos * sumcos);
    return make_cuFloatComplex(sumcos / Z * Y, sumsin / Z * Y);
}

__device__ void Isospin_KMatrix(const float mpp, const float *paraList, const int J_r, const int P_r, const int irst,
                                const int ired, const int kmcount, const int ik, float2 *Mtr, const float _FFB) {
    constexpr float Dzero_mass = 1.86484f;
    constexpr float Dstzero_mass = 2.00685f;
    constexpr float SigmacPlus_mass = 2.45265f;
    constexpr float SigmacstPlus_mass = 2.5174f;
    constexpr float SigmacPlus_halfwidth = -2.3f / 1000.f / 2.f;
    constexpr float SigmacstPlus_halfwidth = -17.2f / 1000.f / 2.f;

    constexpr float2 Dzero_complexMass = {Dzero_mass, 0.f};
    constexpr float2 Dstzero_complexMass = {Dstzero_mass, 0.f};
    constexpr float2 SigmacPlus_complexMass = {SigmacPlus_mass, SigmacPlus_halfwidth};
    constexpr float2 SigmacstPlus_complexMass = {SigmacstPlus_mass, SigmacstPlus_halfwidth};

    constexpr float Dminus_mass = 1.86966f;
    constexpr float Dstminus_mass = 2.01026f;
    constexpr float SigmacPlusPlus_mass = 2.45397f;
    constexpr float SigmacstPlusPlus_mass = 2.51841f;
    constexpr float SigmacPlusPlus_halfwidth = -1.89f / 1000.f / 2.f;
    constexpr float SigmacstPlusPlus_halfwidth = -14.78f / 1000.f / 2.f;

    constexpr float2 Dminus_complexMass = {Dminus_mass, 0.f};
    constexpr float2 Dstminus_complexMass = {Dstminus_mass, 0.f};
    constexpr float2 SigmacPlusPlus_complexMass = {SigmacPlusPlus_mass, SigmacPlusPlus_halfwidth};
    constexpr float2 SigmacstPlusPlus_complexMass = {SigmacstPlusPlus_mass, SigmacstPlusPlus_halfwidth};

    constexpr size_t nChannels = 7;
    constexpr float isospin_factors[2] = {0.816496580927726f, -0.5773502691896257f};

    int Lna = (P_r == 1) ? 1 : 0;
    float s = mpp * mpp;

    float2 rho[nChannels];
    rho[0] = getrho(s, 0.938272046f, 3.096916f, Lna, _FFB);
    if (1 == J_r && P_r == -1) {
        rho[1] = getrho_complexMass(s, Dstminus_complexMass, SigmacPlusPlus_complexMass, Lna, _FFB);
        rho[2] = getrho_complexMass(s, Dstzero_complexMass, SigmacPlus_complexMass, Lna, _FFB);
        rho[3] = getrho_complexMass(s, Dstminus_complexMass, SigmacstPlusPlus_complexMass, Lna, _FFB);
        rho[4] = getrho_complexMass(s, Dstzero_complexMass, SigmacstPlus_complexMass, Lna, _FFB);
        rho[5] = getrho_complexMass(s, Dminus_complexMass, SigmacPlusPlus_complexMass, Lna, _FFB);
        rho[6] = getrho_complexMass(s, Dzero_complexMass, SigmacPlus_complexMass, Lna, _FFB);
    } else if (3 == J_r && P_r == -1) {
        rho[1] = getrho_complexMass(s, Dstminus_complexMass, SigmacPlusPlus_complexMass, Lna, _FFB);
        rho[2] = getrho_complexMass(s, Dstzero_complexMass, SigmacPlus_complexMass, Lna, _FFB);
        rho[3] = getrho_complexMass(s, Dstminus_complexMass, SigmacstPlusPlus_complexMass, Lna, _FFB);
        rho[4] = getrho_complexMass(s, Dstzero_complexMass, SigmacstPlus_complexMass, Lna, _FFB);
        rho[5] = getrho_complexMass(s, Dminus_complexMass, SigmacstPlusPlus_complexMass, Lna, _FFB);
        rho[6] = getrho_complexMass(s, Dzero_complexMass, SigmacstPlus_complexMass, Lna, _FFB);
    } else if (P_r == 1) {
        rho[1] = getrho(s, Dstminus_mass, SigmacPlusPlus_mass, Lna, _FFB);
        rho[2] = getrho(s, Dstzero_mass, SigmacPlus_mass, Lna, _FFB);
        rho[3] = getrho(s, Dminus_mass, SigmacPlusPlus_mass, Lna, _FFB);
        rho[4] = getrho(s, Dzero_mass, SigmacPlus_mass, Lna, _FFB);
        rho[5] = getrho(s, Dminus_mass, SigmacstPlusPlus_mass, Lna, _FFB);
        rho[6] = getrho(s, Dzero_mass, SigmacstPlus_mass, Lna, _FFB);
    }

    const float *g_alpha = &paraList[kmcount + ik * (Npmax + NKMC) * NKMC];
    const float *f_scat = &paraList[kmcount + Npmax * NKMC + ik * (Npmax + NKMC) * NKMC];
    // consider maximum four pole
    int Npole(0);
    float m0;
    float ms[Npmax + 1] = {1, 1, 1, 1, 1};
    float denorm(1);
    int ikm(1);

    for (int ir = irst; ir < ired; ++ir) {
        const float mfit = paraList[ir * nset];
        const int iChannel = (int)paraList[ir * nset + 1];  // Change this to iChannel since g0 is unused.
        const int S = (int)(paraList[ir * nset + 2]);
        const int type = (int)(paraList[ir * nset + 4]);
        const int parity = (int)(paraList[ir * nset + 5]);

        if (type == KMATRIX && S == J_r && parity == P_r) {
            if (mfit > 0) {
                if (iChannel == 1 || iChannel == 3) {
                    float2 _rhoChannel[2];
                    float mfit2 = mfit * mfit;
                    switch (iChannel) {
                        case 1:
                            _rhoChannel[0] =
                                getrho_complexMass(mfit2, Dstminus_complexMass, SigmacPlusPlus_complexMass, Lna, _FFB);
                            _rhoChannel[1] =
                                getrho_complexMass(mfit2, Dstzero_complexMass, SigmacPlus_complexMass, Lna, _FFB);
                            break;
                        case 3:
                            _rhoChannel[0] =
                                getrho_complexMass(mfit2, Dminus_complexMass, SigmacPlusPlus_complexMass, Lna, _FFB);
                            _rhoChannel[1] =
                                getrho_complexMass(mfit2, Dzero_complexMass, SigmacPlus_complexMass, Lna, _FFB);
                            break;
                        default:
                            printf("Something very very wrong has happened!");
                    }
                    const float gChannel = g_alpha[Npole * NKMC + iChannel];
                    // Isospin factors are 2/3 g^2 rho and 1/3 g^2 rho,
                    // numerators (2 and 1) into rho factors, 1/3 into g^2
                    const float2 near_threshold =
                        make_cuFloatComplex(0.f, gChannel * gChannel / 3.f) * (2.f * _rhoChannel[0] + _rhoChannel[1]);
                    // Only take the real part, i.e. near threshold only
                    // contributes below the channel threshold
                    m0 = sqrt(mfit2 + cuCrealf(near_threshold));
                } else if (iChannel == 0) {
                    m0 = mfit;  // No below threshold effects for jpsi p channel
                } else {
                    printf("An unacceptable channel is passed! %i\n", iChannel);
                }

                denorm = (m0 * m0 - s);
                if (fabs(denorm) < 1e-3f) denorm = 1e-3f;
                Npole++;
                ms[0] *= denorm;
                for (int ip = 1; ip <= Npmax; ++ip) {
                    if (ip == ikm) continue;
                    ms[ip] *= denorm;
                }
                ikm++;
            }
        }
    }

    // Pole terms
    float mg[Npmax][NKMC];
    ikm = 0;
    for (int i = 0; i < Npole; ++i) {
        for (int j = 0; j < NKMC; ++j) {
            mg[i][j] = (g_alpha[ikm++] * ms[i + 1]);
        }
    }

    // Scattering terms
    float fs[NKMC][NKMC];
    ikm = 0;
    for (int i = 0; i < NKMC; ++i) {
        for (int j = 0; j < NKMC; ++j) {
            fs[i][j] = f_scat[ikm++] * ms[0];  // here scatter term is fab constant
        }
    }

    // Fill the K-matrix
    float kma[nChannels][nChannels];
// i, j == 0 is jpsip channel
// i = 1, 3, 5 is D- sigmac++, use isospin_factor[0]
// i = 2, 4, 6 is D0 sigmac+, use isospin_factor[1]
#pragma unroll
    for (int i = 0; i < nChannels; ++i) {
        const float iIsospinFactor = (i == 0) ? 1.f : isospin_factors[(i + 1) % 2];
        const int iPrime = (i + 1) / 2;  // The original index

#pragma unroll
        for (int j = i; j < nChannels; ++j) {
            const float jIsospinFactor = (j == 0) ? 1.f : isospin_factors[(j + 1) % 2];
            const float isoFactor = iIsospinFactor * jIsospinFactor;
            const int jPrime = (j + 1) / 2;  // The original index

            kma[i][j] = isoFactor * fs[iPrime][jPrime];
            for (int k = 0; k < Npole; ++k) {
                kma[i][j] += isoFactor * g_alpha[k * NKMC + iPrime] * mg[k][jPrime];
            }

            kma[j][i] = kma[i][j];  // Transpose the matrix
        }
    }

    float2 iKrho[nChannels][nChannels];
    float2 ni = {0, -1.f};
#pragma unroll
    for (int i = 0; i < nChannels; i++) {
#pragma unroll
        for (int j = 0; j < nChannels; j++) {
            iKrho[i][j] = ni * kma[i][j] * rho[j];
        }
    }
#pragma unroll
    for (int i = 0; i < nChannels; i++) {
        iKrho[i][i] = ms[0] + iKrho[i][i];
    }

    float2 T[nChannels];
    // solve_NxN<double2, double2, 0, nChannels>(&(iKrho[0][0]),
    //                                           &(T[0]));
    solve_7x7<double2, double2, 0>(&(iKrho[0][0]), &(T[0]));

    // Since couplings and non-resonant parameters will be multiplied by isospin
    // factors,
    //   we multiply the factor to inverted ( 1 - iKrho ) instead,
    //   mathematically equivalent
    float2 T_isospin[NKMC] = {T[0], isospin_factors[0] * T[1] + isospin_factors[1] * T[2],
                              isospin_factors[0] * T[3] + isospin_factors[1] * T[4],
                              isospin_factors[0] * T[5] + isospin_factors[1] * T[6]};

    float na = (Lna >= 1) ? getFBW2(s, 0.938272046f, 3.096916f, Lna, _FFB) : 1.f;
    for (int i = 0; i < NKMC + Npole; ++i) Mtr[i] = {0, 0};
    for (int ip = 0; ip < Npole; ++ip) {
        for (int i = 0; i < NKMC; ++i) {
            Mtr[ip] = Mtr[ip] + T_isospin[i] * mg[ip][i];
        }
        Mtr[ip] = Mtr[ip] * na;
    }
    for (int i = 0; i < NKMC; ++i) {
        Mtr[Npole + i] = T_isospin[i] * ms[0] * na;
    }
}

__device__ void Default_KMatrix(const float mpp, const float *paraList, const int J_r, const int P_r, const int irst,
                                const int ired, const int kmcount, const int ik, float2 *Mtr, const float _FFB) {
    constexpr float D_mass = 1.86719f;               // average of 0 and -
    constexpr float Dst_mass = 2.008555f;            // average of 0 and -
    constexpr float Sigmac_mass = 2.453435f;         // average of + and ++
    constexpr float Sigmacst_mass = 2.517955f;       // average of + and ++
    constexpr float Sigmac_halfwidth = -0.00105f;    // average of + and ++ states half width
    constexpr float Sigmacst_halfwidth = -0.00800f;  // average of + and ++ states half width

    int Lna = (P_r == 1) ? 1 : 0;
    float s = mpp * mpp;
    float2 rho[NKMC];  // do calculation here======needed
    // const float KM1[5]={0.938272046, 2.008555, 2.008555, 1.86719,  1.86719};
    // const float KM2[5]={3.096916,    2.453435, 2.517955, 2.453435, 2.517955};

    rho[0] = getrho(s, 0.938272046f, 3.096916f, Lna, _FFB);
    // #ifdef ComplexMassSwave
    //     rho[1] = getrho_complexMass(s,make_cuFloatComplex(Dst_mass,
    //     Dst_width),make_cuFloatComplex(Sigmac_mass, 0.f), Lna, _FFB);
    // #else
    // #endif
    if (1 == J_r && P_r == -1) {
#ifdef ComplexMassSwave
        rho[1] = getrho_complexMass(s, make_cuFloatComplex(Dst_mass, 0.f),
                                    make_cuFloatComplex(Sigmac_mass, Sigmac_halfwidth), Lna, _FFB);
        rho[3] = getrho_complexMass(s, make_cuFloatComplex(D_mass, 0.f),
                                    make_cuFloatComplex(Sigmac_mass, Sigmac_halfwidth), Lna, _FFB);
        rho[2] = getrho_complexMass(s, make_cuFloatComplex(Dst_mass, 0.f),
                                    make_cuFloatComplex(Sigmacst_mass, Sigmacst_halfwidth), Lna, _FFB);
        // rho[2] = getrho_complexMass(s,make_cuFloatComplex(Dst_mass,
        // Dst_halfwidth),make_cuFloatComplex(Sigmacst_mass,
        // Sigmacst_halfwidth), Lna, _FFB);
//      float2 rho222 = getrho(s,Dst_mass,Sigmacst_mass, Lna, _FFB);
//      if (1){
//      if (!((rho[2].x-rho222.x)<0.0001 && (rho[2].x-rho222.x)>-0.0001 &&
//      (rho[2].y-rho222.y)>-0.0001 && (rho[2].y-rho222.y)<0.0001)){
//        printf("wmzwmz try debug s=%f, Lna=%d, _FFB=%f, rho[2].x=%f,
//        rho222.x=%f, rho[2].y=%f, rho222.y=%f, 2.008555f,Sigmacst_mass \n", s,
//        Lna, _FFB, rho[2].x, rho222.x, rho[2].y, rho222.y);
//      }
#else
        rho[3] = getrho(s, D_mass, Sigmac_mass, Lna, _FFB);
        rho[2] = getrho(s, Dst_mass, Sigmacst_mass, Lna, _FFB);
        rho[1] = getrho(s, Dst_mass, Sigmac_mass, Lna, _FFB);
#endif
    } else if (3 == J_r && P_r == -1) {
#ifdef ComplexMassSwave
        rho[1] = getrho_complexMass(s, make_cuFloatComplex(Dst_mass, 0.f),
                                    make_cuFloatComplex(Sigmac_mass, Sigmac_halfwidth), Lna, _FFB);
        rho[2] = getrho_complexMass(s, make_cuFloatComplex(Dst_mass, 0.f),
                                    make_cuFloatComplex(Sigmacst_mass, Sigmacst_halfwidth), Lna, _FFB);
        // rho[2] = getrho_complexMass(s,make_cuFloatComplex(Dst_mass,
        // Dst_halfwidth),make_cuFloatComplex(Sigmacst_mass,
        // Sigmacst_halfwidth), Lna, _FFB);
        //      float2 rho222 = getrho(s,Dst_mass,Sigmacst_mass, Lna, _FFB);
        //      if (1){
        //      if (!((rho[2].x-rho222.x)<0.0001 && (rho[2].x-rho222.x)>-0.0001
        //      && (rho[2].y-rho222.y)>-0.0001 && (rho[2].y-rho222.y)<0.0001)){
        //        printf("wmzwmz try debug s=%f, Lna=%d, _FFB=%f, rho[2].x=%f,
        //        rho222.x=%f, rho[2].y=%f, rho222.y=%f, 2.008555f,Sigmacst_mass
        //        \n", s, Lna, _FFB, rho[2].x, rho222.x, rho[2].y, rho222.y);
        //      }
        rho[3] = getrho_complexMass(s, make_cuFloatComplex(D_mass, 0.f),
                                    make_cuFloatComplex(Sigmacst_mass, Sigmacst_halfwidth), Lna, _FFB);
//      float2 rho333 = getrho(s,D_mass,Sigmacst_mass, Lna, _FFB);
//      if (1){
//      if (!((rho[3].x-rho333.x)<0.0001 && (rho[3].x-rho333.x)>-0.0001 &&
//      (rho[3].y-rho333.y)>-0.0001 && (rho[3].y-rho333.y)<0.0001)){
//       printf("wmzwmz try debug s=%f, Lna=%d, _FFB=%f, rho[3].x=%f,
//       rho333.x=%f, rho[3].y=%f, rho333.y=%f, D_mass,Sigmacst_mass \n", s,
//       Lna, _FFB, rho[3].x, rho333.x, rho[3].y, rho333.y);
//      }
#else
        rho[2] = getrho(s, Dst_mass, Sigmacst_mass, Lna, _FFB);
        rho[3] = getrho(s, D_mass, Sigmacst_mass, Lna, _FFB);
        rho[1] = getrho(s, Dst_mass, Sigmac_mass, Lna, _FFB);
#endif
    } else if (P_r == 1) {
        rho[1] = getrho(s, Dst_mass, Sigmac_mass, Lna, _FFB);
        rho[2] = getrho(s, D_mass, Sigmac_mass, Lna, _FFB);
        //      rho[3] = getrho_complexMass(s,make_cuFloatComplex(D_mass,
        //      0.f),make_cuFloatComplex(Sigmacst_mass, Sigmacst_halfwidth),
        //      Lna, _FFB);
        rho[3] = getrho(s, D_mass, Sigmacst_mass, Lna, _FFB);
    }

    const float *g_alpha = &paraList[kmcount + ik * (Npmax + NKMC) * NKMC];
    const float *f_scat = &paraList[kmcount + Npmax * NKMC + ik * (Npmax + NKMC) * NKMC];
    // consider maximum four pole
    int Npole(0);
    float ms[Npmax + 1] = {1, 1, 1, 1, 1};
    float denorm(1);
    float m0;
    int iChannel;
    // int LR;
    int S, type, parity;
    int ikm(1);
    for (int ir = irst; ir < ired; ++ir) {
        float mfit = paraList[ir * nset];
        iChannel = (int)paraList[ir * nset + 1];  // Change this to iChannel since g0 is unused.
        S = (int)(paraList[ir * nset + 2]);
        // LR = (int) (paraList[ir*nset+3]);
        type = (int)(paraList[ir * nset + 4]);
        parity = (int)(paraList[ir * nset + 5]);

        if (type == KMATRIX && S == J_r && parity == P_r) {
            if (mfit > 0) {
                if (iChannel > 0) {
                    float2 rhoChannel;
                    float mfit2 = mfit * mfit;
                    switch (iChannel) {
                        case 1:
                            rhoChannel =
                                getrho_complexMass(mfit2, make_cuFloatComplex(Dst_mass, 0.f),
                                                   make_cuFloatComplex(Sigmac_mass, Sigmac_halfwidth), Lna, _FFB);
                            break;
                        case 2:
                            rhoChannel =
                                getrho_complexMass(mfit2, make_cuFloatComplex(D_mass, 0.f),
                                                   make_cuFloatComplex(Sigmacst_mass, Sigmacst_halfwidth), Lna, _FFB);
                            break;
                        case 3:
                            rhoChannel =
                                getrho_complexMass(mfit2, make_cuFloatComplex(D_mass, 0.f),
                                                   make_cuFloatComplex(Sigmac_mass, Sigmac_halfwidth), Lna, _FFB);
                            break;
                        default:
                            printf("Something very very wrong has happened!");
                    }
                    // near threshold effect = i g^2 rho
                    // float gChannel = g_alpha[iChannel];
                    float gChannel = g_alpha[Npole * NKMC + iChannel];
                    float2 near_threshold = cuCmulf(make_cuFloatComplex(0.f, gChannel * gChannel), rhoChannel);
                    // Only take the real part, i.e. near threshold only
                    // contributes below the channel threshold
                    m0 = sqrt(mfit2 + cuCrealf(near_threshold));
                    // printf("iChannel: %i\t gChannel: %f\t mfit: %f\t m0:
                    // %f\n", iChannel, gChannel, mfit, m0); printf("threshold:
                    // %f\t + i%f\n", near_threshold.x, near_threshold.y);
                    // printf("rhoChannel: %f\t + i%f\n", rhoChannel.x,
                    // rhoChannel.y);
                } else if (iChannel == 0) {
                    m0 = mfit;  // No below threshold effects for jpsi p channel
                } else {
                    printf("An unacceptable channel is passed! %i\n", iChannel);
                }

                denorm = (m0 * m0 - s);
                if (fabs(denorm) < 1e-3f) denorm = 1e-3f;
                Npole++;
                ms[0] *= denorm;
                for (int ip = 1; ip <= Npmax; ++ip) {
                    if (ip == ikm) continue;
                    ms[ip] *= denorm;
                }
                ikm++;
            }
        }
    }
    //  printf("Number of pole g4 fs4 %i %5.2f %5.2f\n", Npole, g_alpha[4],
    //  f_scat[4]);
    float mg[Npmax][NKMC];  // Npole x NC
    ikm = 0;
    for (int i = 0; i < Npole; ++i) {
        for (int j = 0; j < NKMC; ++j) {
            mg[i][j] = (g_alpha[ikm++] * ms[i + 1]);
        }
    }
    //  std::cout << "g_alpha[0], f_scat[0] " << g_alpha[0] << " " << f_scat[0]
    //  << std::endl;
    float fs[NKMC][NKMC], kma[NKMC][NKMC];
    ikm = 0;
    for (int i = 0; i < NKMC; ++i) {
        for (int j = 0; j < NKMC; ++j) {
            fs[i][j] = f_scat[ikm++] * ms[0];  // here scatter term is fab constant
        }
    }

    for (int i = 0; i < NKMC; ++i) {
        for (int j = i; j < NKMC; ++j) {
            kma[i][j] = fs[i][j];
            for (int k = 0; k < Npole; ++k) {
                kma[i][j] += g_alpha[k * NKMC + i] * mg[k][j];
            }
            kma[j][i] = kma[i][j];
        }
    }

    float2 KK[NKMC][NKMC];
    float2 ni = make_cuFloatComplex(0, -1.f);
    for (int i = 0; i < NKMC; i++) {
        for (int j = 0; j < NKMC; j++) {
            KK[i][j] = cuCmulf(ni, cuCmulfc(kma[i][j], rho[j]));
            if (i == j) KK[i][j] = cuCaddfc(ms[0], KK[i][j]);
        }
    }

    float2 T[NKMC];
    // solve4x4<double2, double2, 0>(&(KK[0][0]),
    //                               &(T[0]));
    solve_NxN<double2, double2, 0, 4>(&(KK[0][0]), &(T[0]));

    float na(1.0f);
    if (Lna >= 1) na = getFBW2(s, 0.938272046f, 3.096916f, Lna, _FFB);
    for (int i = 0; i < NKMC + Npole; ++i) Mtr[i] = make_cuFloatComplex(0, 0);
    for (int ip = 0; ip < Npole; ++ip) {
        for (int i = 0; i < NKMC; ++i) {
            Mtr[ip] = cuCaddf(Mtr[ip], cuCmulcf(T[i], mg[ip][i]));
        }
        Mtr[ip] = cuCmulcf(Mtr[ip], na);
    }
    for (int i = 0; i < NKMC; ++i) {
        Mtr[Npole + i] = cuCmulcf(T[i], ms[0] * na);
    }

    if (LINEAR_KMNR) {
        const float *b_prod = &paraList[kmcount + 4 * (Npmax + NKMC) * NKMC + ik * NKMC];
        const float x = (mpp - m_jpsi - m_p) / (m_lb - m_k - m_jpsi - m_p);

        for (int i = 0; i < NKMC; i++) {
            Mtr[Npole + i] = Mtr[Npole + i] * (1 + b_prod[i] * x);
        }
    }

    return;
}

__device__ void KMatrix(const float mpp, const float *paraList, const int J_r, const int P_r, const int irst,
                        const int ired, const int kmcount, const int ik, float2 *Mtr, const float _FFB) {
    if (ISOSPIN_SIGMACD) {
        Isospin_KMatrix(mpp, paraList, J_r, P_r, irst, ired, kmcount, ik, Mtr, _FFB);
    } else {
        // Use the averaged masses and half-widths instead
        Default_KMatrix(mpp, paraList, J_r, P_r, irst, ired, kmcount, ik, Mtr, _FFB);
    }
}

__device__ float2 Revised_KMAMP(const float *paraList, const float mpp, const int nr, const int irst, const int ired,
                                const bool isPcChain, const float _FFR) {
    float m2pp = mpp * mpp;
    float m1, m2, m3;
    float m1_0, m2_0, m3_0;
    //  switch(iType) {
    //  case 0: //k*
    m1 = m_p;
    m2 = m_k;
    m3 = m_jpsi;
    m1_0 = m_p;
    m2_0 = m_k;
    m3_0 = m_jpsi;
    if (isPcChain) {
        m1 = m_jpsi;
        m2 = m_p;
        m3 = m_k;
        m1_0 = m_jpsi;
        m2_0 = m_p;
        m3_0 = m_k;
    }
    //    break;
    //  }
    int L_r, J_r, P_r, type_r;
    float m_r, gamma_r;
    m_r = paraList[nr * nset];
    gamma_r = paraList[nr * nset + 1];
    J_r = (int)(paraList[nr * nset + 2]);
    L_r = (int)(paraList[nr * nset + 3]);
    type_r = (int)(paraList[nr * nset + 4]);
    P_r = (int)(paraList[nr * nset + 5]);
    float totG(0);
    float fsc(0);
    float denorm(1);
    float m0, g0;
    int S, type, parity, LR;
    if (m_r > 0) denorm = (m_r * m_r - m2pp);
    for (int ir = irst; ir < ired; ++ir) {
        m0 = paraList[ir * nset];
        g0 = paraList[ir * nset + 1];
        S = (int)(paraList[ir * nset + 2]);
        LR = (int)(paraList[ir * nset + 3]);
        type = (int)(paraList[ir * nset + 4]);
        parity = (int)(paraList[ir * nset + 5]);
        if (type == type_r && S == J_r && parity == P_r) {
            if (ir == nr) continue;
            totG += KMatrixTerm(m0, g0, mpp, LR, fsc, _FFR) * denorm;
        }
    }
    float pR = getp(mpp, m1, m2);
    float pR0, mreff;
    if (m_r > m1_0 + m2_0) {
        mreff = m_r;
    } else if (m_r >= 0) {
        float mmin = m1_0 + m2_0;
        float mmax = m_lb - m3_0;
        mreff = mmin + 0.5f * (mmax - mmin) * (1.0f + tanh((m_r - (mmin + mmax) / 2.f) / (mmax - mmin)));
    } else {  // NR
        mreff = (m1_0 + m2_0 + m_lb - m3) / 2.0f;
    }
    pR0 = getp(mreff, m1_0, m2_0);
    float FR = F_BW(_FFR, L_r, pR, pR0);
    float factor = FR * powf((pR / pR0), L_r);
    float d2, dx, dy;  // = 1.+term*term;
    // for NR no more factor in the bracket {}
    if (m_r > 0) {
        float gamma = gamma_r;
        if (m_r > m1_0 + m2_0) gamma = gamma_r * powf((pR / pR0), 2 * L_r + 1) * (m_r / mpp) * FR * FR;
        if (m_r < m1_0 + m2_0) {  // Only validate for 1405 !!
            float msig = 1.18937f;
            float mpi0 = 0.13957018f;
            float pRSigPi = getp(mpp, msig, mpi0);
            // wehre LR = 0;
            gamma = 0.0f;
            pR0 = getp(m_r, msig, mpi0);
            FR = 1.0f;
            gamma += gamma_r * powf((pR / pR0), 2 * L_r + 1) * (m_r / mpp) * FR * FR;
            FR = 1.0f;
            gamma += 0.0505f * powf((pRSigPi / pR0), 2 * L_r + 1) * (m_r / mpp) * FR * FR;
            denorm = denorm - fsc * 0.0505f * powf((pRSigPi / pR0), 2 * L_r + 1) * (m_r / mpp) * FR * FR * m_r;
        }
        totG += m_r * gamma;
        float kr = sqrt(m_r * m_r + gamma_r * gamma_r);
        factor = factor * 0.948850f * m_r * sqrt(gamma_r * kr / sqrt(m_r * (m_r + kr)));
    } else {  // For NR
        //    fsc = gamma_r;
        totG += gamma_r * powf((pR / pR0), 2 * L_r + 1) * (m_r / mpp) * FR * FR;
        //    factor = factor * gamma_r;
    }
    d2 = denorm * denorm + totG * totG;
    dx = denorm;  // + fsc*totG;
    dy = totG;    // - fsc*denorm;

    float2 bw = make_cuFloatComplex(dx / d2, dy / d2);
    //  if(!(fsc==fsc)) {printf("wmzwmz try debug fsc=%f \n",fsc);}
    //  if(!(totG==totG)) {printf("wmzwmz try debug totG=%f \n",totG);}
    //  if(!(denorm==denorm)) {printf("wmzwmz try debug denorm=%f \n",denorm);}
    //  if(!(dx==dx)) {printf("wmzwmz try debug dx=%f \n",dx);}
    //  if(!(dy==dy)) {printf("wmzwmz try debug dy=%f \n",dy);}
    //  if(!(d2==d2)) {printf("wmzwmz try debug d2=%f \n",d2);}
    return cuCmulfc(factor, bw);
}

__device__ float product_delta_S(const float *paraList, const float m2pp, const int ir, const int type_r,
                                 const int parity_r, const int S_r, const int irst, const int ired) {
    float product(1);
    for (int jr = irst; jr < ired; jr++) {
        if (jr == ir) continue;

        float m0(paraList[jr * nset]);
        int S(paraList[jr * nset + 2]);
        int LR(paraList[jr * nset + 3]);
        int type(paraList[jr * nset + 4]);
        int parity(paraList[jr * nset + 5]);

        if ((type == type_r) && (S == S_r) && (parity == parity_r)) {
            product *= (m0 * m0 - m2pp);
        }
    }

    return product;
}

// Only for Ls chain
__device__ float m_gamma(const float m_r, const float gamma_r, const float mpp, const int LR, float &fsc,
                         const float _FFR) {
    const float m1 = m_p;
    const float m2 = m_k;
    const float m3 = m_jpsi;
    const float m1_0 = m_p;
    const float m2_0 = m_k;
    const float m3_0 = m_jpsi;

    float mreff;
    if (m_r > m1_0 + m2_0) {
        mreff = m_r;
    } else if (m_r >= 0) {
        float mmin = m1_0 + m2_0;
        float mmax = m_lb - m3_0;
        mreff = mmin + 0.5f * (mmax - mmin) * (1.0f + tanh((m_r - (mmin + mmax) / 2.f) / (mmax - mmin)));
    } else {  // NR
        mreff = (m1_0 + m2_0 + m_lb - m3) / 2.0f;
    }

    const float pR = getp(mpp, m1, m2);
    const float pR0 = getp(mreff, m1_0, m2_0);
    const float FR = F_BW(_FFR, LR, pR, pR0);
    const float gamma = gamma_r * powf((pR / pR0), 2 * LR + 1) * (m_r / mpp) * FR * FR;

    if (m_r < 0) {
        if (fabs(fsc) < 1e-10f) fsc = gamma;
        return fsc * (m_r * m_r - mpp * mpp);
    }

    return m_r * gamma;
}

__device__ float m_gamma_product_delta_S(const float *paraList, const float mpp, const int ir, const int irst,
                                         const int ired, float &fsc, const float _FFR) {
    const float m2pp = mpp * mpp;
    const float m0 = paraList[ir * nset];
    const float g0 = paraList[ir * nset + 1];
    const int S(paraList[ir * nset + 2]);
    const int LR(paraList[ir * nset + 3]);
    const int type(paraList[ir * nset + 4]);
    const int parity(paraList[ir * nset + 5]);

    const float m_gamma_term = m_gamma(m0, g0, mpp, LR, fsc, _FFR);
    const float product_delta_S_term = product_delta_S(paraList, m2pp, ir, type, parity, S, irst, ired);

    return m_gamma_term * product_delta_S_term;
}

__device__ float2 Revised_KMAMP_refactor(const float *paraList, float mpp, int nr, int irst, int ired, bool isPcChain,
                                         float _FFR) {
    const float m2pp = mpp * mpp;
    float m1, m2, m3;
    float m1_0, m2_0, m3_0;
    if (isPcChain) {
        m1 = m_jpsi;
        m2 = m_p;
        m3 = m_k;
        m1_0 = m_jpsi;
        m2_0 = m_p;
        m3_0 = m_k;
    } else {
        m1 = m_p;
        m2 = m_k;
        m3 = m_jpsi;
        m1_0 = m_p;
        m2_0 = m_k;
        m3_0 = m_jpsi;
    }

    const float m_r = paraList[nr * nset];
    const float gamma_r = paraList[nr * nset + 1];
    const int S_r(paraList[nr * nset + 2]);
    const int L_r(paraList[nr * nset + 3]);
    const int type_r(paraList[nr * nset + 4]);
    const int P_r(paraList[nr * nset + 5]);

    float fsc(0);

    float mreff;
    if (m_r > m1_0 + m2_0) {
        mreff = m_r;
    } else if (m_r >= 0) {
        float mmin = m1_0 + m2_0;
        float mmax = m_lb - m3_0;
        mreff = mmin + 0.5f * (mmax - mmin) * (1.0f + tanh((m_r - (mmin + mmax) / 2.f) / (mmax - mmin)));
    } else {  // NR
        mreff = (m1_0 + m2_0 + m_lb - m3) / 2.0f;
    }
    const float pR = getp(mpp, m1, m2);
    const float pR0 = getp(mreff, m1_0, m2_0);
    const float FR = F_BW(_FFR, L_r, pR, pR0);
    const float gamma = gamma_r * powf((pR / pR0), 2 * L_r + 1) * (m_r / mpp) * FR * FR;

    const float m_gamma_r = m_r * gamma;
    const float product_delta_S_r = product_delta_S(paraList, m2pp, nr, type_r, P_r, S_r, irst, ired);
    const float product_delta_S_all = product_delta_S_r * (m_r * m_r - m2pp);

    float sum_resonance_term = product_delta_S_r * m_gamma_r;

    for (int ir = irst; ir < ired; ir++) {
        if (ir == nr) continue;
        const int S(paraList[ir * nset + 2]);
        const int type(paraList[ir * nset + 4]);
        const int P(paraList[ir * nset + 5]);
        if ((S == S_r) && (P == P_r) && (type == type_r)) {
            sum_resonance_term += m_gamma_product_delta_S(paraList, mpp, ir, irst, ired, fsc, _FFR);
        }
    }

    const float denorm = product_delta_S_all * product_delta_S_all + sum_resonance_term * sum_resonance_term;
    const float real = product_delta_S_r * product_delta_S_all;
    const float imag = product_delta_S_r * sum_resonance_term;

    const float2 mass_dependent_term = make_cuFloatComplex(real / denorm, imag / denorm);
    const float kr = sqrt(m_r * m_r + gamma_r * gamma_r);
    const float factor = FR * powf((pR / pR0), L_r) * 0.948850f * m_r * sqrt(gamma_r * kr / sqrt(m_r * (m_r + kr)));

    return cuCmulfc(factor, mass_dependent_term);
}

__device__ float KMatrixTerm(float m_r, float gamma_r, float mpp, int LR, float &fsc, float _FFR) {
    float m2pp = mpp * mpp;
    float m1, m2, m3;
    float m1_0, m2_0, m3_0;
    //  switch(iType) {
    //  case 0: //L*
    m1 = m_p;
    m2 = m_k;
    m3 = m_jpsi;
    m1_0 = m_p;
    m2_0 = m_k;
    m3_0 = m_jpsi;
    //    break;
    //
    //  }
    float pR = getp(mpp, m1, m2);
    float pR0, mreff;
    if (m_r > m1_0 + m2_0) {
        mreff = m_r;
    } else if (m_r >= 0) {
        float mmin = m1_0 + m2_0;
        float mmax = m_lb - m3_0;
        mreff = mmin + 0.5f * (mmax - mmin) * (1.0f + tanh((m_r - (mmin + mmax) / 2.f) / (mmax - mmin)));
    } else {  // NR
        mreff = (m1_0 + m2_0 + m_lb - m3) / 2.0f;
    }
    pR0 = getp(mreff, m1_0, m2_0);
    float FR = F_BW(_FFR, LR, pR, pR0);
    ;
    float gamma = gamma_r;
    // if(m_r>m1_0+m2_0)
    gamma = gamma_r * powf((pR / pR0), 2 * LR + 1) * (m_r / mpp) * FR * FR;
    if (m_r < 0) {
        // changed:: 2020.12.27
        if (fabs(fsc) < 1e-10f) fsc = gamma;
        return fsc;  // gamma_r as fsc;
    }
    // if(iType==1&&m_r<4.18) gamma = gamma_r;

    //  if(m_r*m_r-m2pp>-1e-6 &&m_r*m_r-m2pp<1e-6) {printf("wmzwmz try debug,
    //  m_r*m_r-m2pp = %f \n",m_r*m_r-m2pp);}
    if (fabs(m_r * m_r - m2pp) < 1e-10f) {
        return 0;
    }
    return m_r * gamma / (m_r * m_r - m2pp);
}

// implemented from TSpline3
__device__ float2 ModelIndependentWave3(const float *fPoly, float x) {
    float sumre(0.0f);
    float sumim(0.0f);

    // ms – input mass i.e. m(J/psi p) in this case
    // Rea,Ima – real and imaginary parts of the amplitude

    // get binning and number of interpolation points

    const int fNp = NUMSP;

    if (fNp <= 0) return make_cuFloatComplex(0, 0);

    float fXmin = m_p + m_k - 0.002f;      // 1.513;//1.43;//((RooAbsReal&)res24[3]).getVal();
    float fXmax = m_lb - m_jpsi + 0.002f;  // 2.185;//4.416;//5.03;//2.53;//((RooAbsReal&)res24[4]).getVal();

    if (x < fXmin) return make_cuFloatComplex(0, 0);
    if (x > fXmax) return make_cuFloatComplex(0, 0);

    // The above is the spline interpolation of the magnitude (called sumamp),
    // real and imaginary parts.

    // spline3 from root
    int klow = 0, khig = fNp - 1;
    //
    // If out of boundaries, extrapolate
    // It may be badly wrong
    if (x <= fXmin)
        klow = 0;
    else if (x >= fXmax)
        klow = khig;
    else {
        int khalf;
        //
        // Non equidistant knots, binary search
        while (khig - klow > 1)
            if (x > fPoly[khalf = (klow + khig) / 2])
                klow = khalf;
            else
                khig = khalf;
        //
    }
    // this is the number to be used klow;
    //  Evaluate now
    float dx = x - fPoly[klow];
    int ist = 4 * klow + fNp;
    sumre = (fPoly[ist] + dx * (fPoly[ist + 1] + dx * (fPoly[ist + 2] + dx * fPoly[ist + 3])));
    ist = ist + 4 * fNp;
    sumim = (fPoly[ist] + dx * (fPoly[ist + 1] + dx * (fPoly[ist + 2] + dx * fPoly[ist + 3])));
    return make_cuFloatComplex(sumre, sumim);
}

// implemented from TSpline3
__device__ float2 ModelIndependentWave(const float *fPoly, float x) {
    float sumre(0.0f);
    float sumim(0.0f);

    // ms – input mass i.e. m(J/psi p) in this case
    // Rea,Ima – real and imaginary parts of the amplitude

    // get binning and number of interpolation points

    const int fNp = NUMSPZ;

    if (fNp <= 0) return make_cuFloatComplex(0, 0);

    float fXmin = m_p + m_jpsi - 0.002f;  // 1.513;//1.43;//((RooAbsReal&)res24[3]).getVal();
    float fXmax = 4.6f;  // m_lb-m_k+0.002;//2.185;//4.416;//5.03;//2.53;//((RooAbsReal&)res24[4]).getVal();

    if (x < fXmin) return make_cuFloatComplex(0, 0);
    if (x > fXmax) return make_cuFloatComplex(0, 0);

    // The above is the spline interpolation of the magnitude (called sumamp),
    // real and imaginary parts.

    // spline3 from root
    int klow = 0, khig = fNp - 1;
    //
    // If out of boundaries, extrapolate
    // It may be badly wrong
    if (x <= fXmin)
        klow = 0;
    else if (x >= fXmax)
        klow = khig;
    else {
        int khalf;
        //
        // Non equidistant knots, binary search
        while (khig - klow > 1)
            if (x > fPoly[khalf = (klow + khig) / 2])
                klow = khalf;
            else
                khig = khalf;
        //
    }
    // this is the number to be used klow;
    //  Evaluate now
    float dx = x - fPoly[klow];
    int ist = 4 * klow + fNp;
    sumre = (fPoly[ist] + dx * (fPoly[ist + 1] + dx * (fPoly[ist + 2] + dx * fPoly[ist + 3])));
    ist = ist + 4 * fNp;
    sumim = (fPoly[ist] + dx * (fPoly[ist + 1] + dx * (fPoly[ist + 2] + dx * fPoly[ist + 3])));
    return make_cuFloatComplex(sumre, sumim);
}

__device__ float2 get_b(float2 alpha, float2 p23) {
    float2 unit = make_cuFloatComplex(1, 0);
    float2 unit2 = make_cuFloatComplex(2, 0);
    float2 unit4 = make_cuFloatComplex(4, 0);
    float2 p12 = make_cuFloatComplex(m_lb, 0);
    float2 p13 = make_cuFloatComplex(m_k, 0);

    float2 m2 = make_cuFloatComplex(m_chic1, 0);
    float2 m3 = make_cuFloatComplex(m_p, 0);
    float2 ii = make_cuFloatComplex(0, 1);
    // cnum value = unit + unit/p23/p23*( alpha*(p12*p12-p13*p13-p23*p23) +
    // m3*m3 - m2*m2) - 0.001*ii; float2 value = unit + unit/p23/p23*(
    // alpha*(p12*p12-p13*p13-p23*p23) + m3*m3 - m2*m2) - 0.001*ii;

    float2 this_value =
        cuCsubf(cuCaddf(cuCmulf(alpha, cuCsubf(cuCsubf(cuCmulf(p12, p12), cuCmulf(p13, p13)), cuCmulf(p23, p23))),
                        cuCmulf(m3, m3)),
                cuCmulf(m2, m2));
    float2 value = cuCsubf(cuCaddf(cuCmulf(cuCdivf(cuCdivf(unit, p23), p23), this_value), unit), cuCmulfc(0.001f, ii));

    //   printf("Test get_b value_real:%f  value_imag:%f \n",  cuCreal(value),
    //   cuCimag(value));

    return value;
}

__device__ float2 get_c(float2 alpha, float2 p23, float m_r, float gamma) {
    float2 unit = make_cuFloatComplex(1, 0);
    float2 unit2 = make_cuFloatComplex(2, 0);
    float2 unit4 = make_cuFloatComplex(4, 0);
    float2 p12 = make_cuFloatComplex(m_lb, 0);
    float2 p13 = make_cuFloatComplex(m_k, 0);

    float2 m1 = make_cuFloatComplex(m_r, -1 * gamma / 2.f);
    float2 m2 = make_cuFloatComplex(m_chic1, 0);
    float2 m3 = make_cuFloatComplex(m_p, 0);
    float2 ii = make_cuFloatComplex(0, 1);

    // cnum value = unit/p23/p23*( m3*m3 +
    // alpha*(m1*m1-m3*m3)-alpha*(unit-alpha)*p13*p13);

    float2 this_value1 = cuCaddf(cuCmulf(alpha, cuCsubf(cuCmulf(m1, m1), cuCmulf(m3, m3))), cuCmulf(m3, m3));
    float2 this_value2 = cuCmulf(cuCmulf(alpha, cuCsubf(unit, alpha)), cuCmulf(p13, p13));
    float2 value = cuCmulf(cuCdivf(cuCdivf(unit, p23), p23), cuCsubf(this_value1, this_value2));

    // printf("Test get_c mpp:%f  alpha:%f value_real:%f  value_imag:%f \n",
    // cuCreal(p23), cuCreal(alpha), cuCreal(value), cuCimag(value));

    return value;
}

__device__ float2 this_func(float alpha1, float p23_real, float m_r, float gamma) {
    float2 unit = make_cuFloatComplex(1, 0);
    float2 unit2 = make_cuFloatComplex(2, 0);
    float2 unit4 = make_cuFloatComplex(4, 0);
    float2 p12 = make_cuFloatComplex(m_lb, 0);
    float2 p13 = make_cuFloatComplex(m_k, 0);

    float2 m2 = make_cuFloatComplex(m_chic1, 0);
    float2 m3 = make_cuFloatComplex(m_p, 0);
    float2 ii = make_cuFloatComplex(0, 1);

    float2 alpha = make_cuFloatComplex(alpha1, 0);
    float2 p23 = make_cuFloatComplex(p23_real, 0);
    float2 sqrt_value =
        cuCsqrtf(cuCsubf(cuCmulf(unit4, get_c(alpha, p23, m_r, gamma)), cuCmulf(get_b(alpha, p23), get_b(alpha, p23))));

    float2 tan1 = cuCatanf(cuCdivf(get_b(alpha, p23), sqrt_value));
    float2 tan2 = cuCatanf(cuCdivf(cuCaddf(get_b(alpha, p23), cuCmulf(unit2, cuCsubf(alpha, unit))), sqrt_value));
    float2 value = cuCmulf(cuCdivf(unit, sqrt_value), cuCsubf(tan1, tan2));

    // cnum value = unit/sqrt_value * ( atan(get_b(alpha,p23)/sqrt_value) -
    // atan( (get_b(alpha,p23)+unit2*(alpha-unit)) /sqrt_value) );

    // printf("Test this_function mpp:%f  alpha:%f sqrt_real:%f   sqrt_imag:%f
    // tan1_real:%f tan1_imag:%f tan2_real:%f tan2_imag:%f value_real:%f
    // value_imag:%f \n",
    //	   cuCreal(p23), cuCreal(alpha),
    //	   cuCreal(sqrt_value), cuCimag(sqrt_value),
    //	   cuCreal(tan1), cuCimag(tan1),
    //	   cuCreal(tan2), cuCimag(tan2),
    //	   cuCreal(value), cuCimag(value));
    // cnum sqrt_value = sqrt(unit4*get_c(alpha,p23, m_r,
    // gamma)-get_b(alpha,p23)*get_b(alpha,p23));
    return value;
}

__device__ float2 get_cusp_ampli(float m, float gamma, float mpp) {
    float2 integral_value = make_cuFloatComplex(0, 0);
    int bins = 100;
    float delta_x_real = 1. / bins;
    float2 delta_x = make_cuFloatComplex(delta_x_real, 0);

    for (int i = 0; i < bins; i++) {
        float center_x = i * delta_x_real + delta_x_real / 2;
        // integral_value = integral_value + delta_x*this_func(center_x, p23,
        // m_r, gamma);
        integral_value = cuCaddf(integral_value, cuCmulf(delta_x, this_func(center_x, mpp, m, gamma)));
    }
    // cnum value = 1/8./3.1415926/3.1415926/p23/p23*integral_value;
    float2 value = cuCmulfc(1.f / mpp / mpp, integral_value);
    // std::cout << " ampli : " << value.imag() << std::endl;

    //  printf("Test get_cusp_ampli m_r:%f  gamma_r:%f  mpp:%f  r_ampl:%f
    //  i_ampl:%f \n ", m, gamma, mpp, cuCrealf(value), cuCimagf(value) );

    return value;

    // float r_ampl = value.real();
    // float i_ampl = value.imag();
    // return make_cuFloatComplex(0, 0);
}

// For new cusp

__device__ float2 GetCusp_AMP(float m_r, float gamma_r, float mpp) {
    float2 bw = get_cusp_ampli(m_r, gamma_r, mpp);

    // return cuCmuldc(factor, bw );
    return cuCmulfc(1, bw);
}
