/*************************************************************************
  > File Name: kernel_calEva.h
  > Author:
  > Mail:
  > Created Time: 2017年03月21日 星期二 18时20分53秒
 ************************************************************************/

#ifndef _KERNEL_CALEVA_H
#define _KERNEL_CALEVA_H
#include <vector>

// double *d_float_pp=NULL;
// #include "cu_PWA_PARAS.h"
// int initialize_data(std::vector<PWA_PARAS>&, DataPointers&); // 把vector数据和指针数据对应起来，并copy到gpu里面去
// int data_distribution(DataPointers&, CudaDataPointers&); // 把vector数据和指针数据对应起来，并copy到gpu里面去
// double calEva(const PWA_PARAS &pp, int idp);
// double kernel_calEva(const PWA_PARAS &pp,int idp);
// void func(DataPointers& cpu_data_pointers);
void cu_malloc_h_pp(float *, float *&, int, int);
void cu_Free_pp(std::vector<float *> d_float_pp, std::vector<float *> d_float_pp1, std::vector<float *> d_float_pp2, std::vector<float *> d_float_pp3, std::vector<float *> d_float_pp4);
double host_store_fx(std::vector<float *> d_float_pp, std::vector<float *> d_float_pp1, std::vector<float *> d_float_pp2, std::vector<float *> d_float_pp3, std::vector<float *> d_float_pp4, std::vector<float *> d_float_sw, float *h_paraList, int para_size, int vamp_size, int paraSpline_size, double *h_fx, int numElements, int begin, int, int, int, int, int, int, double &, double, double, float, float, bool store_h_fx);
void cuda_configure_Pc_KMatrix(const float *paraList);
void cuda_configure_ampcount(const float *paraList, const int paraSize);
void cuda_configure_smear();
void host_store_amp2s(std::vector<float *> d_float_pp, std::vector<float *> d_float_pp1, std::vector<float *> d_float_pp2, std::vector<float *> d_float_pp3, std::vector<float *> d_float_pp4, float *h_paraList, int para_size, int vamp_size, int paraSpline_size, double *h_amp2s, int end, int begin, int NUMLz, int NUMPc, int NUMZcs, float _FFR, float _FFB);
void host_store_lineshapeLsKM(const float *h_mkp, float *h_KM_real, float *h_KM_imag, float *h_BW_real, float *h_BW_imag, const int n_points, const int NR, const int nLz, const float _FFR, float *h_paraList, const int para_size, const int vamp_size, const int paraSpline_size);

#endif
