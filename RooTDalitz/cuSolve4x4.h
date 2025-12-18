#ifndef solve4x4_c_h
#define solve4x4_c_h

#include "../RooTDalitz/cuComplex.h"
#include "../RooTDalitz/extended_cuComplex.h"
#include <assert.h>

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

__device__ static __inline__ double2 cuCinverse (const double2& x)
{
    // We only invert 4 times
    //   Use double so we don't lose precision
    double real = x.x;
    double imag = x.y;
    double s = real * real + imag * imag;
    double norm = 1/s;

    real = norm * real;
    imag = -norm * imag;
    double2 result{
        real,
        imag
    };
    return result;
}

template <typename T, typename U>
__device__ static __inline__ U cuComplexConvert(const T& x)
{
    U result;
    result.x = x.x;
    result.y = x.y;

    return result;
}

template <typename T, typename B>
// Basically solves LUx = b, find x given b
__device__ static __inline__ void solve(const T& L10,
                                        const T& L20, const T& L21,
                                        const T& L30, const T& L31, const T& L32,
                                        const T& U00, const T& U01, const T& U02, const T& U03,
                                        const T& U11, const T& U12, const T& U13,
                                        const T& U22, const T& U23,
                                        const T& U33,
                                        const T& inv_U00, const T& inv_U11,
                                        const T& inv_U22, const T& inv_U33,
                                        const B * b, B * x
                                        )
{
    const B y0 = b[0];
    const B y1 = b[1] - L10 * y0;
    const B y2 = b[2] - L20 * y0 - L21 * y1;
    const B y3 = b[3] - L30 * y0 - L31 * y1 - L32 * y2;

    x[3] = inv_U33 * y3;
    x[2] = inv_U22 * (y2 - U23 * x[3]);
    x[1] = inv_U11 * (y1 - U12 * x[2] - U13 * x[3]);
    x[0] = inv_U00 * (y0 - U01 * x[1] - U02 * x[2] - U03 * x[3]);
}

template <typename A, typename B, typename X>
// Calculate the error to correct for
__device__ static __inline__ B compensated_error(const B& b, 
                                                 const A& A0, const X& x0,
                                                 const A& A1, const X& x1,
                                                 const A& A2, const X& x2,
                                                 const A& A3, const X& x3
                                                )
{
    // Use double anyway for best precision since we want something close to 0
    X error{b.x, b.y};
    X d_A0{A0.x, A0.y};
    X d_A1{A1.x, A1.y};
    X d_A2{A2.x, A2.y};
    X d_A3{A3.x, A3.y};

    // Simplified first two loops of kahan where carry = 0, sum = 0
    error = error - d_A0 * x0 - d_A1 * x1 - d_A2 * x2 - d_A3 * x3;

    X result = cuComplexConvert<X, B>(error);
    return result;
}

template <typename T, typename B>
// Calculate the error to correct for
__device__ static __inline__ void error(const T& A00, const T& A01, const T& A02, const T& A03,
                                        const T& A10, const T& A11, const T& A12, const T& A13,
                                        const T& A20, const T& A21, const T& A22, const T& A23,
                                        const T& A30, const T& A31, const T& A32, const T& A33,
                                        const B* b, const B* x, B * e
                                       ) 
{
    e[0] = compensated_error(b[0], A00, x[0], A01, x[1], A02, x[2], A03, x[3]);
    e[1] = compensated_error(b[1], A10, x[0], A11, x[1], A12, x[2], A13, x[3]);
    e[2] = compensated_error(b[2], A20, x[0], A21, x[1], A22, x[2], A23, x[3]);
    e[3] = compensated_error(b[3], A30, x[0], A31, x[1], A32, x[2], A33, x[3]);
}

// Define lower triangular and upper triangular structs
// These are just used for easier handling of data
// The indexing are done at compile-time in the matrix inversion code so we use static_assert
template <typename T, size_t size>
struct lower_t
{
    static_assert(size > 0);
    // 1 on the diagonal
    T data[size * (size - 1) / 2];

    // On the ith row, we have gone through i rows.
    // [ 0, 1, 2, ... i - 1 ] entries on each row (1 on diagonal)
    // Doing an arithmetic sum till i - 1 gives ( i - 1 ) * i / 2 entries
    // Before this row
    const __device__ __inline__ T& operator()(const int row, const int column) const {
        assert(row > 0);
        assert(row > column);
        assert(row < size);
        const int index = ( row - 1 ) * ( row ) / 2 + column;

        return data[index];
    }

    __device__ __inline__ T& operator()(const int row, const int column) {
        assert(row > 0);
        assert(row > column);
        assert(row < size);
        const int index = ( row - 1 ) * ( row ) / 2 + column;

        return data[index];
    }
};

template <typename T, size_t size>
struct upper_t
{   
    // 1 on the diagonal
    static_assert(size > 0);
    T data[size * (size + 1) / 2];

    // On the ith row, we have been gone throught i rows,
    // [ size, size - 1, size - 2 ... size - i - 1] entries on each row
    // So there are entries = sum_j ( size - j ) = size * i - i * (i - 1) / 2
    // Before this row
    const __device__ __inline__ T& operator()(const int row, const int column) const {
        assert(row <= column);
        assert(column < size);
        const int index =  row * ( size ) - row * ( row - 1 ) / 2 + ( column - row );

        return data[index];
    }

    __device__ __inline__ T& operator()(const int row, const int column){
        assert(row <= column);
        assert(column < size);
        const int index =  row * ( size ) - row * ( row - 1 ) / 2 + ( column - row );

        return data[index];
    }
};

// Template transposed matrix based on whether we want to cache the values
template <typename T, size_t size, size_t nCorrection>
struct transposed_t
{
    static_assert(size > 0);
    T data[size*size];

    __device__ transposed_t(const float2 * source){
        #pragma unroll
        for (int i=0; i<size; i++){
            #pragma unroll
            for (int j=0; j<size; j++){
                const int index = i * size + j;
                const int srcID = j * size + i;
                data[index] = cuComplexConvert<float2, T>( source[srcID] );
            }
        }
    }

    __device__ const __inline__ T& operator()(const int row, const int column) const {
        assert(row < size);
        assert(column < size);
        const auto index = row * size + column;
        return data[index];
    }
};

// // Simple structure to get row and column of inverted matrix
// template <typename T, size_t size>
// struct transposed_t<typename T, size_t size, true>
// {
// };

// Simple structure to get row and column of inverted matrix
template <typename T, size_t size>
struct transposed_t<T, size, 0>
{
    const float2 * data;

    __device__ transposed_t(const float2 * source) : data(source) {
        // data = source;
    }

    __device__ const __inline__ T operator()(const int row, const int column) const {
        assert(row < size);
        assert(column < size);
        const auto index = column * size + row;
        return cuComplexConvert<float2, T>( data[index] );
    }
};

template <typename T, typename B, size_t size>
// Basically solves LUx = b, find x given b
__device__ static __inline__ void substitution(const lower_t<T, size>& lower, const upper_t<T, size>& upper, const T * inv_upper,
                                               const B * b, B * x
                                              )
{
    B y[size];

    // Ly = b
    // forward substitution
    #pragma unroll
    for (int i=0; i<size; i++){
        y[i] = b[i];
        #pragma unroll
        for (int j=0; j<i; j++){
            y[i] = y[i] - lower(i, j) * y[j];
        }
    }

    // Ux = y
    // backward substition
    #pragma unroll
    for (int i=(size-1); i>=0; i--){
        x[i] = y[i];
        #pragma unroll
        for (int j=i+1; j<size; j++){
            x[i] = x[i] - upper(i, j) * x[j];
        }
        x[i] = inv_upper[i] * x[i];
    }
}

template <typename T, typename B, size_t size>
// Calculate the error to correct for
__device__ static __inline__ void error(const transposed_t<T,size,true>& A,
                                        const B* b, const B* x, B * e
                                       ) 
{
    #pragma unroll
    for (int i=0; i<size; i++){
        e[i] = b[i];
        #pragma unroll
        for (int j=0; j<size; j++){
            e[i] = e[i] - A(i, j) * x[j];
        }
    }
}

// Matrix solve, returns 1st row
template <typename LU, typename B, const size_t nCorrection, const size_t size>
__device__ static __inline__ void solve_NxN(const float2 * S, float2 * dst){
    // static_assert(size > 1); // Why do you want to invert a matrix of size 1?

    // const bool cache = ( nCorrection > 0 ) ;
    const transposed_t<LU, size, nCorrection> A( S );
    // const auto A = ( nCorrection > 0 ) transposed_t<LU, size, true>( S ) : transposed_t<LU, size, false>( S );

    lower_t<LU,size> L;
    upper_t<LU,size> U;
    LU inv_U[size];

    // LU decomposition
    #pragma unroll
    for (int i=0; i<size; i++){ // row of upper, column of lower
        #pragma unroll
        for (int column=i; column<size; column++){
            U(i, column) = A(i, column);
            // Subtract up till k=i
            #pragma unroll
            for (int k=0; k<i; k++){
                U(i, column) = U(i, column) - L(i, k) * U(k, column);
            }
        }

        inv_U[i] = cuCinverse(U(i, i));

        #pragma unroll
        for (int row=(i+1); row<size; row++){
            L(row, i) = A(row, i);
            #pragma unroll
            for (int k=0; k<i; k++){
                L(row, i) = L(row, i) - L(row, k) * U(k, i);
            }
            L(row, i) = L(row, i) * inv_U[i];
        }
    }

    B b[size];
    B x[size];
    b[0] = { 1, 0 };
    #pragma unroll
    for (int i = 1; i < size; i++){
        b[i] = { 0, 0 };
    }

    substitution(L, U, inv_U, b, x);
    // if (nCorrection > 0){
    //     B r[size];
    //     B e[size];

    //     for (int i = 0; i < nCorrection; i++){
    //         error(A00, A01, A02, A03,
    //               A10, A11, A12, A13,
    //               A20, A21, A22, A23,
    //               A30, A31, A32, A33,
    //               b, x, r
    //              );

    //         substitution<size>(L, U, inv_U, r, e);

    //         #pragma unroll
    //         for (int j=0; j<size; j++){
    //             x[j] = x[j] + e[j];
    //         }
    //     }
    // }

    #pragma unroll
    for (int i=0; i<size; i++){
        dst[i] = cuComplexConvert<B, float2>(x[i]);
    }
}

// Template the precision of LU triangular matrices and b vector
// Calculates the first row of an inverted 4x4 matrix
// Solves Ax = b, b = [1,0,0,0] to find x = first row of A inverse
template <typename LU, typename B, size_t nCorrection>
__device__ static __inline__ void solve4x4(const float2 * S, float2 * dst)
{
    // Note that we solve with transpose of (1 - i*K*rho), A = src^T
    // Transpose is expensive, we just use correct indices instead
    const LU A00 = cuComplexConvert<float2, LU>(S[0]);
    const LU A01 = cuComplexConvert<float2, LU>(S[4]);
    const LU A02 = cuComplexConvert<float2, LU>(S[8]);
    const LU A03 = cuComplexConvert<float2, LU>(S[12]);
    const LU A10 = cuComplexConvert<float2, LU>(S[1]);
    const LU A11 = cuComplexConvert<float2, LU>(S[5]);
    const LU A12 = cuComplexConvert<float2, LU>(S[9]);
    const LU A13 = cuComplexConvert<float2, LU>(S[13]);
    const LU A20 = cuComplexConvert<float2, LU>(S[2]);
    const LU A21 = cuComplexConvert<float2, LU>(S[6]);
    const LU A22 = cuComplexConvert<float2, LU>(S[10]);
    const LU A23 = cuComplexConvert<float2, LU>(S[14]);
    const LU A30 = cuComplexConvert<float2, LU>(S[3]);
    const LU A31 = cuComplexConvert<float2, LU>(S[7]);
    const LU A32 = cuComplexConvert<float2, LU>(S[11]);
    const LU A33 = cuComplexConvert<float2, LU>(S[15]);

    // First get L U matrices
    const LU U00 = A00;
    const LU U01 = A01;
    const LU U02 = A02;
    const LU U03 = A03;
    const LU inv_U00 = cuCinverse(U00);

    const LU L10 = A10 * inv_U00;
    const LU L20 = A20 * inv_U00;
    const LU L30 = A30 * inv_U00;

    const LU U11 = A11 - L10 * U01;
    const LU U12 = A12 - L10 * U02;
    const LU U13 = A13 - L10 * U03;
    const LU inv_U11 = cuCinverse(U11);

    const LU L21 = inv_U11 * ( A21 - L20 * U01 );
    const LU L31 = inv_U11 * ( A31 - L30 * U01 );

    const LU U22 = A22 - L20 * U02 - L21 * U12;
    const LU U23 = A23 - L20 * U03 - L21 * U13;
    const LU inv_U22 = cuCinverse(U22);

    const LU L32 = inv_U22 * ( A32 - L30 * U02 - L31 * U12 );
    const LU U33 = A33 - L30 * U03 - L31 * U13 - L32 * U23;
    const LU inv_U33 = cuCinverse(U33);

    // First iteration, assume [1, 0, 0, 0]
    B b[4];
    B x[4];

    b[0] = (B){1., 0.};
    b[1] = (B){0., 0.};
    b[2] = (B){0., 0.};
    b[3] = (B){0., 0.};

    solve(L10, 
          L20, L21,
          L30, L31, L32,
          U00, U01, U02, U03,
          U11, U12, U13,
          U22, U23,
          U33,
          inv_U00, inv_U11,
          inv_U22, inv_U33,
          b, x
         );

    if (nCorrection > 0){
        B r[4];
        B e[4];

        for (int i = 0; i < nCorrection; i++){
            error(A00, A01, A02, A03,
                  A10, A11, A12, A13,
                  A20, A21, A22, A23,
                  A30, A31, A32, A33,
                  b, x, r
                 );

            solve(L10, 
                  L20, L21,
                  L30, L31, L32,
                  U00, U01, U02, U03,
                  U11, U12, U13,
                  U22, U23,
                  U33,
                  inv_U00, inv_U11,
                  inv_U22, inv_U33,
                  r, e
                 );

            x[0] = x[0] + e[0];
            x[1] = x[1] + e[1];
            x[2] = x[2] + e[2];
            x[3] = x[3] + e[3];
        }
    }

    dst[0] = cuComplexConvert<B, float2>(x[0]);
    dst[1] = cuComplexConvert<B, float2>(x[1]);
    dst[2] = cuComplexConvert<B, float2>(x[2]);
    dst[3] = cuComplexConvert<B, float2>(x[3]);
}

template <typename LU, typename B, size_t nCorrection>
__device__ static __inline__ void solve_7x7(const float2 * S, float2 * dst)
{
    const LU A00 = cuComplexConvert<float2, LU>(S[0]);
    const LU A01 = cuComplexConvert<float2, LU>(S[7]);
    const LU A02 = cuComplexConvert<float2, LU>(S[14]);
    const LU A03 = cuComplexConvert<float2, LU>(S[21]);
    const LU A04 = cuComplexConvert<float2, LU>(S[28]);
    const LU A05 = cuComplexConvert<float2, LU>(S[35]);
    const LU A06 = cuComplexConvert<float2, LU>(S[42]);
    const LU A10 = cuComplexConvert<float2, LU>(S[1]);
    const LU A11 = cuComplexConvert<float2, LU>(S[8]);
    const LU A12 = cuComplexConvert<float2, LU>(S[15]);
    const LU A13 = cuComplexConvert<float2, LU>(S[22]);
    const LU A14 = cuComplexConvert<float2, LU>(S[29]);
    const LU A15 = cuComplexConvert<float2, LU>(S[36]);
    const LU A16 = cuComplexConvert<float2, LU>(S[43]);
    const LU A20 = cuComplexConvert<float2, LU>(S[2]);
    const LU A21 = cuComplexConvert<float2, LU>(S[9]);
    const LU A22 = cuComplexConvert<float2, LU>(S[16]);
    const LU A23 = cuComplexConvert<float2, LU>(S[23]);
    const LU A24 = cuComplexConvert<float2, LU>(S[30]);
    const LU A25 = cuComplexConvert<float2, LU>(S[37]);
    const LU A26 = cuComplexConvert<float2, LU>(S[44]);
    const LU A30 = cuComplexConvert<float2, LU>(S[3]);
    const LU A31 = cuComplexConvert<float2, LU>(S[10]);
    const LU A32 = cuComplexConvert<float2, LU>(S[17]);
    const LU A33 = cuComplexConvert<float2, LU>(S[24]);
    const LU A34 = cuComplexConvert<float2, LU>(S[31]);
    const LU A35 = cuComplexConvert<float2, LU>(S[38]);
    const LU A36 = cuComplexConvert<float2, LU>(S[45]);
    const LU A40 = cuComplexConvert<float2, LU>(S[4]);
    const LU A41 = cuComplexConvert<float2, LU>(S[11]);
    const LU A42 = cuComplexConvert<float2, LU>(S[18]);
    const LU A43 = cuComplexConvert<float2, LU>(S[25]);
    const LU A44 = cuComplexConvert<float2, LU>(S[32]);
    const LU A45 = cuComplexConvert<float2, LU>(S[39]);
    const LU A46 = cuComplexConvert<float2, LU>(S[46]);
    const LU A50 = cuComplexConvert<float2, LU>(S[5]);
    const LU A51 = cuComplexConvert<float2, LU>(S[12]);
    const LU A52 = cuComplexConvert<float2, LU>(S[19]);
    const LU A53 = cuComplexConvert<float2, LU>(S[26]);
    const LU A54 = cuComplexConvert<float2, LU>(S[33]);
    const LU A55 = cuComplexConvert<float2, LU>(S[40]);
    const LU A56 = cuComplexConvert<float2, LU>(S[47]);
    const LU A60 = cuComplexConvert<float2, LU>(S[6]);
    const LU A61 = cuComplexConvert<float2, LU>(S[13]);
    const LU A62 = cuComplexConvert<float2, LU>(S[20]);
    const LU A63 = cuComplexConvert<float2, LU>(S[27]);
    const LU A64 = cuComplexConvert<float2, LU>(S[34]);
    const LU A65 = cuComplexConvert<float2, LU>(S[41]);
    const LU A66 = cuComplexConvert<float2, LU>(S[48]);

    const LU U00 = A00;
    const LU U01 = A01;
    const LU U02 = A02;
    const LU U03 = A03;
    const LU U04 = A04;
    const LU U05 = A05;
    const LU U06 = A06;
    const LU inv_U00 = cuCinverse( U00 );

    const LU L10 = inv_U00 * ( A10 );
    const LU L20 = inv_U00 * ( A20 );
    const LU L30 = inv_U00 * ( A30 );
    const LU L40 = inv_U00 * ( A40 );
    const LU L50 = inv_U00 * ( A50 );
    const LU L60 = inv_U00 * ( A60 );

    const LU U11 = A11 - L10 * U01;
    const LU U12 = A12 - L10 * U02;
    const LU U13 = A13 - L10 * U03;
    const LU U14 = A14 - L10 * U04;
    const LU U15 = A15 - L10 * U05;
    const LU U16 = A16 - L10 * U06;
    const LU inv_U11 = cuCinverse( U11 );

    const LU L21 = inv_U11 * ( A21 - L20 * U01 );
    const LU L31 = inv_U11 * ( A31 - L30 * U01 );
    const LU L41 = inv_U11 * ( A41 - L40 * U01 );
    const LU L51 = inv_U11 * ( A51 - L50 * U01 );
    const LU L61 = inv_U11 * ( A61 - L60 * U01 );

    const LU U22 = A22 - L20 * U02 - L21 * U12;
    const LU U23 = A23 - L20 * U03 - L21 * U13;
    const LU U24 = A24 - L20 * U04 - L21 * U14;
    const LU U25 = A25 - L20 * U05 - L21 * U15;
    const LU U26 = A26 - L20 * U06 - L21 * U16;
    const LU inv_U22 = cuCinverse( U22 );

    const LU L32 = inv_U22 * ( A32 - L30 * U02 - L31 * U12 );
    const LU L42 = inv_U22 * ( A42 - L40 * U02 - L41 * U12 );
    const LU L52 = inv_U22 * ( A52 - L50 * U02 - L51 * U12 );
    const LU L62 = inv_U22 * ( A62 - L60 * U02 - L61 * U12 );

    const LU U33 = A33 - L30 * U03 - L31 * U13 - L32 * U23;
    const LU U34 = A34 - L30 * U04 - L31 * U14 - L32 * U24;
    const LU U35 = A35 - L30 * U05 - L31 * U15 - L32 * U25;
    const LU U36 = A36 - L30 * U06 - L31 * U16 - L32 * U26;
    const LU inv_U33 = cuCinverse( U33 );

    const LU L43 = inv_U33 * ( A43 - L40 * U03 - L41 * U13 - L42 * U23 );
    const LU L53 = inv_U33 * ( A53 - L50 * U03 - L51 * U13 - L52 * U23 );
    const LU L63 = inv_U33 * ( A63 - L60 * U03 - L61 * U13 - L62 * U23 );

    const LU U44 = A44 - L40 * U04 - L41 * U14 - L42 * U24 - L43 * U34;
    const LU U45 = A45 - L40 * U05 - L41 * U15 - L42 * U25 - L43 * U35;
    const LU U46 = A46 - L40 * U06 - L41 * U16 - L42 * U26 - L43 * U36;
    const LU inv_U44 = cuCinverse( U44 );

    const LU L54 = inv_U44 * ( A54 - L50 * U04 - L51 * U14 - L52 * U24 - L53 * U34 );
    const LU L64 = inv_U44 * ( A64 - L60 * U04 - L61 * U14 - L62 * U24 - L63 * U34 );

    const LU U55 = A55 - L50 * U05 - L51 * U15 - L52 * U25 - L53 * U35 - L54 * U45;
    const LU U56 = A56 - L50 * U06 - L51 * U16 - L52 * U26 - L53 * U36 - L54 * U46;
    const LU inv_U55 = cuCinverse( U55 );

    const LU L65 = inv_U55 * ( A65 - L60 * U05 - L61 * U15 - L62 * U25 - L63 * U35 - L64 * U45 );

    const LU U66 = A66 - L60 * U06 - L61 * U16 - L62 * U26 - L63 * U36 - L64 * U46 - L65 * U56;
    const LU inv_U66 = cuCinverse( U66 );


    constexpr B b0 = { 1, 0 };
    constexpr B b1 = { 0, 0 };
    constexpr B b2 = { 0, 0 };
    constexpr B b3 = { 0, 0 };
    constexpr B b4 = { 0, 0 };
    constexpr B b5 = { 0, 0 };
    constexpr B b6 = { 0, 0 };

    const B y0 = b0;
    const B y1 = b1 - L10 * y0;
    const B y2 = b2 - L20 * y0 - L21 * y1;
    const B y3 = b3 - L30 * y0 - L31 * y1 - L32 * y2;
    const B y4 = b4 - L40 * y0 - L41 * y1 - L42 * y2 - L43 * y3;
    const B y5 = b5 - L50 * y0 - L51 * y1 - L52 * y2 - L53 * y3 - L54 * y4;
    const B y6 = b6 - L60 * y0 - L61 * y1 - L62 * y2 - L63 * y3 - L64 * y4 - L65 * y5;

    const B x6 = inv_U66 * ( y6 );
    const B x5 = inv_U55 * ( y5 - U56 * x6 );
    const B x4 = inv_U44 * ( y4 - U45 * x5 - U46 * x6 );
    const B x3 = inv_U33 * ( y3 - U34 * x4 - U35 * x5 - U36 * x6 );
    const B x2 = inv_U22 * ( y2 - U23 * x3 - U24 * x4 - U25 * x5 - U26 * x6 );
    const B x1 = inv_U11 * ( y1 - U12 * x2 - U13 * x3 - U14 * x4 - U15 * x5 - U16 * x6 );
    const B x0 = inv_U00 * ( y0 - U01 * x1 - U02 * x2 - U03 * x3 - U04 * x4 - U05 * x5 - U06 * x6 );


    dst[0] = cuComplexConvert<B, float2>(x0);
    dst[1] = cuComplexConvert<B, float2>(x1);
    dst[2] = cuComplexConvert<B, float2>(x2);
    dst[3] = cuComplexConvert<B, float2>(x3);
    dst[4] = cuComplexConvert<B, float2>(x4);
    dst[5] = cuComplexConvert<B, float2>(x5);
    dst[6] = cuComplexConvert<B, float2>(x6);
}

#endif /* solve4x4_c_h */
