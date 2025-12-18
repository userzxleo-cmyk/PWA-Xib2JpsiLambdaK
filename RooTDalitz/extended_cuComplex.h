#ifndef EXTENDED_CU_COMPLEX_h
#define EXTENDED_CU_COMPLEX_h

#include "../RooTDalitz/cuComplex.h"

__device__ static __inline__ double2 operator+(const double2& x,
                                               const double2& y)
{
    double2 result = cuCadd(x, y);
    return result;
}

__device__ static __inline__ double2 operator+(const double& x,
                                               const double2& y)
{
    double2 result = cuCadddc(x, y);
    return result;
}

__device__ static __inline__ double2 operator+(const double2& x,
                                               const double& y)
{
    double2 result = cuCadddc(y, x);
    return result;
}

__device__ static __inline__ double2 operator-(const double2& x,
                                               const double2& y)
{
    double2 result = cuCsub(x, y);
    return result;
}

__device__ static __inline__ double2 operator*(const double2& x,
                                               const double2& y)
{
    double2 result = cuCmul(x, y);
    return result;
}

__device__ static __inline__ double2 operator*(const double x,
                                               const double2& y)
{
    double2 result = cuCmuldc(x, y);
    return result;
}

__device__ static __inline__ double2 operator*(const double2& x,
                                               const double y)
{
    double2 result = cuCmuldc(y, x);
    return result;
}

__device__ static __inline__ double2 operator+(const float2& x,
                                               const double2& y)
{
    double2 result = cuCadd(cuComplexFloatToDouble(x), y);
    return result;
}

__device__ static __inline__ double2 operator-(const float2& x,
                                               const double2& y)
{
    double2 result = cuCsub(cuComplexFloatToDouble(x), y);
    return result;
}

__device__ static __inline__ double2 operator*(const float2& x,
                                               const double2& y)
{
    double2 result = cuCmul(cuComplexFloatToDouble(x), y);
    return result;
}

__device__ static __inline__ double2 operator*(const float x,
                                               const double2& y)
{
    double2 result = cuCmuldc((double)x, y);
    return result;
}

__device__ static __inline__ double2 operator*(const float2& x,
                                               const double y)
{
    double2 result = cuCmuldc(y, cuComplexFloatToDouble(x));
    return result;
}

__device__ static __inline__ double2 operator+(const double2& x,
                                               const float2& y)
{
    double2 result = cuCadd(x, cuComplexFloatToDouble(y));
    return result;
}

__device__ static __inline__ double2 operator-(const double2& x,
                                               const float2& y)
{
    double2 result = cuCsub(x, cuComplexFloatToDouble(y));
    return result;
}

__device__ static __inline__ double2 operator*(const double2& x,
                                               const float2& y)
{   
    double2 result = cuCmul(x, cuComplexFloatToDouble(y));
    return result;
}

__device__ static __inline__ double2 operator*(const double2& x,
                                              const float y)
{
    double2 result = cuCmuldc((double)y, x);
    return result;
}

__device__ static __inline__ double2 operator*(const double x,
                                              const float2& y)
{
    double2 result = cuCmuldc(x, cuComplexFloatToDouble(y));
    return result;
}

__device__ static __inline__ float2 operator+(const float2& x,
                                              const float2& y)
{
    float2 result = cuCaddf(x, y);
    return result;
}

__device__ static __inline__ float2 operator+(const float& x,
                                              const float2& y)
{
    float2 result = cuCaddfc(x, y);
    return result;
}

__device__ static __inline__ float2 operator+(const float2& x,
                                              const float& y)
{
    float2 result = cuCaddfc(y, x);
    return result;
}

__device__ static __inline__ float2 operator-(const float2& x,
                                              const float2& y)
{
    float2 result = cuCsubf(x, y);
    return result;
}

__device__ static __inline__ float2 operator*(const float2& x,
                                              const float2& y)
{
    float2 result = cuCmulf(x, y);
    return result;
}

__device__ static __inline__ float2 operator*(const float x,
                                              const float2& y)
{
    float2 result = cuCmulfc(x, y);
    return result;
}

__device__ static __inline__ float2 operator*(const float2& x,
                                              const float y)
{
    float2 result = cuCmulfc(y, x);
    return result;
}

__device__ static __inline__ float2 operator- (const float2& x)
{   
    float2 result{
        -x.x,
        -x.y
    };
    return result;
}

__device__ static __inline__ double2 operator- (const double2& x)
{   
    double2 result{
        -x.x,
        -x.y
    };
    return result;
}

__device__ static __inline__ float magnitude2(const float2& x)
{
    return cuCabsf2(x);
}

__device__ static __inline__ double magnitude2(const double2& x)
{
    return cuCabs2(x);
}

__device__ static __inline__ float real_part(const float2& x)
{
    return cuCrealf(x);
}

__device__ static __inline__ double real_part(const double2& x)
{
    return cuCreal(x);
}

__device__ static __inline__ float imag_part(const float2& x)
{
    return cuCimagf(x);
}

__device__ static __inline__ double imag_part(const double2& x)
{
    return cuCimag(x);
}

#endif
