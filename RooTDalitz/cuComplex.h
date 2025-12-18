/*
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO LICENSEE:
 *
 * This source code and/or documentation ("Licensed Deliverables") are
 * subject to NVIDIA intellectual property rights under U.S. and
 * international Copyright laws.
 *
 * These Licensed Deliverables contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a form of NVIDIA software license agreement by and
 * between NVIDIA and Licensee ("License Agreement") or electronically
 * accepted by Licensee.  Notwithstanding any terms or conditions to
 * the contrary in the License Agreement, reproduction or disclosure
 * of the Licensed Deliverables to any third party without the express
 * written consent of NVIDIA is prohibited.
 *
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, NVIDIA MAKES NO REPRESENTATION ABOUT THE
 * SUITABILITY OF THESE LICENSED DELIVERABLES FOR ANY PURPOSE.  IT IS
 * PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
 * NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THESE LICENSED
 * DELIVERABLES, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY
 * SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THESE LICENSED DELIVERABLES.
 *
 * U.S. Government End Users.  These Licensed Deliverables are a
 * "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT
 * 1995), consisting of "commercial computer software" and "commercial
 * computer software documentation" as such terms are used in 48
 * C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government
 * only as a commercial end item.  Consistent with 48 C.F.R.12.212 and
 * 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995), all
 * U.S. Government End Users acquire the Licensed Deliverables with
 * only those rights set forth herein.
 *
 * Any use of the Licensed Deliverables in individual and commercial
 * software must include, in the user documentation and internal
 * comments to the code, the above Disclaimer and U.S. Government End
 * Users Notice.
 */

#if !defined(CU_COMPLEX_H_)
#define CU_COMPLEX_H_

/* When trying to include C header file in C++ Code extern "C" is required
 * But the Standard QNX headers already have ifdef extern in them when compiling C++ Code
 * extern "C" cannot be nested
 * Hence keep the header out of extern "C" block
 */

#include <math.h>       /* import fabsf, sqrt */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include "vector_types.h"

typedef float2 cuFloatComplex;

__host__ __device__ static __inline__ float cuCrealf (cuFloatComplex x) 
{ 
    return x.x; 
}

__host__ __device__ static __inline__ float cuCimagf (cuFloatComplex x) 
{ 
    return x.y; 
}

__host__ __device__ static __inline__ cuFloatComplex make_cuFloatComplex 
                                                             (float r, float i)
{
    cuFloatComplex res;
    res.x = r;
    res.y = i;
    return res;
}

__host__ __device__ static __inline__ cuFloatComplex cuConjf (cuFloatComplex x)
{
    return make_cuFloatComplex (cuCrealf(x), -cuCimagf(x));
}
__host__ __device__ static __inline__ cuFloatComplex cuCaddf (cuFloatComplex x,
                                                              cuFloatComplex y)
{
    return make_cuFloatComplex (cuCrealf(x) + cuCrealf(y), 
                                cuCimagf(x) + cuCimagf(y));
}

__host__ __device__ static __inline__ cuFloatComplex cuCaddfc (float x,
                                                              cuFloatComplex y)
{
    return make_cuFloatComplex (x + cuCrealf(y), cuCimagf(y));
}

__host__ __device__ static __inline__ cuFloatComplex cuCaddcf (cuFloatComplex x,
                                                              float y)
{
    return make_cuFloatComplex (y + cuCrealf(x), cuCimagf(x));
}

__host__ __device__ static __inline__ cuFloatComplex cuCsubf (cuFloatComplex x,
                                                              cuFloatComplex y)
{
        return make_cuFloatComplex (cuCrealf(x) - cuCrealf(y), 
                                    cuCimagf(x) - cuCimagf(y));
}

__host__ __device__ static __inline__ cuFloatComplex cuCsubfc (float x,
                                                              cuFloatComplex y)
{
        return make_cuFloatComplex (x - cuCrealf(y), 
                                    0.0f - cuCimagf(y));
}

__host__ __device__ static __inline__ cuFloatComplex cuCsubcf (cuFloatComplex x,
                                                              float y)
{
        return make_cuFloatComplex (cuCrealf(x) - y, 
                                    cuCimagf(x) );
}

/* This implementation could suffer from intermediate overflow even though
 * the final result would be in range. However, various implementations do
 * not guard against this (presumably to avoid losing performance), so we 
 * don't do it either to stay competitive.
 */
__host__ __device__ static __inline__ cuFloatComplex cuCmulf (cuFloatComplex x,
                                                              cuFloatComplex y)
{
    cuFloatComplex prod;
    prod = make_cuFloatComplex  ((cuCrealf(x) * cuCrealf(y)) - 
                                 (cuCimagf(x) * cuCimagf(y)),
                                 (cuCrealf(x) * cuCimagf(y)) + 
                                 (cuCimagf(x) * cuCrealf(y)));
    return prod;
}

__host__ __device__ static __inline__ cuFloatComplex cuCmulfc (float x,
                                                              cuFloatComplex y)
{
    cuFloatComplex prod;
    prod = make_cuFloatComplex  (x * cuCrealf(y) ,
                                 x * cuCimagf(y)) ;
    return prod;
}

__host__ __device__ static __inline__ cuFloatComplex cuCmulcf (cuFloatComplex x,
                                                              float y)
{
    cuFloatComplex prod;
    prod = make_cuFloatComplex  (y * cuCrealf(x) ,
                                 y * cuCimagf(x)) ;
    return prod;
}
/* This implementation guards against intermediate underflow and overflow
 * by scaling. Such guarded implementations are usually the default for
 * complex library implementations, with some also offering an unguarded,
 * faster version.
 */
__host__ __device__ static __inline__ cuFloatComplex cuCdivf (cuFloatComplex x,
                                                              cuFloatComplex y)
{
    cuFloatComplex quot;
    float s = fabsf(cuCrealf(y)) + fabsf(cuCimagf(y));
    float oos = 1.0f / s;
    float ars = cuCrealf(x) * oos;
    float ais = cuCimagf(x) * oos;
    float brs = cuCrealf(y) * oos;
    float bis = cuCimagf(y) * oos;
    s = (brs * brs) + (bis * bis);
    oos = 1.0f / s;
    quot = make_cuFloatComplex (((ars * brs) + (ais * bis)) * oos,
                                ((ais * brs) - (ars * bis)) * oos);
    return quot;
}

__host__ __device__ static __inline__ cuFloatComplex cuCdivfc (float z,
                                                              cuFloatComplex y)
{
    cuFloatComplex quot;
    cuFloatComplex x = make_cuFloatComplex(z,0.0f);
    float s = fabsf(cuCrealf(y)) + fabsf(cuCimagf(y));
    float oos = 1.0f / s;
    float ars = cuCrealf(x) * oos;
    float ais = cuCimagf(x) * oos;
    float brs = cuCrealf(y) * oos;
    float bis = cuCimagf(y) * oos;
    s = (brs * brs) + (bis * bis);
    oos = 1.0f / s;
    quot = make_cuFloatComplex (((ars * brs) + (ais * bis)) * oos,
                                ((ais * brs) - (ars * bis)) * oos);
    return quot;
}

__host__ __device__ static __inline__ cuFloatComplex cuCdivcf (cuFloatComplex x,
                                                              float z)
{
    cuFloatComplex quot;
    cuFloatComplex y = make_cuFloatComplex(z,0.0f);
    float s = fabsf(cuCrealf(y)) + fabsf(cuCimagf(y));
    float oos = 1.0f / s;
    float ars = cuCrealf(x) * oos;
    float ais = cuCimagf(x) * oos;
    float brs = cuCrealf(y) * oos;
    float bis = cuCimagf(y) * oos;
    s = (brs * brs) + (bis * bis);
    oos = 1.0f / s;
    quot = make_cuFloatComplex (((ars * brs) + (ais * bis)) * oos,
                                ((ais * brs) - (ars * bis)) * oos);
    return quot;
}
/* 
 * We would like to call hypotf(), but it's not available on all platforms.
 * This discrete implementation guards against intermediate underflow and 
 * overflow by scaling. Otherwise we would lose half the exponent range. 
 * There are various ways of doing guarded computation. For now chose the 
 * simplest and fastest solution, however this may suffer from inaccuracies 
 * if sqrt and division are not IEEE compliant. 
 */
__host__ __device__ static __inline__ float cuCabsf (cuFloatComplex x)
{
    float a = cuCrealf(x);
    float b = cuCimagf(x);
    float v, w, t;
    a = fabsf(a);
    b = fabsf(b);
    if (a > b) {
        v = a;
        w = b; 
    } else {
        v = b;
        w = a;
    }
    t = w / v;
    t = 1.0f + t * t;
    t = v * sqrtf(t);
    if ((v == 0.0f) || (v > 3.402823466e38f) || (w > 3.402823466e38f)) {
        t = v + w;
    }
    return t;
}

/* Double precision */
typedef double2 cuDoubleComplex;

__host__ __device__ static __inline__ double cuCreal (cuDoubleComplex x) 
{ 
    return x.x; 
}

__host__ __device__ static __inline__ double cuCimag (cuDoubleComplex x) 
{ 
    return x.y; 
}

__host__ __device__ static __inline__ cuDoubleComplex make_cuDoubleComplex 
                                                           (double r, double i)
{
    cuDoubleComplex res;
    res.x = r;
    res.y = i;
    return res;
}

__host__ __device__ static __inline__ cuDoubleComplex cuConj(cuDoubleComplex x)
{
    return make_cuDoubleComplex (cuCreal(x), -cuCimag(x));
}

__host__ __device__ static __inline__ cuDoubleComplex cuCadd(cuDoubleComplex x,
                                                             cuDoubleComplex y)
{
    return make_cuDoubleComplex (cuCreal(x) + cuCreal(y), 
                                 cuCimag(x) + cuCimag(y));
}

__host__ __device__ static __inline__ cuDoubleComplex cuCadddc(double x,
                                                             cuDoubleComplex y)
{
    return make_cuDoubleComplex (x + cuCreal(y), 
                                 cuCimag(y));
}

__host__ __device__ static __inline__ cuDoubleComplex cuCaddcd(cuDoubleComplex x,
                                                             double y)
{
    return make_cuDoubleComplex (cuCreal(x) + y , 
                                 cuCimag(x) );
}

__host__ __device__ static __inline__ cuDoubleComplex cuCsub(cuDoubleComplex x,
                                                             cuDoubleComplex y)
{
    return make_cuDoubleComplex (cuCreal(x) - cuCreal(y), 
                                 cuCimag(x) - cuCimag(y));
}

__host__ __device__ static __inline__ cuDoubleComplex cuCsubdc(double x,
                                                             cuDoubleComplex y)
{
    return make_cuDoubleComplex (x - cuCreal(y), 
                                 0.0 - cuCimag(y));
}

__host__ __device__ static __inline__ cuDoubleComplex cuCsubcd(cuDoubleComplex x,
                                                             double y)
{
    return make_cuDoubleComplex (cuCreal(x) - y, 
                                 cuCimag(x));
}
/* This implementation could suffer from intermediate overflow even though
 * the final result would be in range. However, various implementations do
 * not guard against this (presumably to avoid losing performance), so we 
 * don't do it either to stay competitive.
 */
__host__ __device__ static __inline__ cuDoubleComplex cuCmul(cuDoubleComplex x,
                                                             cuDoubleComplex y)
{
    cuDoubleComplex prod;
    prod = make_cuDoubleComplex ((cuCreal(x) * cuCreal(y)) - 
                                 (cuCimag(x) * cuCimag(y)),
                                 (cuCreal(x) * cuCimag(y)) + 
                                 (cuCimag(x) * cuCreal(y)));
    return prod;
}

__host__ __device__ static __inline__ cuDoubleComplex cuCmuldc(double x,
                                                             cuDoubleComplex y)
{
    cuDoubleComplex prod;
    prod = make_cuDoubleComplex (x * cuCreal(y) ,
                                 x * cuCimag(y) );
    return prod;
}

__host__ __device__ static __inline__ cuDoubleComplex cuCmulcd(cuDoubleComplex x,
                                                             double y)
{
    cuDoubleComplex prod;
    prod = make_cuDoubleComplex (cuCreal(x) * y ,
                                 cuCimag(x) * y);
    return prod;
}
/* This implementation guards against intermediate underflow and overflow
 * by scaling. Such guarded implementations are usually the default for
 * complex library implementations, with some also offering an unguarded,
 * faster version.
 */
__host__ __device__ static __inline__ cuDoubleComplex cuCdiv(cuDoubleComplex x,
                                                             cuDoubleComplex y)
{
    cuDoubleComplex quot;
    double s = (fabs(cuCreal(y))) + (fabs(cuCimag(y)));
    double oos = 1.0 / s;
    double ars = cuCreal(x) * oos;
    double ais = cuCimag(x) * oos;
    double brs = cuCreal(y) * oos;
    double bis = cuCimag(y) * oos;
    s = (brs * brs) + (bis * bis);
    oos = 1.0 / s;
    quot = make_cuDoubleComplex (((ars * brs) + (ais * bis)) * oos,
                                 ((ais * brs) - (ars * bis)) * oos);
    return quot;
}

__host__ __device__ static __inline__ cuDoubleComplex cuCdivdc(double z,
                                                             cuDoubleComplex y)
{
    cuDoubleComplex quot;
    cuDoubleComplex x= make_cuDoubleComplex(z,0.0);
    double s = (fabs(cuCreal(y))) + (fabs(cuCimag(y)));
    double oos = 1.0 / s;
    double ars = cuCreal(x) * oos;
    double ais = cuCimag(x) * oos;
    double brs = cuCreal(y) * oos;
    double bis = cuCimag(y) * oos;
    s = (brs * brs) + (bis * bis);
    oos = 1.0 / s;
    quot = make_cuDoubleComplex (((ars * brs) + (ais * bis)) * oos,
                                 ((ais * brs) - (ars * bis)) * oos);
    return quot;
}

__host__ __device__ static __inline__ cuDoubleComplex cuCdivcd(cuDoubleComplex x,
                                                             double z)
{
    cuDoubleComplex quot;
    cuDoubleComplex y= make_cuDoubleComplex(z,0.0);
    double s = (fabs(cuCreal(y))) + (fabs(cuCimag(y)));
    double oos = 1.0 / s;
    double ars = cuCreal(x) * oos;
    double ais = cuCimag(x) * oos;
    double brs = cuCreal(y) * oos;
    double bis = cuCimag(y) * oos;
    s = (brs * brs) + (bis * bis);
    oos = 1.0 / s;
    quot = make_cuDoubleComplex (((ars * brs) + (ais * bis)) * oos,
                                 ((ais * brs) - (ars * bis)) * oos);
    return quot;
}
/* This implementation guards against intermediate underflow and overflow
 * by scaling. Otherwise we would lose half the exponent range. There are
 * various ways of doing guarded computation. For now chose the simplest
 * and fastest solution, however this may suffer from inaccuracies if sqrt
 * and division are not IEEE compliant.
 */
__host__ __device__ static __inline__ double cuCabs (cuDoubleComplex x)
{
    double a = cuCreal(x);
    double b = cuCimag(x);
    double v, w, t;
    a = fabs(a);
    b = fabs(b);
    if (a > b) {
        v = a;
        w = b; 
    } else {
        v = b;
        w = a;
    }
    t = w / v;
    t = 1.0 + t * t;
    t = v * sqrt(t);
    if ((v == 0.0) || 
        (v > 1.79769313486231570e+308) || (w > 1.79769313486231570e+308)) {
        t = v + w;
    }
    return t;
}

__host__ __device__ static __inline__ double cuCabs2 (cuDoubleComplex x)
{
    double a = cuCreal(x);
    double b = cuCimag(x);
    double v, w, t;
    a = fabs(a);
    b = fabs(b);
    if (a > b) {
        v = a;
        w = b; 
    } else {
        v = b;
        w = a;
    }
    t = w / v;
    t = 1.0 + t * t;
    t = v * v * t;
    if ((v == 0.0) || 
        (v > 1.79769313486231570e+308) || (w > 1.79769313486231570e+308)) {
        t = v + w;
 	  t = t*t;
    }
    return t;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/* aliases */
typedef cuFloatComplex cuComplex;
__host__ __device__ static __inline__ cuComplex make_cuComplex (float x, 
                                                                float y) 
{ 
    return make_cuFloatComplex (x, y); 
}

/* float-to-double promotion */
__host__ __device__ static __inline__ cuDoubleComplex cuComplexFloatToDouble
                                                      (cuFloatComplex c)
{
    return make_cuDoubleComplex ((double)cuCrealf(c), (double)cuCimagf(c));
}

__host__ __device__ static __inline__ cuFloatComplex cuComplexDoubleToFloat
(cuDoubleComplex c)
{
	return make_cuFloatComplex ((float)cuCreal(c), (float)cuCimag(c));
}


__host__ __device__ static __inline__  cuComplex cuCfmaf( cuComplex x, cuComplex y, cuComplex d)
{
    float real_res;
    float imag_res;
    
    real_res = (cuCrealf(x) *  cuCrealf(y)) + cuCrealf(d);
    imag_res = (cuCrealf(x) *  cuCimagf(y)) + cuCimagf(d);
            
    real_res = -(cuCimagf(x) * cuCimagf(y))  + real_res;  
    imag_res =  (cuCimagf(x) *  cuCrealf(y)) + imag_res;          
     
    return make_cuComplex(real_res, imag_res);
}

__host__ __device__ static __inline__  cuDoubleComplex cuCfma( cuDoubleComplex x, cuDoubleComplex y, cuDoubleComplex d)
{
    double real_res;
    double imag_res;
    
    real_res = (cuCreal(x) *  cuCreal(y)) + cuCreal(d);
    imag_res = (cuCreal(x) *  cuCimag(y)) + cuCimag(d);
            
    real_res = -(cuCimag(x) * cuCimag(y))  + real_res;  
    imag_res =  (cuCimag(x) *  cuCreal(y)) + imag_res;     
     
    return make_cuDoubleComplex(real_res, imag_res);
}

__host__ __device__ static __inline__ cuDoubleComplex cuCsqrt(cuDoubleComplex x)
{
   double radius = cuCabs(x);
   double cosA = x.x / radius;
   cuDoubleComplex out;
   out.x = sqrt(radius * (cosA + 1.0) / 2.0);
   out.y = sqrt(radius * (1.0 - cosA) / 2.0);
   // signbit should be false if x.y is negative
//   if (signbit(x.y))
   if (signbit(x.y) && fabs(x.y/x.x)>0.001)
	out.y *= -1.0;

   return out;
}


__host__ __device__ static __inline__ cuDoubleComplex cuCexp(cuDoubleComplex x)
{
   	double factor = exp(x.x);
		return make_cuDoubleComplex(factor * cos(x.y), factor * sin(x.y));
}


__host__ __device__ static __inline__ cuDoubleComplex cuClog(cuDoubleComplex x)
{
      double r = sqrt( x.x*x.x + x.y*x.y );
      double cos_theta = x.x/sqrt(x.x*x.x + x.y*x.y);
	double theta = acos(cos_theta);
	if( x.y<0 ){
	   theta = -1*theta;
	}
		return make_cuDoubleComplex( log(r), theta);
}


__host__ __device__ static __inline__ cuDoubleComplex cuCatan(cuDoubleComplex x)
{
      double2 ii = make_cuDoubleComplex(0,1);
	double2 ratio = cuClog( cuCdiv( cuCadddc(1.0, cuCmul(ii,x)), cuCsubdc(1.0, cuCmul(ii,x)) ) );
	
		return cuCmuldc(-1./2., cuCmul(ii, ratio));
}


__host__ __device__ static __inline__ double cuCnorm(cuDoubleComplex a, cuDoubleComplex b, cuDoubleComplex c)
{
   
   double absA = cuCabs(a);
   double absB = cuCabs(b);
   double absC = cuCabs(c);
   double u, v, w, r, s;

   if (absA > absB){
	if (absA > absC){
	   // a is largest
	   u = absA;
	   v = absB;
	   w = absC;

	}
	else{
	   // c is largest
	   u = absC;
	   v = absA;
	   w = absB;
	}
   }
   else{
	if (absB > absC){
	   // b is largest
	   u = absB;
	   v = absC;
	   w = absA;
	}
	else{
	   // c is largest
	   u = absC;
	   v = absA;
	   w = absB;
	}
   }
   r = v / u;
   s = w / u;
   r = 1.0 + r*r + s*s;
   r = u * sqrt(r);

   if ((u == 0.0) ||
	   (u > 1.79769313486231570e+308) || (v > 1.79769313486231570e+308) || (w > 1.79769313486231570e+308)) {
	r = u + v + w;
   }
   return r;
}


__host__ __device__ static __inline__ float cuCabsf2 (cuFloatComplex x)
{
    float a = cuCrealf(x);
    float b = cuCimagf(x);
    float v, w, t;
    a = fabsf(a);
    b = fabsf(b);
    if (a > b) {
        v = a;
        w = b;
    } else {
        v = b;
        w = a;
    }
    t = w / v;
    t = 1.0f + t * t;
    t = v * v *t ;
    if ((v == 0.0f) || (v > 3.402823466e38f) || (w > 3.402823466e38f)) {
        t = v + w;
        t = t * t;
    }
    return t;
}


//#include "cublas_v2.h"

__host__ __device__ static __inline__ cuFloatComplex cuCsqrtf(cuFloatComplex x)
{
   if(fabsf(x.x)<0.000001f && fabsf(x.y)<0.000001f) return make_cuFloatComplex(0.f, 0.f); 
   float radius = cuCabsf(x);
   float cosA = x.x / radius;
   cuFloatComplex out;
   out.x = sqrt(radius * (cosA + 1.0f) / 2.0f);
   out.y = sqrt(radius * (1.0f - cosA) / 2.0f);
//   printf("out.y = %f, step 1\n", out.y );
   // signbit should be false if x.y is negative
//   if (signbit(x.y))
   if (signbit(x.y) && fabsf(x.y/x.x)>0.001f)
	out.y *= -1.0f;
//   printf("out.y = %f, step 2\n", out.y );

   return out;
}


__host__ __device__ static __inline__ cuFloatComplex cuClogf(cuFloatComplex x)
{
      float r = sqrt( x.x*x.x + x.y*x.y );
      float cos_theta = x.x/sqrt(x.x*x.x + x.y*x.y);
	float theta = acos(cos_theta);
	if( x.y<0 ){
	   theta = -1*theta;
	}
		return make_cuFloatComplex( log(r), theta);
}

__host__ __device__ static __inline__ cuFloatComplex cuCatanf(cuFloatComplex x)
{
      float2 ii = make_cuFloatComplex(0,1);
	float2 ratio = cuClogf( cuCdivf( cuCaddfc(1.0, cuCmulf(ii,x)), cuCsubfc(1.0, cuCmulf(ii,x)) ) );
	
		return cuCmulfc(-1./2., cuCmulf(ii, ratio));
}



#endif /* !defined(CU_COMPLEX_H_) */
