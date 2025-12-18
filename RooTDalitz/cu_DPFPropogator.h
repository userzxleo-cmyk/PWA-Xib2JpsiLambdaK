#ifndef CU_DPF_PROPOGATOR_HH
#define CU_DPF_PROPOGATOR_HH
//#define NUMSP 8//44

#include <cuda_runtime.h>

__device__ float gauss_res(float delta, float sigma);
__device__ float getp(float M, float m1, float m2);
__device__ float2 getrho(float M2, float m1, float m2, int L, float _FFB);
__device__ float2 getrho_complexMass(float M2, float2 m1, float2 m2, int L, float _FFB);
__device__ float getFBW2(float M2, float m1, float m2, int L, float _FFB);
__device__ float2 getFBW2_complexMass(float M2, float2 m1, float2 m2, int L, float _FFB);
__device__ float F_BW(float r, int L, float p, float p0);
__device__ float BTerm(float Spin, float mpp, float mr, float _FFB);
__device__ float BTermZ(float Spin, float mpp, float mr, float _FFB);
__device__ float2 breit_wigner(float m, float gamma, float e2);
__device__ float2 BW_AMP(float m_r, float gamma_r, float mpp, int LR, float _FFR);
__device__ float2 BW_AMPZ(float m_r, float gamma_r, float mpp, int LR, float _FFR);
__device__ float2 BW_AMPZSMR(float m_r, float gamma_r, float mpp, int LR, float _FFR);
__device__ float2 ModelIndependentWave(const float *ampxxx, float x);
__device__ float2 ModelIndependentWave3(const float *ampxxx, float x);
__device__ void KMatrix(float mpp, const float *paraList, int, int, int irst, int ired, int, int, float2 *Mtr, float _FFB);
__device__ float2 Revised_KMAMP(const float *paraList, float mpp, int nr, int irst, int ired, bool isPcChain, float _FFR);
__device__ float2 Revised_KMAMP_refactor(const float * paraList, float mpp, int nr, int irst, int ired, bool isPcChain, float _FFR);
__device__ float KMatrixTerm(float m_r, float gamma_r, float mpp, int LR, float &fsc, float _FFR);
__device__ float2 get_b(float2 alpha, float2 p23);
__device__ float2 get_c(float2 alpha, float2 p23, float m_r, float gamma);
__device__ float2 this_func(float alpha1, float p23_real, float m_r, float gamma);
__device__ float2 get_cusp_ampli(float m, float gamma, float mpp);
__device__ float2 GetCusp_AMP(float m_r, float gamma_r, float mpp);
__device__ float BTermX(float Spin, float mpp, float mr, float _FFB);
__device__ float2 BW_AMPX(float m_r, float gamma_r, float mpp, int LR, float _FFR);
#endif
