#include <cuda_runtime.h>
#include <thrust/reduce.h>
#include <thrust/device_ptr.h>
#include "../RooTDalitz/cuComplex.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "../RooTDalitz/cu_DPFPropogator.h"
#include "../RooTDalitz/kernel_calEva.h"
#include <assert.h>
#include "../RooTDalitz/MultDevice.h"
#include "../RooTDalitz/cu_Jpsihh_dlz.h"
#include "../RooTDalitz/Resonances.h"
#include "../RooTDalitz/cuSolve4x4.h"
#include "../RooTDalitz/extended_cuComplex.h"
// #include "nvToolsExt.h"
using namespace std;
#include "nvtx3/nvToolsExt.h"

// #define Calculate_d_fx_MC_noInteference

/*
   const float _smear_nsigmas = 10;
   const float _smear_nbins = 30;
   const float _mres0 = -0.134064 ;
   const float _mres1 =  0.0599014;
   const float _mres2 = -0.00655841;
 */

#define BW_Lambda
// #define KMatrix_Lambda

// #define BW_Pc
#define KMatrix_Pc

// #define NUMLz 18 //#Lz
// #define NUMPc 4 //#Pc

// CUDA constants of fitting parameters
__constant__ float _constant_paraList[2048];

// CUDA constants for smearing
__constant__ float _constant_smear_binning_scheme[SMEAR_NBINS];
__constant__ float _constant_smear_fine_binning_scheme[SMEAR_FINE_NBINS];
__constant__ float _constant_smear_bad_binning_scheme[SMEAR_BAD_NBINS];

// Other CUDA constants which can increase parallelism
// CUDA constants that configure Pc K-matrix and helicity coupling
// CUDA constants that configure
__constant__ int _constant_Pc_ikmcount[Constant::number_of_Pc];
__constant__ int _constant_ampcount[Constant::number_of_resonances];
__constant__ int _constant_splcount[Constant::number_of_resonances];

// CUDA constants used during fit
__constant__ int _constant_sizes[3];
__constant__ float _constant_form_factors[2];

// 将任何cuda函数作为CUDA_CALL的参数，能够显示返回的错误，并定位错误。
#define CUDA_CALL(x)                                                                                     \
    {                                                                                                    \
        const cudaError_t a = (x);                                                                       \
        if (a != cudaSuccess)                                                                            \
        {                                                                                                \
            printf("\nerror in line:%d CUDAError:%s(err_num=%d)\n", __LINE__, cudaGetErrorString(a), a); \
            cudaDeviceReset();                                                                           \
            assert(0);                                                                                   \
        }                                                                                                \
    }
// block_size 的设定要考虑shared memory 的大小
// shared memory per block ：48k bytes
// 每个block所使用的共享空间大小 ：paraList+vampList+paraSplineList)*sizeof(dobule) + 337×sizeof(float)xBLOCK_SIZE
// 控制BLOCK_SIZE 使所使用的shared memory 不可高于上限
#define BLOCK_SIZE 256

double *d_fx[DEVICE_NUM];
double *d_fx_MC_noInteference[DEVICE_NUM];
double *d_nll[DEVICE_NUM];
int Ns[DEVICE_NUM + 1];
bool once_flag_config = 1;
// const int nset  = 6;
// GetAmp2s是在gpu中运行的一个子程序
// float * intermediate_GetAmp2s[DEVICE_NUM];

template <const int NR, typename CSUMTYPE>
__device__ __forceinline__ static void GetAmpLz(const int ip,
                                                const float *pp,
                                                const int N_size,
                                                const float _FFR,
                                                const float _FFB,
                                                CSUMTYPE *H_L_Zcs)
{
    // pp是全局寄存器变量，读取速度不高，先拷到局部寄存器，可节省20%时间［需读取NR次］
    float ARe_[2][2][6];
    float AIm_[2][2][6];
    float DL_[5][2][6];
    for (int i = 0; i <= 1; ++i)
    { // delmu
        for (int j = 0; j <= 1; ++j)
        { // hel0
#pragma unroll
            for (int m = 0; m < 6; ++m)
            {
                ARe_[i][j][m] = (pp[ip + (3 + (i * 6 * 2 + j * 6 + m) * 2) * N_size]);     // pp->ARe[i][j][m];
                AIm_[i][j][m] = (pp[ip + (3 + (i * 6 * 2 + j * 6 + m) * 2 + 1) * N_size]); // pp->AIm[i][j][m];
            }
        }
    }
    for (int i = 0; i < 5; ++i)
    {
        for (int k = 0; k <= 1; ++k)
        {
#pragma unroll
            for (int m = 0; m < 6; ++m)
            {
                DL_[i][k][m] = (pp[ip + (51 + i * 6 * 2 + k * 6 + m) * N_size]); // pp->DL[i][k][m];
            }
        }
    }

    const float mkp(pp[ip]);
    for (int iLz = 0; iLz < NR; iLz++)
    {
        const int ir = iLz;
        float m0 = _constant_paraList[ir * nset + MASS];
        const float g0 = _constant_paraList[ir * nset + WIDTH];
        const int S2 = (int)_constant_paraList[ir * nset + SPIN];
        const int LR = (int)_constant_paraList[ir * nset + LRESON];
        const float S = 0.5f * S2;
        const int type = (int)_constant_paraList[ir * nset + TYPE];

        float2 BW;
        switch (type)
        {
        case BREITWIGNER:
        {
            BW = BW_AMP(m0, g0, mkp, LR, _FFR);
            break;
        }
        case REVISEDKMATRIX:
        {
            BW = Revised_KMAMP(_constant_paraList, mkp, ir, 0, NR, 0, _FFR);
            break;
        }
        case MODELINDEPENDENT:
        {
            BW = ModelIndependentWave3(&_constant_paraList[_constant_splcount[ir]], mkp);
            m0 = -0.5f;
            break;
        }
        case NONRESONANT:
        {
            BW = {1.0f, 0.0f};
            break;
        }
        }

        const float Bterm = BTerm(S, mkp, m0, _FFB);
        BW = cuCmulfc(Bterm, BW);

        int ampcount = _constant_ampcount[ir];
        const int spin_index = (S2 - 1) / 2;
        for (int k = 0; k <= 1; ++k)
        { // hel3 proton
            for (int m = 0; m < 6; ++m)
            {

                if (fabs(S - 0.5f) < 1e-10f && m >= 4)
                {
                    continue;
                }
                const float2 ampCoff = make_cuFloatComplex(_constant_paraList[ampcount++],
                                                           _constant_paraList[ampcount++]);
                if (cuCabsf(ampCoff) < 1e-10f)
                    continue;

#pragma unroll
                for (int i = 0; i <= 1; ++i)
                { // delmu
#pragma unroll
                    for (int j = 0; j <= 1; ++j)
                    { // hel0
                        const float2 Atmp2 = DL_[spin_index][k][m] * (make_cuFloatComplex(ARe_[i][j][m], AIm_[i][j][m]) * BW);
                        H_L_Zcs[4 * i + 2 * j + k] = H_L_Zcs[4 * i + 2 * j + k] + Atmp2 * ampCoff;
                    }
                }
            }
        }
    }
}

template <const int NR, const int NZ, const int NX, typename CSUMTYPE>
__device__ __forceinline__ static void GetAmpZcs(const int ip,
                                                 const float *pp3,
                                                 const float *pp4,
                                                 const int N_size,
                                                 const float _FFR,
                                                 const float _FFB,
                                                 CSUMTYPE *H_L_Zcs)
{
    float AXRe_[2][2][12];
    float AXIm_[2][2][12];
    float DX_[4][2][12];
    for (int i = 0; i <= 1; ++i)
    { // delmu
        for (int j = 0; j <= 1; ++j)
        { // hel0
#pragma unroll
            for (int m = 0; m < 12; ++m)
            {
                AXRe_[i][j][m] = (pp3[ip + (1 + (i * 12 * 2 + j * 12 + m) * 2) * N_size]);
                AXIm_[i][j][m] = (pp3[ip + (1 + (i * 12 * 2 + j * 12 + m) * 2 + 1) * N_size]);
            }
        }
    }
    for (int i = 0; i < 4; ++i)
    {
        for (int k = 0; k <= 1; ++k)
        {
#pragma unroll
            for (int m = 0; m < 12; ++m)
            {
                DX_[i][k][m] = (pp4[ip + (i * 12 * 2 + k * 12 + m) * N_size]);
            }
        }
    }

    const float mjpsik = pp3[ip];
    ;
    for (int iZcs = 0; iZcs < NX; iZcs++)
    {
        const int ir = NR + NZ + iZcs;
        const float m0 = _constant_paraList[ir * nset + MASS];
        const float g0 = _constant_paraList[ir * nset + WIDTH];
        const int S2 = (int)_constant_paraList[ir * nset + SPIN];
        const int LR = (int)_constant_paraList[ir * nset + LRESON];
        const float S = 0.5f * S2;
        const int type = (int)_constant_paraList[ir * nset + TYPE];

        const float Bterm = BTermX(S, mjpsik, m0, _FFB);
        const float2 BW = Bterm * BW_AMPX(m0, g0, mjpsik, LR, _FFR);

        int ampcount = _constant_ampcount[ir];
        const int spin_index = (S2) / 2;
        for (int k = 0; k <= 1; ++k)
        { // hel3 proton
            // there are 12 amplitudes to be added coherently //helpsi,0,+/-1, helZ,+/-1/2, help=+/-1/2
            int m(0);
            for (float hel_psi = -1; hel_psi <= 1; hel_psi++)
            {
                if (abs(hel_psi) > S)
                    continue;

                for (float hel_p = 0; hel_p <= 1; hel_p++)
                {
                    for (float hel_Z = hel_p - 0.5; hel_Z <= hel_p + 0.5; hel_Z++)
                    {

                        if (fabs(hel_Z) > S)
                            continue;

                        const float2 ampCoff = make_cuFloatComplex(_constant_paraList[ampcount++],
                                                                   _constant_paraList[ampcount++]);
                        if (cuCabsf(ampCoff) < 1e-10)
                        {
                            m++;
                            continue;
                        }

#pragma unroll
                        for (int i = 0; i <= 1; ++i)
                        { // delmu
#pragma unroll
                            for (int j = 0; j <= 1; ++j)
                            {
                                const float2 Atmp2 = DX_[spin_index][k][m] * (make_cuFloatComplex(AXRe_[i][j][m], AXIm_[i][j][m]) * BW);
                                H_L_Zcs[4 * i + 2 * j + k] = H_L_Zcs[4 * i + 2 * j + k] + Atmp2 * ampCoff;
                            }
                        }
                        m++;
                    }
                }
            }
        }
    }
}

template <const int NR, const int NZ, const int NX, bool interference, typename CSUMTYPE, typename SUMTYPE>
__device__ double GetAmp2s(const int ip,
                           const float *pp,
                           const float *pp1,
                           const float *pp2,
                           const float *pp3,
                           const float *pp4,
                           const int N_size)
////return square of complex amplitude
{
    const int para_size = _constant_sizes[0];
    const int vamp_size = _constant_sizes[1];
    const int paraSpline_size = _constant_sizes[2];
    const float _FFR = _constant_form_factors[0];
    const float _FFB = _constant_form_factors[1];

    const int kmcount = para_size + vamp_size + paraSpline_size;

    CSUMTYPE H_L_Zcs[8];
    SUMTYPE H_total(0);

#pragma unroll
    for (int i = 0; i < 8; i++)
    {
        H_L_Zcs[i] = {0, 0};
    }

    /**************
        Lz part
    **************/
    GetAmpLz<NR>(ip, pp, N_size, _FFR, _FFB, H_L_Zcs);
    if (not(interference))
    {
        H_total = magnitude2(H_L_Zcs[0]) + magnitude2(H_L_Zcs[1]) + magnitude2(H_L_Zcs[2]) + magnitude2(H_L_Zcs[3]) + magnitude2(H_L_Zcs[4]) + magnitude2(H_L_Zcs[5]) + magnitude2(H_L_Zcs[6]) + magnitude2(H_L_Zcs[7]);

        for (int i = 0; i <= 1; ++i)
        {
            for (int j = 0; j <= 1; ++j)
            {
                for (int k = 0; k <= 1; ++k)
                {
                    H_L_Zcs[4 * i + 2 * j + k] = {0, 0};
                }
            }
        }
    }

    /**************
        Zcs part
    **************/
    if constexpr (NX > 0)
    {
        GetAmpZcs<NR, NZ, NX>(ip, pp3, pp4, N_size, _FFR, _FFB, H_L_Zcs);
        if (not(interference))
        {
            H_total += magnitude2(H_L_Zcs[0]) + magnitude2(H_L_Zcs[1]) + magnitude2(H_L_Zcs[2]) + magnitude2(H_L_Zcs[3]) + magnitude2(H_L_Zcs[4]) + magnitude2(H_L_Zcs[5]) + magnitude2(H_L_Zcs[6]) + magnitude2(H_L_Zcs[7]);
        }
    }

    /**************
        Pc part
    **************/
    CSUMTYPE H_Z[8];
#pragma unroll
    for (int i = 0; i < 8; ++i)
    {
        H_Z[i] = {0, 0};
    }

    // Maybe try angular loop first, then lineshape loop

    float AZRe_[2][2][12];
    float AZIm_[2][2][12];
    float DZ_[4][2][12];
    for (int i = 0; i <= 1; ++i)
    { // delmu
        for (int j = 0; j <= 1; ++j)
        { // hel0
#pragma unroll
            for (int m = 0; m < 12; ++m)
            {
                AZRe_[i][j][m] = (pp1[ip + ((i * 12 * 2 + j * 12 + m) * 2) * N_size]);     // pp->AZRe[i][j][m];
                AZIm_[i][j][m] = (pp1[ip + ((i * 12 * 2 + j * 12 + m) * 2 + 1) * N_size]); // pp->AZIm[i][j][m];
            }
        }
    }
    for (int i = 0; i < 4; ++i)
    {
        for (int k = 0; k <= 1; ++k)
        {
#pragma unroll
            for (int m = 0; m < 12; ++m)
            {
                DZ_[i][k][m] = (pp2[ip + (i * 12 * 2 + k * 12 + m) * N_size]); // pp->DZ[i][k][m];
            }
        }
    }

    const float mjpsip(pp[ip + N_size]);
    float2 AKM[4][Constant::max_ikmcount];
    AKM[0][0] = make_cuFloatComplex(0.0f, 0.0f);
    AKM[1][0] = make_cuFloatComplex(0.0f, 0.0f);
    AKM[2][0] = make_cuFloatComplex(0.0f, 0.0f);
    AKM[3][0] = make_cuFloatComplex(0.0f, 0.0f);

    for (int iPc = 0; iPc < NZ; iPc++)
    {
        const int ir = NR + iPc;
        float m0 = _constant_paraList[ir * nset + MASS];
        const float g0 = _constant_paraList[ir * nset + WIDTH];
        const int S2 = (int)_constant_paraList[ir * nset + SPIN];
        const int LR = (int)_constant_paraList[ir * nset + LRESON];
        const float S = 0.5f * S2;
        const int type = (int)_constant_paraList[ir * nset + TYPE];

        float2 BW;
        switch (type)
        {
        case MODELINDEPENDENT:
        {
            BW = ModelIndependentWave(&_constant_paraList[_constant_splcount[ir]], mjpsip);
            m0 = -0.5f;
            break;
        }
        case BREITWIGNER:
        {
            BW = BW_AMPZ(m0, g0, mjpsip, LR, _FFR);
            break;
        }
        case REVISEDKMATRIX:
        {
            BW = Revised_KMAMP(_constant_paraList, mjpsip, ir, NR, NR + NZ, 1, _FFR);
            break;
        }
        case TRIANGLE:
        {
            BW = GetCusp_AMP(m0, g0, mjpsip);
            break;
        }
        case KMATRIX:
        {
            const int ik = (LR > 0)
                               ? (S2 == 3) ? 3 : 2
                           : (S2 == 3) ? 1
                                       : 0;
            if (cuCabsf(AKM[ik][0]) < 1e-10f)
            {
                KMatrix(mjpsip,
                        _constant_paraList,
                        S2,
                        (LR > 0) ? 1 : -1,
                        NR,
                        NR + NZ,
                        kmcount,
                        ik,
                        AKM[ik],
                        _FFB);
            }
            const int ikmcount = _constant_Pc_ikmcount[iPc];
            BW = AKM[ik][ikmcount];
            break;
        }
        case NONRESONANT:
        {
            BW = {1.0f, 0.0f};
            break;
        }
        }

        const float Bterm = BTermZ(S, mjpsip, m0, _FFB);
        BW = Bterm * BW;

        int ampcount = _constant_ampcount[ir];
        const int spin_index = (S2 - 1) / 2;
        for (int k = 0; k <= 1; ++k)
        { // hel3 proton
            // there are 12 amplitudes to be added coherently //helpsi,0,+/-1, helZ,+/-1/2, help=+/-1/2
            int m(0);
            for (float hel_psi = -1; hel_psi <= 1; hel_psi++)
            {
                for (int ihz = 0; ihz <= 1; ihz++)
                {
                    for (float hel_p = -0.5; hel_p <= 0.5; hel_p++)
                    {

                        if (fabsf(hel_psi - hel_p) > S)
                        {
                            m++;
                            continue;
                        }

                        const float2 ampCoff = make_cuFloatComplex(_constant_paraList[ampcount++],
                                                                   _constant_paraList[ampcount++]);
                        if (cuCabsf(ampCoff) < 1e-10f)
                        {
                            m++;
                            continue;
                        }

#pragma unroll
                        for (int i = 0; i <= 1; ++i)
                        { // delmu
#pragma unroll
                            for (int j = 0; j <= 1; ++j)
                            { // hel0
                                const float2 Atmp2 = DZ_[spin_index][k][m] * (make_cuFloatComplex(AZRe_[i][j][m], AZIm_[i][j][m]) * BW);
                                H_Z[4 * i + 2 * j + k] = H_Z[4 * i + 2 * j + k] + Atmp2 * ampCoff;
                            }
                        }
                        m++;
                    }
                }
            }
        }
    }

    if (not(interference))
    {
        H_total += magnitude2(H_Z[0]) + magnitude2(H_Z[1]) + magnitude2(H_Z[2]) + magnitude2(H_Z[3]) + magnitude2(H_Z[4]) + magnitude2(H_Z[5]) + magnitude2(H_Z[6]) + magnitude2(H_Z[7]);
    }

    if (interference)
    {
        H_total = magnitude2(H_L_Zcs[0] + H_Z[0]) + magnitude2(H_L_Zcs[1] + H_Z[1]) + magnitude2(H_L_Zcs[2] + H_Z[2]) + magnitude2(H_L_Zcs[3] + H_Z[3]) + magnitude2(H_L_Zcs[4] + H_Z[4]) + magnitude2(H_L_Zcs[5] + H_Z[5]) + magnitude2(H_L_Zcs[6] + H_Z[6]) + magnitude2(H_L_Zcs[7] + H_Z[7]);
    }

    return H_total;
}

template <const int NR, const int NZ, const int NX, const int NBINS, const float *binning_scheme, typename CSUMTYPE, typename SUMTYPE>
__device__ SUMTYPE TemplateGetAmp2s(
    const int ip,
    const float *pp,
    const float *pp1,
    const float *pp2,
    const float *pp3,
    const float *pp4,
    const int N_size)
// everything else in constant memory
{
    const int para_size = _constant_sizes[0];
    const int vamp_size = _constant_sizes[1];
    const int paraSpline_size = _constant_sizes[2];
    const float _FFR = _constant_form_factors[0];
    const float _FFB = _constant_form_factors[1];
    const int kmcount = para_size + vamp_size + paraSpline_size;

    CSUMTYPE H_L_Zcs[8];
#pragma unroll
    for (int i = 0; i < 8; i++)
    {
        H_L_Zcs[i] = {0, 0};
    }

    /**************
        Lz part
    **************/
    GetAmpLz<NR>(ip, pp, N_size, _FFR, _FFB, H_L_Zcs);

    /**************
        Zcs part
    **************/
    if constexpr (NX > 0)
    {
        GetAmpZcs<NR, NZ, NX>(ip, pp3, pp4, N_size, _FFR, _FFB, H_L_Zcs);
    }

    /**************
        Pc part
    **************/
    CSUMTYPE HZ[NZ][8];
    CSUMTYPE BW1Pc[NZ];
    SUMTYPE BW2PcRR[NZ][NZ];
    SUMTYPE BW2PcII[NZ][NZ];
    SUMTYPE BW2PcIR[NZ][NZ];
    SUMTYPE BW2PcRI[NZ][NZ];

    for (int iPc = 0; iPc < NZ; ++iPc)
    {
#pragma unroll
        for (int i = 0; i < 8; i++)
        {
            HZ[iPc][i] = {0, 0};
        }

        BW1Pc[iPc] = {0, 0};
        for (int jPc = 0; jPc < NZ; ++jPc)
        {
            BW2PcRR[iPc][jPc] = 0;
            BW2PcII[iPc][jPc] = 0;
            BW2PcIR[iPc][jPc] = 0;
            BW2PcRI[iPc][jPc] = 0;
        }
    }

    float AZRe_[2][2][12];
    float AZIm_[2][2][12];
    float DZ_[4][2][12];
    for (int i = 0; i <= 1; ++i)
    { // delmu
        for (int j = 0; j <= 1; ++j)
        { // hel0
#pragma unroll
            for (int m = 0; m < 12; ++m)
            {
                AZRe_[i][j][m] = (pp1[ip + ((i * 12 * 2 + j * 12 + m) * 2) * N_size]);
                AZIm_[i][j][m] = (pp1[ip + ((i * 12 * 2 + j * 12 + m) * 2 + 1) * N_size]);
            }
        }
    }
    for (int i = 0; i < 4; ++i)
    {
        for (int k = 0; k <= 1; ++k)
        {
#pragma unroll
            for (int m = 0; m < 12; ++m)
            {
                DZ_[i][k][m] = (pp2[ip + (i * 12 * 2 + k * 12 + m) * N_size]); // pp->DZ[i][k][m];
            }
        }
    }

    for (int iPc = 0; iPc < NZ; iPc++)
    {
        const int ir = NR + iPc;
        const int S2 = (int)_constant_paraList[ir * nset + SPIN];
        const float S = 0.5f * S2;

        int ampcount = _constant_ampcount[ir];
        const int spin_index = (S2 - 1) / 2;
        for (int k = 0; k <= 1; ++k)
        { // hel3 proton
            // there are 12 amplitudes to be added coherently
            // hel_psi = [-1, 0, +1]
            // hel_Z   = [-1/2, +1/2]
            // hel_p   = [-1/2, +1/2]

            int m = 0;
            for (float hel_psi = -1; hel_psi <= 1; hel_psi++)
            {
                for (int ihz = 0; ihz <= 1; ihz++)
                {
                    for (float hel_p = -0.5; hel_p <= 0.5; hel_p++)
                    {

                        if (fabs(hel_psi - hel_p) > S)
                        {
                            m++;
                            continue;
                        }

                        const float2 ampCoff = make_cuFloatComplex(_constant_paraList[ampcount++],
                                                                   _constant_paraList[ampcount++]);
                        if (cuCabsf(ampCoff) < 1e-10f)
                        {
                            m++;
                            continue;
                        }

#pragma unroll
                        for (int i = 0; i <= 1; ++i)
                        { // delmu
#pragma unroll
                            for (int j = 0; j <= 1; ++j)
                            { // hel0
                                const float2 Atmp2 = DZ_[spin_index][k][m] * make_cuFloatComplex(AZRe_[i][j][m], AZIm_[i][j][m]);
                                HZ[iPc][4 * i + 2 * j + k] = HZ[iPc][4 * i + 2 * j + k] + Atmp2 * ampCoff;
                            }
                        }
                        m++;
                    }
                }
            }
        }
    }

    const float mjpsip(pp[ip + N_size]);
    float mres = _mres0 + mjpsip * (_mres1 + mjpsip * _mres2);
    float2 BWPc_tmp[NZ];

#ifdef RESPLUS
    mres = mres * 1.1;
#endif
#ifdef RESMINUS
    mres = mres * 0.9;
#endif

#ifdef SMARTBINNING
    const float weight_smear = 1. / NBINS;
#else
    const float smear_binwidth = mres * _smear_nsigmas * 2 / NBINS;
#endif

    for (int smear_ibin = 0; smear_ibin < NBINS; smear_ibin++)
    {

#ifdef SMARTBINNING
        const float mjpsip_smear = binning_scheme[smear_ibin] * mres;
#else
        const float mjpsip_smear = -_smear_nsigmas * mres + smear_binwidth * smear_ibin + smear_binwidth * 0.5f;
        const float weight_smear = gauss_res(mjpsip_smear, mres) * smear_binwidth;
#endif

        float2 AKM[4][Constant::max_ikmcount];
        AKM[0][0] = make_cuFloatComplex(0.0f, 0.0f);
        AKM[1][0] = make_cuFloatComplex(0.0f, 0.0f);
        AKM[2][0] = make_cuFloatComplex(0.0f, 0.0f);
        AKM[3][0] = make_cuFloatComplex(0.0f, 0.0f);

        for (int iPc = 0; iPc < NZ; iPc++)
        {
            const int ir = NR + iPc;
            float m0 = _constant_paraList[ir * nset + MASS];
            const float g0 = _constant_paraList[ir * nset + WIDTH];
            const int S2 = (int)_constant_paraList[ir * nset + SPIN];
            const int LR = (int)_constant_paraList[ir * nset + LRESON];
            const float S = 0.5f * S2;
            const int type = (int)_constant_paraList[ir * nset + TYPE];

            float2 BW;
            switch (type)
            {
            case MODELINDEPENDENT:
            {
                BW = ModelIndependentWave(&_constant_paraList[_constant_splcount[ir]], mjpsip + mjpsip_smear);
                m0 = -0.5f;
                break;
            }

            case BREITWIGNER:
            {
                BW = BW_AMPZ(m0, g0, mjpsip + mjpsip_smear, LR, _FFR);
                break;
            }

            case REVISEDKMATRIX:
            {
                BW = Revised_KMAMP(_constant_paraList, mjpsip + mjpsip_smear, ir, NR, NR + NZ, 1, _FFR);
                break;
            }

            case TRIANGLE:
            {
                BW = GetCusp_AMP(m0, g0, mjpsip + mjpsip_smear);
                break;
            }

            case KMATRIX:
            {
                const int ik = (LR > 0)
                                   ? (S2 == 3) ? 3 : 2
                               : (S2 == 3) ? 1
                                           : 0;

                if (cuCabsf(AKM[ik][0]) < 1e-10f)
                {
                    KMatrix(mjpsip + mjpsip_smear,
                            _constant_paraList,
                            S2,
                            (LR > 0) ? 1 : -1,
                            NR,
                            NR + NZ,
                            kmcount,
                            ik,
                            AKM[ik],
                            _FFB);
                }
                const int ikmcount = _constant_Pc_ikmcount[iPc];
                BW = AKM[ik][ikmcount];
                break;
            }
            case NONRESONANT:
            {
                BW = {1.0f, 0.0f};
                break;
            }
            }
            const float Bterm = BTermZ(S, mjpsip + mjpsip_smear, m0, _FFB) * weight_smear;
            BW = Bterm * BW;
            BWPc_tmp[iPc] = BW;
            BW1Pc[iPc] = BW + BW1Pc[iPc];
        }

        const float unscale = 1.f / weight_smear;
        for (int iPc = 0; iPc < NZ; iPc++)
        {
            const float2 iBW = BWPc_tmp[iPc];
            for (int jPc = 0; jPc < NZ; jPc++)
            {
                const float2 jBW = unscale * BWPc_tmp[jPc];
                BW2PcRR[iPc][jPc] += iBW.x * jBW.x; // weight involved in iBW, float counted.
                BW2PcII[iPc][jPc] += iBW.y * jBW.y;
                BW2PcIR[iPc][jPc] += iBW.y * jBW.x;
                BW2PcRI[iPc][jPc] += iBW.x * jBW.y;
            }
        }
    }

    SUMTYPE H_total(0.f);
    for (int k = 0; k <= 1; ++k)
    {
        for (int i = 0; i <= 1; ++i)
        {
            for (int j = 0; j <= 1; ++j)
            {
                H_total += magnitude2(H_L_Zcs[4 * i + 2 * j + k]);
                // Inteference between Pc and L* (and maybe Zcs)
                for (int iPc = 0; iPc < NZ; iPc++)
                {
                    // Do the convolution on BW1Pc_smear
                    const CSUMTYPE Amp1Pc = BW1Pc[iPc] * HZ[iPc][4 * i + 2 * j + k];
                    const CSUMTYPE Amp1L = H_L_Zcs[4 * i + 2 * j + k];
                    H_total += 2 * (Amp1Pc.x * Amp1L.x + Amp1Pc.y * Amp1L.y);
                    for (int jPc = 0; jPc < NZ; jPc++)
                    {
                        // Do the convolution on BW2Pc_smear
                        H_total += (BW2PcRR[iPc][jPc] + BW2PcII[iPc][jPc]) * (HZ[iPc][4 * i + 2 * j + k].x * HZ[jPc][4 * i + 2 * j + k].x + HZ[iPc][4 * i + 2 * j + k].y * HZ[jPc][4 * i + 2 * j + k].y) + (BW2PcIR[iPc][jPc] - BW2PcRI[iPc][jPc]) * (HZ[iPc][4 * i + 2 * j + k].x * HZ[jPc][4 * i + 2 * j + k].y - HZ[iPc][4 * i + 2 * j + k].y * HZ[jPc][4 * i + 2 * j + k].x);
                    }
                }
            }
        }
    }
    return H_total;
}

#define GetAmp2sSMR TemplateGetAmp2s<Constant::number_of_Lz, Constant::number_of_Pc, Constant::number_of_Zcs, SMEAR_NBINS, _constant_smear_binning_scheme, float2, float>
#define GetAmp2sSMR_finebinning TemplateGetAmp2s<Constant::number_of_Lz, Constant::number_of_Pc, Constant::number_of_Zcs, SMEAR_FINE_NBINS, _constant_smear_fine_binning_scheme, float2, float>
#define GetAmp2sSMR_badbinning TemplateGetAmp2s<Constant::number_of_Lz, Constant::number_of_Pc, Constant::number_of_Zcs, SMEAR_BAD_NBINS, _constant_smear_bad_binning_scheme, float2, float>
#define GetAmp2s_NoSMR GetAmp2s<Constant::number_of_Lz, Constant::number_of_Pc, Constant::number_of_Zcs, true, float2, float>
#define GetAmp2s_NoSMR_NoInterference GetAmp2s<Constant::number_of_Lz, Constant::number_of_Pc, Constant::number_of_Zcs, false, float2, float>

__global__ void kernel_store_fx_SMR(const float *float_pp, const float *float_pp1, const float *float_pp2, const float *float_pp3, const float *float_pp4, int para_size,
                                    int vamp_size, int paraSpline_size, int paraKM_size, double *d_fx, int end,
                                    int begin, int NUMLz, int NUMPc, int NUMZcs, int N_size, int index_flag, float _FFR, float _FFB)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if (index < end - begin && index >= 0)
    {
        index = (index_flag == 1) ? index : index + begin;

        d_fx[index] = GetAmp2sSMR(index,
                                  float_pp,
                                  float_pp1,
                                  float_pp2,
                                  float_pp3,
                                  float_pp4,
                                  N_size);

        if (d_fx[index] < 0)
            printf("\n WSM m:%f, fx[%d]:%f\n", float_pp[index + N_size], index, d_fx[index]);
    }
}

__global__ void kernel_store_fx_SMR_finebinning(const float *float_pp, const float *float_pp1, const float *float_pp2, const float *float_pp3, const float *float_pp4, int para_size,
                                                int vamp_size, int paraSpline_size, int paraKM_size, double *d_fx, int end,
                                                int begin, int NUMLz, int NUMPc, int NUMZcs, int N_size, int index_flag, float _FFR, float _FFB)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if (index < end - begin && index >= 0)
    {
        index = (index_flag == 1) ? index : index + begin;

        d_fx[index] = GetAmp2sSMR_finebinning(index,
                                              float_pp,
                                              float_pp1,
                                              float_pp2,
                                              float_pp3,
                                              float_pp4,
                                              N_size);

        if (d_fx[index] < 0)
            printf("\n WSM m:%f, fx[%d]:%f\n", float_pp[index + N_size], index, d_fx[index]);
    }
}

__global__ void kernel_store_fx_normalisation(const float *float_pp, const float *float_pp1, const float *float_pp2, const float *float_pp3, const float *float_pp4, int para_size,
                                              int vamp_size, int paraSpline_size, int paraKM_size, double *d_fx, double *d_fx_MC_noInteference, int end,
                                              int begin, int NUMLz, int NUMPc, int NUMZcs, int N_size, int index_flag, float _FFR, float _FFB)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if (index < end - begin && index >= 0)
    {
        index = (index_flag == 1) ? index : index + begin;
        float mjpsip(float_pp[index + N_size]);
        double d_fx_temp;
        //        if (0)
        if ((mjpsip > 4.4 && mjpsip < 4.48) || (mjpsip > 4.27 && mjpsip < 4.34))
        {
            d_fx_temp = GetAmp2sSMR_badbinning(index,
                                               float_pp,
                                               float_pp1,
                                               float_pp2,
                                               float_pp3,
                                               float_pp4,
                                               N_size);
        }
        else
        {
            d_fx_temp = GetAmp2s_NoSMR(index,
                                       float_pp,
                                       float_pp1,
                                       float_pp2,
                                       float_pp3,
                                       float_pp4,
                                       N_size);
        }
        if (float_pp[index + 2 * N_size] <= -1000.f)
        {
            d_fx[index] = d_fx_temp;
        }
        else
        {
            // Multiply amp2 of normalisation with its per-event weight
            d_fx[index] = d_fx_temp * float_pp[index + 2 * N_size];
        }
        // mjpsip
        if (d_fx_temp < 0)
            printf("\n NSM m:%f, fx[%d]:%f\n", float_pp[index + N_size], index, d_fx_temp);
        // printf("index: %d\n", index+Nda_SMR);
    }
    if (float_pp[index + 2 * N_size] <= -1000.f)
    {
        d_fx_MC_noInteference[index] = 0.;
    }
    else
    {
        d_fx_MC_noInteference[index] = GetAmp2s_NoSMR_NoInterference(index, float_pp, float_pp1, float_pp2, float_pp3, float_pp4, N_size) * float_pp[index + 2 * N_size];
    }
    if (d_fx_MC_noInteference[index] < 0)
        printf("\n NSM m:%f, fx[%d]:%f\n", float_pp[index + N_size], index, d_fx_MC_noInteference[index]);
}

void fake_kernel_store_fx(const float *float_pp, const float *float_pp1, const float *float_pp2, const float *float_pp3, const float *float_pp4, int para_size,
                          int vamp_size, int paraSpline_size, int paraKM_size, double *d_fx, double *d_fx_MC_noInteference, int end,
                          int begin, int NUMLz, int NUMPc, int NUMZcs, int Nda_SMR, int Nda_SMR_finebinning, int N_size, float _FFR, float _FFB)
{
    //        printf("Nda_SMR = %d, Nda_SMR_finebinning = %d, begin=%d, end=%d \n",Nda_SMR, Nda_SMR_finebinning, begin, end);
    // if(begin>=Nda_SMR) break;
    // printf("hhhhhhh NDA:%d,begin%d\n",Nda_SMR,begin);
    int index_flag = 1;
    if (begin < Nda_SMR)
    {
        // printf("hhhhhhhhhhhhhhhhhhh!!!!1\n");
        int N_thread = min(end, Nda_SMR) - begin;
        int threadsPerBlock = BLOCK_SIZE;
        int blocksPerGrid = (N_thread + threadsPerBlock - 2) / threadsPerBlock;
        kernel_store_fx_SMR<<<blocksPerGrid, threadsPerBlock>>>(float_pp, float_pp1, float_pp2, float_pp3, float_pp4, para_size, vamp_size, paraSpline_size, paraKM_size, d_fx, N_thread + begin, begin, NUMLz, NUMPc, NUMZcs, N_size, index_flag, _FFR, _FFB);
        //        int blocksPerGrid1 = (Nda_SMR + threadsPerBlock - 1) / threadsPerBlock;
        //        kernel_store_fx_SMR<<<blocksPerGrid1, threadsPerBlock>>>(float_pp, float_pp1, float_pp2, float_pp3, float_pp4, d_paraList,para_size,vamp_size, paraSpline_size, paraKM_size, d_fx,Nda_SMR+begin,begin, NUMLz, NUMPc, NUMZcs, N_size, index_flag, _FFR, _FFB, false);
        index_flag = 0;
        //        int blocksPerGrid2 = (N_thread - Nda_SMR + threadsPerBlock - 1) / threadsPerBlock;
        //        kernel_store_fx_SMR<<<blocksPerGrid2, threadsPerBlock>>>(float_pp, float_pp1, float_pp2, float_pp3, float_pp4, d_paraList,para_size,vamp_size, paraSpline_size, paraKM_size, d_fx,N_thread+begin,Nda_SMR+begin, NUMLz, NUMPc, NUMZcs, N_size, index_flag, _FFR, _FFB, true);
        //        printf("begin = %d, N_thread/2+begin = %d, N_thread+begin = %d, Nda_SMR = %d \n", begin,  N_thread/2+begin, N_thread+begin, Nda_SMR);
    }
    if (end > Nda_SMR && begin < Nda_SMR + Nda_SMR_finebinning)
    {
        int N_thread = min(end, Nda_SMR + Nda_SMR_finebinning) - max(begin, Nda_SMR);
        int threadsPerBlock = BLOCK_SIZE;
        int blocksPerGrid = (N_thread + threadsPerBlock - 2) / threadsPerBlock;
        //        printf("index_flag = %d \n", index_flag);
        kernel_store_fx_SMR_finebinning<<<blocksPerGrid, threadsPerBlock>>>(float_pp, float_pp1, float_pp2, float_pp3, float_pp4, para_size, vamp_size, paraSpline_size, paraKM_size, d_fx, N_thread + max(begin, Nda_SMR), max(begin, Nda_SMR), NUMLz, NUMPc, NUMZcs, N_size, index_flag, _FFR, _FFB);
        index_flag = 0;
    }

    if (end > Nda_SMR + Nda_SMR_finebinning)
    {
        // printf("hhhhhhhhhhhhhhhhhhh!!!!2\n");
        int N_thread = end - max(Nda_SMR + Nda_SMR_finebinning, begin);
        // printf("hhhhhhhhhh, %d, %d\n", end, N_thread);
        int threadsPerBlock = BLOCK_SIZE;
        int blocksPerGrid = (N_thread + threadsPerBlock - 1) / threadsPerBlock;
        kernel_store_fx_normalisation<<<blocksPerGrid, threadsPerBlock>>>(float_pp, float_pp1, float_pp2, float_pp3, float_pp4, para_size, vamp_size, paraSpline_size, paraKM_size, d_fx, d_fx_MC_noInteference, end, end - N_thread, NUMLz, NUMPc, NUMZcs, N_size, index_flag, _FFR, _FFB);
    }
}

// begin of chenchen
__device__ double GetNll_Bp(double fx, double sw)
{
    return (fx > 0) ? log(fx) * sw : -1e12;
}

__global__ void kernel_store_nll_Bp(const double *fx, const float *sw, double *d_nll, int end, int begin)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if (index < end - begin && index >= 0)
    {
        d_nll[index] = GetNll_Bp(fx[index], sw[index]);
    }
}

void cuda_configure_Pc_KMatrix(const float *paraList)
{
    std::vector<int> Pc_Kmatrix_indices;

    int ikmcount[] = {0, 0, 0, 0};

    size_t start = Constant::number_of_Lz;
    size_t end = Constant::number_of_Lz + Constant::number_of_Pc;

    for (size_t ir = start; ir < end; ir++)
    {
        int S2(paraList[ir * nset + SPIN]);
        int LR(paraList[ir * nset + 3]);
        int type(paraList[ir * nset + TYPE]);

        if (type == KMATRIX)
        {
            int ik = (LR > 0)
                         ? (S2 == 3) ? 3 : 2
                     : (S2 == 3) ? 1
                                 : 0;

            Pc_Kmatrix_indices.push_back(ikmcount[ik]);
            ikmcount[ik]++;
        }
    }

    for (size_t i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_Pc_ikmcount, Pc_Kmatrix_indices.data(), Constant::number_of_Pc * sizeof(int)));
    }
}

// This function iterates over all defined resonances to figure out where each resonance start to look for helicity amplitude
// Ampcount is reliant on resonance ordering and helicity loop, so this is configured beforehand
void cuda_configure_ampcount(const float *paraList, const int paraSize)
{
    // ampcount begins after paraSize
    int ampcount = paraSize;

    std::vector<int> ampcount_indices;
    for (int iLz = 0; iLz < Constant::number_of_Lz; iLz++)
    {
        int ir = iLz;
        int S2(paraList[ir * nset + SPIN]);
        float S(S2 / 2.f);
        ampcount_indices.push_back(ampcount);
        for (int k = 0; k <= 1; ++k)
        { // hel3 proton
            for (int m = 0; m < 6; ++m)
            {
                if (fabs(S - 0.5f) < 1e-10f && m >= 4)
                {
                    continue;
                }
                ampcount += 2;
            }
        }
    }

    for (int iPc = 0; iPc < Constant::number_of_Pc; iPc++)
    {
        int ir = Constant::number_of_Lz + iPc;
        int S2(paraList[ir * nset + SPIN]);
        float S(S2 / 2.f);
        ampcount_indices.push_back(ampcount);
        for (int k = 0; k <= 1; ++k)
        { // hel3 proton
            // there are 12 amplitudes to be added coherently //helpsi,0,+/-1, helZ,+/-1/2, help=+/-1/2
            for (int helpsi = -1; helpsi <= 1; ++helpsi)
            {
                for (int ihz = 0; ihz <= 1; ++ihz)
                {
                    //   helZ = (ihz==0) ? -0.5: 0.5;
                    for (int ihp = 0; ihp <= 1; ++ihp)
                    {
                        float help = (ihp == 0) ? -0.5f : 0.5f;
                        if (fabsf((float)helpsi - help) > S)
                            continue;
                        ampcount += 2;
                    }
                }
            }
        }
    }

    for (int iZcs = 0; iZcs < Constant::number_of_Zcs; iZcs++)
    {
        int ir = Constant::number_of_Lz + Constant::number_of_Pc + iZcs;
        int S2(paraList[ir * nset + SPIN]);
        float S(S2 / 2.f);
        ampcount_indices.push_back(ampcount);
        for (int k = 0; k <= 1; ++k)
        { // hel3 proton
            // there are 12 amplitudes to be added coherently //helpsi,0,+/-1, helZ,+/-1/2, help=+/-1/2
            for (int helpsi = -1; helpsi <= 1; ++helpsi)
            {
                if (abs(helpsi) > S)
                    continue;
                for (int ihp = 0; ihp <= 1; ++ihp)
                {
                    float help = (ihp == 0) ? -0.5 : 0.5;
                    for (int ihz = 0; ihz <= 1; ++ihz)
                    {
                        float helZ = (ihz == 0) ? help - 0.5 : help + 0.5;
                        if (fabs(helZ) > S)
                            continue;
                        ampcount += 2;
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_ampcount, ampcount_indices.data(), Constant::number_of_resonances * sizeof(int)));
    }
}

void cuda_configure_splcount(const float *h_paraList,
                             const int para_size,
                             const int vamp_size,
                             const int NR,
                             const int NZ)
{
    int splcount = para_size + vamp_size;
    std::vector<int> splcount_array;
    splcount_array.reserve(NR + NZ);

    for (int iLz = 0; iLz < NR; iLz++)
    {
        const int ir = iLz;
        const int type = (int)h_paraList[ir * nset + TYPE];

        splcount_array.push_back(splcount);
        if (type == MODELINDEPENDENT)
        {
            splcount += 9 * NUMSP;
        }
    }

    for (int iPc = 0; iPc < NZ; iPc++)
    {
        const int ir = NR + iPc;
        const int type = (int)h_paraList[ir * nset + TYPE];

        splcount_array.push_back(splcount);
        if (type == MODELINDEPENDENT)
        {
            splcount += 9 * NUMSPZ;
        }
    }

    // Probably don't need for mjpsik resonances

    for (size_t i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_splcount, splcount_array.data(), (NR + NZ) * sizeof(int)));
    }
}

void cuda_configure_smear()
{
    for (size_t i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_smear_binning_scheme, _smear_binning_scheme, SMEAR_NBINS * sizeof(float)));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_smear_fine_binning_scheme, _smear_fine_binning_scheme, SMEAR_FINE_NBINS * sizeof(float)));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_smear_bad_binning_scheme, _smear_bad_binning_scheme, SMEAR_BAD_NBINS * sizeof(float)));
    }
}

void cuda_configure_paraList(const float *h_paraList, const int total_size)
{
    for (size_t i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_paraList, h_paraList, total_size * sizeof(float)));
    }
}

void cuda_configure_sizes(const int para_size,
                          const int vamp_size,
                          const int paraSpline_size)
{
    std::vector<int> sizes = {para_size, vamp_size, paraSpline_size};
    for (size_t i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_sizes, sizes.data(), 3 * sizeof(int)));
    }
}

void cuda_configure_form_factors(const float _FFR, const float _FFB)
{
    std::vector<float> _FFs = {_FFR, _FFB};
    for (size_t i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        CUDA_CALL(cudaMemcpyToSymbol(_constant_form_factors, _FFs.data(), 2 * sizeof(float)));
    }
}

// end of chenchen
double host_store_fx(std::vector<float *> d_float_pp,
                     std::vector<float *> d_float_pp1,
                     std::vector<float *> d_float_pp2,
                     std::vector<float *> d_float_pp3,
                     std::vector<float *> d_float_pp4,
                     std::vector<float *> d_float_sw,
                     float *h_paraList,
                     int para_size,
                     int vamp_size,
                     int paraSpline_size,
                     double *h_fx,
                     int end,
                     int begin,
                     int Nda,
                     int Nda_SMR,
                     int Nda_SMR_finebinning,
                     int NUMLz,
                     int NUMPc,
                     int NUMZcs,
                     double &mynll,
                     double SumW,
                     double sumW_data,
                     float _FFR,
                     float _FFB,
                     bool store_h_fx)
{
    // init Ns
    // Ns为分段数组，第i个gpu所处理的线程序号范围为:[ Ns[i] , Ns[i+1] )
    // int Ns[DEVICE_NUM+1];
    // malloc memory
    // zhihong
    ////double *d_fx[DEVICE_NUM];
    ////float *d_paraList[DEVICE_NUM];
    //  float *d_vamp[DEVICE_NUM];
    //  float *d_paraSpline[DEVICE_NUM];

    const int km_size = (Npmax + NKMC) * NKMC * 4 + ((LINEAR_KMNR) ? 4 * NKMC : 0);
    int total_para_size = para_size + vamp_size + paraSpline_size + km_size;

    //  printf("km size %i total size %i\n",(Npmax+NKMC)*NKMC*2, total_para_size);
    // 动态分配shared memory 的大小：
    // int size_paraList=total_para_size*sizeof(float);
    // int size_smear_nbins=(SMEAR_NBINS)*sizeof(float);
    // int size_smear_fine_nbins=(SMEAR_FINE_NBINS)*sizeof(float);
    // int size_smear_bad_nbins=(SMEAR_BAD_NBINS)*sizeof(float);

    cuda_configure_paraList(h_paraList, total_para_size);
    cuda_configure_form_factors(_FFR, _FFB);

    if (once_flag_config == 1)
    {
        Ns[0] = 0;
        for (int i = 1; i < DEVICE_NUM; i++)
        {
            Ns[i] = Ns[i - 1] + end / DEVICE_NUM;
        }
        Ns[DEVICE_NUM] = end;
        for (int i = 0; i < DEVICE_NUM; i++)
        {
            nvtxRangePush("malloc");
            CUDA_CALL(cudaSetDevice(i + iDevice));
            int N_thread = Ns[i + 1] - Ns[i];
            CUDA_CALL(cudaMalloc((void **)&(d_fx[i]), N_thread * sizeof(double)));
            CUDA_CALL(cudaMalloc((void **)&(d_fx_MC_noInteference[i]), N_thread * sizeof(double)));
            // CUDA_CALL(cudaMalloc((void **)&(d_paraList[i]),size_paraList));
            // CUDA_CALL(cudaMalloc((void **)&(smear_binning_scheme[i]),size_smear_nbins));
            // CUDA_CALL(cudaMalloc((void **)&(smear_fine_binning_scheme[i]),size_smear_fine_nbins));
            // CUDA_CALL(cudaMalloc((void **)&(smear_bad_binning_scheme[i]),size_smear_bad_nbins));
            //      CUDA_CALL(cudaMalloc((void **)&(d_vamp[i]),vamp_size * sizeof(float)));
            //      CUDA_CALL(cudaMalloc((void **)&(d_paraSpline[i]),paraSpline_size * sizeof(float)));
            CUDA_CALL(cudaMalloc((void **)&(d_nll[i]), N_thread * sizeof(double)));
            nvtxRangePop();
        }
        once_flag_config = 0;
        cuda_configure_sizes(para_size, vamp_size, paraSpline_size);
        cuda_configure_splcount(h_paraList, para_size, vamp_size, Constant::number_of_Lz, Constant::number_of_Pc);
    }

    // memcpy d_paraList
    //  for(int i=0;i<DEVICE_NUM;i++)
    //  {
    //      CUDA_CALL(cudaSetDevice(i+iDevice) );
    // 使用异步函数.
    //  CUDA_CALL(cudaMemcpyAsync(d_paraList[i] , h_paraList, size_paraList, cudaMemcpyHostToDevice));
    //     CUDA_CALL(cudaMemcpyAsync(smear_binning_scheme[i] , _smear_binning_scheme, size_smear_nbins, cudaMemcpyHostToDevice));
    //     CUDA_CALL(cudaMemcpyAsync(smear_fine_binning_scheme[i] , _smear_fine_binning_scheme, size_smear_fine_nbins, cudaMemcpyHostToDevice));
    //     CUDA_CALL(cudaMemcpyAsync(smear_bad_binning_scheme[i] , _smear_bad_binning_scheme, size_smear_bad_nbins, cudaMemcpyHostToDevice));
    // }

    int threadsPerBlock = BLOCK_SIZE;
    for (int i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        int N_thread = Ns[i + 1] - Ns[i];
        int N_size = (N_thread / 32 + 1) * 32;
        // int blocksPerGrid =(N_thread + threadsPerBlock - 1) / threadsPerBlock;
        nvtxRangePush("kernel");
        // kernel_store_fx<<<blocksPerGrid, threadsPerBlock>>>(d_float_pp[i], d_paraList[i],para_size,vamp_size, paraSpline_size, d_fx[i],Ns[i+1],Ns[i], NUMLz, NUMPc);
        // kernel_store_fx<<<blocksPerGrid, threadsPerBlock,size_paraList>>>(d_float_pp[i], d_paraList[i],para_size,vamp_size, paraSpline_size, (Npmax+NKMC)*NKMC*4, d_fx[i],Ns[i+1],Ns[i], NUMLz, NUMPc);
        fake_kernel_store_fx(d_float_pp[i], d_float_pp1[i], d_float_pp2[i], d_float_pp3[i], d_float_pp4[i], para_size, vamp_size, paraSpline_size, (Npmax + NKMC) * NKMC * 4, d_fx[i], d_fx_MC_noInteference[i], Ns[i + 1], Ns[i], NUMLz, NUMPc, NUMZcs, Nda_SMR, Nda_SMR_finebinning, N_size, _FFR, _FFB);
        nvtxRangePop();
    }
    /*
    for(int i=0;i<DEVICE_NUM;i++)
    {
        if(Ns[i]>=Nda) break;
        CUDA_CALL(cudaSetDevice(i+iDevice) );
        int N_thread=min(Ns[i+1],Nda)-Ns[i];
        CUDA_CALL(cudaMemcpyAsync(&h_fx[Ns[i]] , d_fx[i], N_thread * sizeof(double), cudaMemcpyDeviceToHost));
    }*/
    double sumint(0.);
    double sumint_MC_noInteference(0.);

    for (int i = 0; i < DEVICE_NUM; i++)
    {
        if (Ns[i + 1] <= Nda)
            continue; // For MC
        CUDA_CALL(cudaSetDevice(i + iDevice));
        int N_thread = Ns[i + 1] - Ns[i];
        thrust::device_ptr<double> dev_ptr(d_fx[i]);
        sumint = thrust::reduce(dev_ptr + max(Nda - Ns[i], 0), dev_ptr + N_thread, sumint, thrust::plus<double>());

        if (store_h_fx)
        {
            CUDA_CALL(cudaMemcpyAsync(&h_fx[Ns[i]], d_fx[i], N_thread * sizeof(double), cudaMemcpyDeviceToHost));
        }
    }
    for (int i = 0; i < DEVICE_NUM; i++)
    {
        if (Ns[i + 1] <= Nda)
            continue; // For MC
        CUDA_CALL(cudaSetDevice(i + iDevice));
        int N_thread = Ns[i + 1] - Ns[i];
        thrust::device_ptr<double> dev_ptr(d_fx_MC_noInteference[i]);
        sumint_MC_noInteference = thrust::reduce(dev_ptr + max(Nda - Ns[i], 0), dev_ptr + N_thread, sumint_MC_noInteference, thrust::plus<double>());
    }

    //    printf("sum device %.10f \n",sumint);

    for (int i = 0; i < DEVICE_NUM; i++)
    {
        if (Ns[i] >= Nda)
            continue;
        CUDA_CALL(cudaSetDevice(i + iDevice));
        int N_thread = min(Ns[i + 1] - Ns[i], Nda - Ns[i]);
        if (!N_thread)
            continue; // no event to process
        int blocksPerGrid = (N_thread + threadsPerBlock - 1) / threadsPerBlock;
        nvtxRangePush("kernel_store_nll_Bp");
        kernel_store_nll_Bp<<<blocksPerGrid, threadsPerBlock>>>(d_fx[i], d_float_sw[i], d_nll[i], Ns[i] + N_thread, Ns[i]);
        nvtxRangePop();
    }

    std::cout << "SUMINT:" << sumint << std::endl;

    // nll sum of data
    double inttt = sumint / SumW;
    double nll_shift = NLL_SHIFT;
    double nll_mc = sumW_data * log(inttt);
    nll_shift += nll_mc;
    double nll_data = 0;
    for (int i = 0; i < DEVICE_NUM; i++)
    {
        if (Ns[i] >= Nda)
            continue;
        CUDA_CALL(cudaSetDevice(i + iDevice));
        int N_thread = min(Ns[i + 1] - Ns[i], Nda - Ns[i]);
        thrust::device_ptr<double> dev_ptr(d_nll[i]);
        nvtxRangePush("log sum of data");
        nll_data = thrust::reduce(dev_ptr, dev_ptr + N_thread, nll_data, thrust::plus<double>());
        nvtxRangePop();
    }
    mynll = nll_shift - nll_data;

    if (fabs(sumint_MC_noInteference / sumint - 1.f) > 0.3f)
        mynll = mynll + 1000.f;
    return sumint;
}

__global__ void kernel_store_amp2s(const float *float_pp, const float *float_pp1, const float *float_pp2, const float *float_pp3, const float *float_pp4, int para_size, int vamp_size, int paraSpline_size, int paraKM_size, double *d_amp2s, int N_thread, int NUMLz, int NUMPc, int NUMZcs, int N_size, float _FFR, float _FFB)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if (index >= 0 && index < N_thread)
    {
        double amp2s = GetAmp2s_NoSMR(index, float_pp, float_pp1, float_pp2, float_pp3, float_pp4, N_size);
        d_amp2s[index] = amp2s;
    }
}

// Store per-event amp^2 for hit-and-reject toys
void host_store_amp2s(std::vector<float *> d_float_pp, std::vector<float *> d_float_pp1, std::vector<float *> d_float_pp2, std::vector<float *> d_float_pp3, std::vector<float *> d_float_pp4, float *h_paraList, int para_size, int vamp_size, int paraSpline_size, double *h_amp2s, int end, int begin, int NUMLz, int NUMPc, int NUMZcs, float _FFR, float _FFB)
{
    const int km_size = (Npmax + NKMC) * NKMC * 4 + ((LINEAR_KMNR) ? 4 * NKMC : 0);
    int total_para_size = para_size + vamp_size + paraSpline_size + km_size;
    cuda_configure_paraList(h_paraList, total_para_size);

    double *d_amp2s[DEVICE_NUM];

    Ns[0] = begin;
    for (int i = 1; i < DEVICE_NUM; i++)
    {
        Ns[i] = Ns[i - 1] + end / DEVICE_NUM;
    }
    Ns[DEVICE_NUM] = end;

    for (int i = 0; i < DEVICE_NUM; i++)
    {
        nvtxRangePush("malloc");
        CUDA_CALL(cudaSetDevice(i + iDevice));
        int N_thread = Ns[i + 1] - Ns[i];
        CUDA_CALL(cudaMalloc((void **)&(d_amp2s[i]), N_thread * sizeof(double)));
        nvtxRangePop();
    }

    for (int i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        int N_thread = Ns[i + 1] - Ns[i];
        int N_size = (N_thread / 32 + 1) * 32;

        // Calculate per-event amp2s and store it to host array
        nvtxRangePush("kernel");
        int threadsPerBlock = BLOCK_SIZE;
        int blocksPerGrid = int(ceil(float(N_thread) / threadsPerBlock));
        kernel_store_amp2s<<<blocksPerGrid, threadsPerBlock>>>(d_float_pp[i], d_float_pp1[i], d_float_pp2[i], d_float_pp3[i], d_float_pp4[i], para_size, vamp_size, paraSpline_size, (Npmax + NKMC) * NKMC * 4, d_amp2s[i], N_thread, NUMLz, NUMPc, NUMZcs, N_size, _FFR, _FFB);
        CUDA_CALL(cudaMemcpyAsync(&h_amp2s[Ns[i]], d_amp2s[i], N_thread * sizeof(double), cudaMemcpyDeviceToHost));
        nvtxRangePop();
    }

    for (int i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaFree(d_amp2s[i]));
    }
}

/*
//在gpu中为pwa_paras开辟空间
void cu_malloc_h_pp(float *h_float_pp,float *&d_float_pp,int length,int device)
{
    //Ns为分段数组，第i个gpu所处理的线程序号范围为:[ Ns[i] , Ns[i+1] )
    int Ns[DEVICE_NUM+1];
    Ns[0]=0;
    for(int i=1;i<DEVICE_NUM;i++)
    {
        Ns[i]=Ns[i-1]+length/DEVICE_NUM;
    }
    Ns[DEVICE_NUM]=length;
    int deviceCount;
    CUDA_CALL( cudaGetDevice(&deviceCount) );
    printf("Num GPU %i %i\n",deviceCount, device);
    CUDA_CALL( cudaSetDevice(device+iDevice) );
    //  CUDA_CALL( cudaDeviceSynchronize() );
    //  CUDA_CALL( cudaThreadSynchronize() );
    int N_size = Ns[device+1]-Ns[device];
    size_t mem_size = (int(sizeof(cu_Jpsihh_dlz)/32)+1)*32;// * N_size;
    int array_size = (int(sizeof(cu_Jpsihh_dlz)/32)+1)*32/sizeof(float);
    printf("size to locate %d %d\n", mem_size, N_size);
    CUDA_CALL(cudaMalloc((void **)&d_float_pp, mem_size * N_size));
    CUDA_CALL(cudaMemcpy(d_float_pp , &h_float_pp[array_size*Ns[device]], mem_size * N_size, cudaMemcpyHostToDevice));
}*/

void cu_malloc_h_pp(float *h_float_pp, float *&d_float_pp, int mem_length, int device)
{
    // int deviceCount;
    // CUDA_CALL( cudaGetDevice(&deviceCount) );
    // printf("Num GPU %i %i\n",deviceCount, device);
    CUDA_CALL(cudaSetDevice(device + iDevice));
    printf("size to locate device: %d\n", device);

    CUDA_CALL(cudaMalloc((void **)&d_float_pp, mem_length));
    CUDA_CALL(cudaMemcpy(d_float_pp, &h_float_pp[0], mem_length, cudaMemcpyHostToDevice));
}

void cu_Free_pp(std::vector<float *> d_float_pp, std::vector<float *> d_float_pp1, std::vector<float *> d_float_pp2, std::vector<float *> d_float_pp3, std::vector<float *> d_float_pp4)
{
    if (d_float_pp.size() != DEVICE_NUM)
        return;
    if (d_float_pp1.size() != DEVICE_NUM)
        return;
    if (d_float_pp2.size() != DEVICE_NUM)
        return;
    if (d_float_pp3.size() != DEVICE_NUM)
        return;
    if (d_float_pp4.size() != DEVICE_NUM)
        return;
    for (int i = 0; i < DEVICE_NUM; i++)
    {
        CUDA_CALL(cudaSetDevice(i + iDevice));
        if (d_float_pp[i])
            CUDA_CALL(cudaFree(d_float_pp[i]));
        if (d_float_pp1[i])
            CUDA_CALL(cudaFree(d_float_pp1[i]));
        if (d_float_pp2[i])
            CUDA_CALL(cudaFree(d_float_pp2[i]));
        if (d_float_pp3[i])
            CUDA_CALL(cudaFree(d_float_pp3[i]));
        if (d_float_pp4[i])
            CUDA_CALL(cudaFree(d_float_pp4[i]));

        // once_flag_config is true when these exists
        if (!(once_flag_config))
        {
            CUDA_CALL(cudaFree(d_fx[i]));
            CUDA_CALL(cudaFree(d_nll[i]));
        }
    }
    once_flag_config = 1;
}

__device__ float2 GetLineshapeLsKM(const float mkp, const int NR, const float _FFR, const int iLz) //, const size_t * R_indices, const size_t * Z_indices, const size_t * X_indices)
                                                                                                   ////return square of complex amplitude
{
    float m0 = _constant_paraList[iLz * nset + MASS];
    int S2 = (int)(_constant_paraList[iLz * nset + SPIN]);
    float S = (float)S2 / 2.f;
    float2 BW = Revised_KMAMP_refactor(_constant_paraList, mkp, iLz, 0, NR, 0, _FFR);

    float Bterm = BTerm(S, mkp, m0, _FFR);
    BW = cuCmulfc(Bterm, BW);

    return BW;
}

__device__ float2 GetLineshapeLsBW(const float mkp, const float _FFR, const int iLz) //, const size_t * R_indices, const size_t * Z_indices, const size_t * X_indices)
                                                                                     ////return square of complex amplitude
{
    float m0 = _constant_paraList[iLz * nset + MASS];
    float g0 = _constant_paraList[iLz * nset + WIDTH];
    int S2 = (int)(_constant_paraList[iLz * nset + SPIN]);
    int LR = (int)(_constant_paraList[iLz * nset + LRESON]);
    float S = (float)S2 / 2.f;
    float2 BW = BW_AMP(m0, g0, mkp, LR, _FFR);

    float Bterm = BTerm(S, mkp, m0, _FFR);
    BW = cuCmulfc(Bterm, BW);

    return BW;
}

__global__ void storeLineshapeLsKM(const float *mkp_array, float *KM_real, float *KM_imag, float *BW_real, float *BW_imag, const int n_points, const int NR, const float _FFR)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    const float mkp = mkp_array[index];
    for (int iLz = 0; iLz < NR; iLz++)
    {
        float2 KM_amp = GetLineshapeLsKM(mkp, NR, _FFR, iLz);
        KM_real[n_points * iLz + index] = KM_amp.x;
        KM_imag[n_points * iLz + index] = KM_amp.y;
    }

    for (int iLz = 0; iLz < NR; iLz++)
    {
        float2 BW_amp = GetLineshapeLsBW(mkp, _FFR, iLz);
        BW_real[n_points * iLz + index] = BW_amp.x;
        BW_imag[n_points * iLz + index] = BW_amp.y;
    }
}

// Store per-event amp^2 for hit-and-reject toys
void host_store_lineshapeLsKM(const float *h_mkp, float *h_KM_real, float *h_KM_imag, float *h_BW_real, float *h_BW_imag, const int n_points, const int NR, const int nLz, const float _FFR, float *h_paraList, const int para_size, const int vamp_size, const int paraSpline_size)
{
    int total_para_size = para_size + vamp_size + paraSpline_size + (Npmax + NKMC) * NKMC * 4;
    cuda_configure_paraList(h_paraList, total_para_size);

    float *d_mkp;
    float *d_KM_real;
    float *d_KM_imag;
    float *d_BW_real;
    float *d_BW_imag;
    nvtxRangePush("malloc");
    CUDA_CALL(cudaSetDevice(iDevice));
    CUDA_CALL(cudaMalloc((void **)(&d_mkp), n_points * sizeof(float)));
    CUDA_CALL(cudaMalloc((void **)(&d_KM_real), n_points * nLz * sizeof(float)));
    CUDA_CALL(cudaMalloc((void **)(&d_KM_imag), n_points * nLz * sizeof(float)));
    CUDA_CALL(cudaMalloc((void **)(&d_BW_real), n_points * nLz * sizeof(float)));
    CUDA_CALL(cudaMalloc((void **)(&d_BW_imag), n_points * nLz * sizeof(float)));
    nvtxRangePop();

    nvtxRangePush("hostToDevice");
    CUDA_CALL(cudaMemcpy(d_mkp, h_mkp, n_points * sizeof(float), cudaMemcpyHostToDevice));
    nvtxRangePop();

    nvtxRangePush("deviceToHost");
    CUDA_CALL(cudaSetDevice(iDevice));
    int threadsPerBlock = 32;
    int blocksPerGrid = int(ceil(float(n_points) / threadsPerBlock));
    storeLineshapeLsKM<<<blocksPerGrid, threadsPerBlock>>>(d_mkp, d_KM_real, d_KM_imag, d_BW_real, d_BW_imag, n_points, NR, _FFR);
    CUDA_CALL(cudaMemcpy(h_KM_real, d_KM_real, n_points * nLz * sizeof(float), cudaMemcpyDeviceToHost));
    CUDA_CALL(cudaMemcpy(h_KM_imag, d_KM_imag, n_points * nLz * sizeof(float), cudaMemcpyDeviceToHost));
    CUDA_CALL(cudaMemcpy(h_BW_real, d_BW_real, n_points * nLz * sizeof(float), cudaMemcpyDeviceToHost));
    CUDA_CALL(cudaMemcpy(h_BW_imag, d_BW_imag, n_points * nLz * sizeof(float), cudaMemcpyDeviceToHost));
    nvtxRangePop();

    CUDA_CALL(cudaFree(d_mkp));
    CUDA_CALL(cudaFree(d_KM_real));
    CUDA_CALL(cudaFree(d_KM_imag));
    CUDA_CALL(cudaFree(d_BW_real));
    CUDA_CALL(cudaFree(d_BW_imag));
}
