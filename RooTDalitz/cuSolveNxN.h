#ifndef solveNxN_c_h
#define solveNxN_c_h

#include "../RooTDalitz/cuComplex.h"
#include "../RooTDalitz/extended_cuComplex.h"

__device__ static __inline__ float2 cuCinverse (const float2& x)
{
    // We only invert 4 times
    //   Use double so we don't lose precision
    double real = x.x;
    double imag = x.y;
    double s = real * real + imag * imag;
    double norm = 1/s;

    real = norm * real;
    imag = -norm * imag;
    float2 result{
        (float) real,
        (float) imag
    };
    return result;
}

#endif
