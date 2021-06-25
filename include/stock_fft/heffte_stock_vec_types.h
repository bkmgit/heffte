/*
    -- heFFTe --
       Univ. of Tennessee, Knoxville
       @date
*/

#ifndef HEFFTE_STOCK_VEC_TYPES_H
#define HEFFTE_STOCK_VEC_TYPES_H

#ifdef __AVX__
#include <immintrin.h>
#endif
#include <complex>

namespace heffte {
namespace stock {
//! \brief Alias to decide whether a type is identical to float
template<typename T>
using is_float = std::is_same<float, typename std::remove_cv<T>::type>;
//! \brief Alias to decide whether a type is identical to double
template<typename T>
using is_double = std::is_same<double, typename std::remove_cv<T>::type>;
//! \brief Alias to decide whether a type is identical to std::complex float
template<typename T>
using is_fcomplex = std::is_same<std::complex<float>, typename std::remove_cv<T>::type>;
//! \brief Alias to decide whether a type is identical to std::complex double
template<typename T>
using is_dcomplex = std::is_same<std::complex<double>, typename std::remove_cv<T>::type>;

//! \brief Struct determining whether a type is a real number
template<typename T> struct is_real {
    static constexpr bool value = is_float<T>::value || is_double<T>::value;
};

//! \brief Struct determining whether a type is a complex number
template<typename T> struct is_complex {
    static constexpr bool value = is_fcomplex<T>::value || is_dcomplex<T>::value;
};

//! \brief Struct to retrieve the vector type associated with the number of elements stored "per unit"
template<typename T, int N> struct pack {};
//! \brief Alias for default float type
template<> struct pack<float, 1> { using type = std::complex<float>; };
//! \brief Alias for default double type
template<> struct pack<double, 1> { using type = std::complex<double>; };

// Some simple operations that will be useful for vectorized types.

/*!
 * \ingroup stockvectype
 * \brief Create zero of type
 */
template<typename F, int L>
inline typename pack<F,L>::type mm_zero(){return 0.0;}
/*!
 *  \ingroup stockvectype
 *  \brief Load vectorized type from primitive
 */
template<typename F, int L>
inline typename pack<F,L>::type mm_load(F const *src) { return typename pack<F,L>::type {src[0], src[1]}; }
/*!
 *  \ingroup stockvectype
 *  \brief Store vectorized type into primitives
 */
template<typename F, int L>
inline void mm_store(F *dest, typename pack<F,L>::type const &src) {
    dest[0] = src.real(); dest[1] = src.imag();
}
/*!
 *  \ingroup stockvectype
 *  \brief Have vectorized pack to be repeated two values
 */
template<typename F, int L>
inline typename pack<F,L>::type mm_pair_set(F x, F y) { return typename pack<F,L>::type(x, y); }
/*!
 *  \ingroup stockvectype
 *  \brief Set all elements of type as one value
 */
template<typename F, int L>
inline typename pack<F,L>::type mm_set1(F src) { return typename pack<F,L>::type(src,src); }
/*!
 *  \ingroup stockvectype
 *  \brief Load a pack from a pointer to std::complex values
 */
template<typename F, int L>
inline typename pack<F,L>::type mm_complex_load(std::complex<F> const *src) { return *src; }
/*!
 *  \ingroup stockvectype
 *  \brief Load a pack from a pointer to std::complex values
 */
template<typename F, int L>
inline typename pack<F,L>::type mm_complex_load(std::complex<F> const *src, int) { return *src; }

// Real basic arithmetic for the "none" case

//! \brief Calculate single-precision complex addition
inline typename pack<float, 1>::type mm_add(typename pack<float, 1>::type const &a, typename pack<float, 1>::type const &b){ return a + b; }
//! \brief Calculate double-precision complex addition
inline typename pack<double, 1>::type mm_add(typename pack<double, 1>::type const &a, typename pack<double, 1>::type const &b){ return a + b; }
//! \brief Calculate single-precision complex subtraction
inline typename pack<float, 1>::type mm_sub(typename pack<float, 1>::type const &a, typename pack<float, 1>::type const &b){ return a - b; }
//! \brief Calculate double-precision complex subtraction
inline typename pack<double, 1>::type mm_sub(typename pack<double, 1>::type const &a, typename pack<double, 1>::type const &b){ return a - b; }
//! \brief Calculate single-precision a division
inline typename pack<float, 1>::type mm_div(typename pack<float, 1>::type const &a, typename pack<float, 1>::type const &b){ return a / b.real(); }
//! \brief Calculate double-precision a division
inline typename pack<double, 1>::type mm_div(typename pack<double, 1>::type const &a, typename pack<double, 1>::type const &b){ return a / b.real(); }
//! \brief Calculate single-precision a multiplication
inline typename pack<float, 1>::type mm_mul(typename pack<float, 1>::type const &a, typename pack<float, 1>::type const &b){ return a * b.real(); }
//! \brief Calculate double-precision a multiplication
inline typename pack<double, 1>::type mm_mul(typename pack<double, 1>::type const &a, typename pack<double, 1>::type const &b){ return a * b.real(); }
//! \brief Perform single-precision complex multiplication
inline typename pack<float, 1>::type mm_complex_mul(typename pack<float, 1>::type const &a, typename pack<float, 1>::type const &b){ return a * b; }
//! \brief Perform double-precision complex multiplication
inline typename pack<double, 1>::type mm_complex_mul(typename pack<double, 1>::type const &a, typename pack<double, 1>::type const &b){ return a * b; }
//! \brief Calculate single-precision complex squared modulus
inline typename pack<float, 1>::type mm_complex_sq_mod(typename pack<float,1>::type const &a){ return norm(a); }
//! \brief Calculate double-precision complex squared modulus
inline typename pack<double, 1>::type mm_complex_sq_mod(typename pack<double,1>::type const &a){ return norm(a); }
//! \brief Calculate single-precision complex modulus
inline float mm_complex_mod(typename pack<float,1>::type const &a){ return std::abs(a); }
//! \brief Calculate double-precision complex modulus
inline double mm_complex_mod(typename pack<double,1>::type const &a){ return std::abs(a); }
//! \brief Calculate single-precision complex conjugation
inline typename pack<float, 1>::type mm_complex_conj(typename pack<float,1>::type const &a){ return conj(a); }
//! \brief Calculate double-precision complex conjugation
inline typename pack<double, 1>::type mm_complex_conj(typename pack<double,1>::type const &a){ return conj(a); }
//! \brief For a single-precision complex type, perform division
inline typename pack<float, 1>::type mm_complex_div(typename pack<float, 1>::type const &a, typename pack<float, 1>::type const &b){ return a / b; }
//! \brief For a double-precision complex type, perform division
inline typename pack<double, 1>::type mm_complex_div(typename pack<double, 1>::type const &a, typename pack<double, 1>::type const &b){ return a / b; }

/////////////////////////////////////////////
/* Below is functionality for vector packs */
/////////////////////////////////////////////

#ifdef __AVX__

//! \brief Alias for vector pack of 2 elements, double precision
template<> struct pack<double, 2> { using type = __m128d; };
//! \brief Alias for vector pack of 4 elements, single precision
template<> struct pack<float, 4> { using type = __m128; };
//! \brief Alias for vector pack of 4 elements, double precision
template<> struct pack<double, 4> { using type = __m256d; };
//! \brief Alias for vector pack of 8 elements, single precision
template<> struct pack<float, 8> { using type = __m256; };

//////////////////////////////////////////
/* Below are structs for pack<float, 4> */
//////////////////////////////////////////

//! \brief Sets the zero if there are two pairs of single precision complex numbers
template<>
inline typename pack<float, 4>::type  mm_zero<float, 4>(){ return _mm_setzero_ps(); }

//! \brief Loads from a pointer to at least 4 floats into a vectorized type
template<>
inline typename pack<float, 4>::type mm_load<float, 4>(float const *src) { return _mm_loadu_ps(src); }

//! \brief Stores four floats from a vectorized type in a pointer of floats
template<>
inline void mm_store<float, 4>(float *dest, pack<float, 4>::type const &src) { _mm_storeu_ps(dest, src); }

//! \brief Stores pair of floats into vectorized type
template<>
inline typename pack<float, 4>::type mm_pair_set<float, 4>(float x, float y) { return _mm_setr_ps(x, y, x, y); }

//! \brief Sets a vectorized type as a repeated float
template<>
inline typename pack<float, 4>::type mm_set1<float,4> (float x) { return _mm_set1_ps(x); }

//! \brief Creates a vectorized type from strided pointer to two 32-bit floating point complex numbers
template<>
inline typename pack<float, 4>::type mm_complex_load<float,4>(std::complex<float> const *src, int stride) {
    return _mm_setr_ps(src[0].real(), src[0].imag(), src[stride].real(), src[stride].imag());
}
//! \brief Creates a vectorized type from pointer to two 32-bit floating point complex numbers
template<>
inline typename pack<float, 4>::type mm_complex_load<float,4>(std::complex<float> const *src) {
    return mm_complex_load<float,4>(src, 1);
}

//////////////////////////////////////////
/* Below are structs for pack<float, 8> */
//////////////////////////////////////////

//! \brief Sets the zero if there are four pairs of single precision complex numbers
template<>
inline typename pack<float, 8>::type mm_zero<float, 8>(){ return _mm256_setzero_ps(); }

//! \brief Loads from a pointer to at least 8 floats into a vectorized type
template<>
inline typename pack<float, 8>::type mm_load<float, 8>(float const *src) { return _mm256_loadu_ps(src); }

//! \brief Stores four floats from a vectorized type in a pointer of floats
template<>
inline void mm_store<float, 8>(float *dest, pack<float, 8>::type const &src) { _mm256_storeu_ps(dest, src); }

//! \brief Stores pair of floats into vectorized type
template<>
inline typename pack<float, 8>::type mm_pair_set<float, 8>(float x, float y) { return _mm256_setr_ps(x, y, x, y, x, y, x, y); }

//! \brief Sets a vectorized type as a repeated float
template<>
inline typename pack<float, 8>::type mm_set1<float,8> (float x) { return _mm256_set1_ps(x); }

//! \brief Creates a vectorized type from strided pointer to four 32-bit floating point complex numbers
template<>
inline typename pack<float, 8>::type mm_complex_load<float, 8>(std::complex<float> const *src, int stride) {
    return _mm256_setr_ps(src[0*stride].real(), src[0*stride].imag(),
                          src[1*stride].real(), src[1*stride].imag(),
                          src[2*stride].real(), src[2*stride].imag(),
                          src[3*stride].real(), src[3*stride].imag());
}
//! \brief Creates a vectorized type from pointer to four 32-bit floating point complex numbers
template<>
inline typename pack<float, 8>::type mm_complex_load<float,8>(std::complex<float> const *src) {
    return mm_complex_load<float,8>(src, 1);
}

///////////////////////////////////////////
/* Below are structs for pack<double, 2> */
///////////////////////////////////////////

//! \brief Sets the zero if there is one pair of double precision complex numbers
template<>
inline typename pack<double, 2>::type mm_zero<double, 2>(){ return _mm_setzero_pd(); }

//! \brief Loads from a pointer to at least 2 doubles into a vectorized type
template<>
inline typename pack<double, 2>::type mm_load<double, 2>(double const *src) { return _mm_loadu_pd(src); }

//! \brief Stores two doubles from a vectorized type in a pointer of doubles
template<>
inline void mm_store<double, 2>(double *dest, pack<double, 2>::type const &src) { _mm_storeu_pd(dest, src); }

//! \brief Stores pair of doubles into vectorized type
template<>
inline typename pack<double, 2>::type mm_pair_set<double, 2>(double x, double y) { return _mm_setr_pd(x, y); }

//! \brief Sets a vectorized type as a repeated double
template<>
inline typename pack<double, 2>::type mm_set1<double, 2>(double x) { return _mm_set1_pd(x); }

//! \brief Creates a vectorized type from pointer to at least one double precision complex number
template<>
inline typename pack<double,2>::type mm_complex_load<double,2>(std::complex<double> const *src, int) {
    return _mm_setr_pd(src[0].real(), src[0].imag());
}
template<>
inline typename pack<double,2>::type mm_complex_load<double,2>(std::complex<double> const *src) {
    return mm_complex_load<double,2>(src, 1);
}

///////////////////////////////////////////
/* Below are structs for pack<double, 4> */
///////////////////////////////////////////

//! \brief Sets the zero if there is one pair of double precision complex numbers
template<>
inline typename pack<double, 4>::type mm_zero<double, 4>(){ return _mm256_setzero_pd(); }

//! \brief Loads from a pointer to at least 4 doubles into a vectorized type
template<>
inline typename pack<double, 4>::type mm_load<double, 4>(double const *src) { return _mm256_loadu_pd(src); }

//! \brief Stores two doubles from a vectorized type in a pointer of doubles
template<>
inline void mm_store<double, 4>(double *dest, pack<double, 4>::type const &src) { _mm256_storeu_pd(dest, src); }

//! \brief Stores 4 doubles into vectorized type
template<>
inline typename pack<double, 4>::type mm_pair_set<double, 4>(double x, double y) { return _mm256_setr_pd(x, y, x, y); }

//! \brief Sets a vectorized type as a repeated double
template<>
inline typename pack<double, 4>::type mm_set1<double, 4>(double x) { return _mm256_set1_pd(x); }

//! \brief Creates a vectorized type from strided pointer to at least two double-precision complex numbers
template<>
inline typename pack<double,4>::type mm_complex_load<double,4>(std::complex<double> const *src, int stride) {
    return _mm256_setr_pd(src[0].real(), src[0].imag(), src[stride].real(), src[stride].imag());
}
//! \brief Creates a vectorized type from pointer to at least two double-precision complex numbers
template<>
inline typename pack<double,4>::type mm_complex_load<double,4>(std::complex<double> const *src) {
    return mm_complex_load<double,4>(src, 1);
}

///////////////////////////////////////////////////
/* Elementary binary operations for vector packs */
///////////////////////////////////////////////////

/* Addition */

//! \brief Perform addition on vectorized packs of four floats
inline pack<float, 4>::type mm_add(pack<float, 4>::type const &x,pack<float, 4>::type const &y) {
    return _mm_add_ps(x, y);
}

//! \brief Perform addition on vectorized packs of eight floats
inline pack<float, 8>::type mm_add(pack<float, 8>::type const &x, pack<float, 8>::type const &y) {
    return _mm256_add_ps(x, y);
}

//! \brief Perform addition on vectorized packs of two doubles
inline pack<double, 2>::type mm_add(pack<double, 2>::type const &x, pack<double, 2>::type const &y) {
    return _mm_add_pd(x, y);
}

//! \brief Perform addition on vectorized packs of four doubles
inline pack<double, 4>::type mm_add(pack<double, 4>::type const &x, pack<double, 4>::type const &y) {
    return _mm256_add_pd(x, y);
}

/* Subtraction */

//! \brief Perform subtraction on vectorized packs of four floats
inline pack<float, 4>::type mm_sub(pack<float, 4>::type const &x,pack<float, 4>::type const &y) {
    return _mm_sub_ps(x, y);
}

//! \brief Perform subtraction on vectorized packs of eight floats
inline pack<float, 8>::type mm_sub(pack<float, 8>::type const &x, pack<float, 8>::type const &y) {
    return _mm256_sub_ps(x, y);
}

//! \brief Perform subtraction on vectorized packs of two doubles
inline pack<double, 2>::type mm_sub(pack<double, 2>::type const &x, pack<double, 2>::type const &y) {
    return _mm_sub_pd(x, y);
}

//! \brief Perform subtraction on vectorized packs of four doubles
inline pack<double, 4>::type mm_sub(pack<double, 4>::type const &x, pack<double, 4>::type const &y) {
    return _mm256_sub_pd(x, y);
}

/* Multiplication */

//! \brief Perform multiplication on vectorized packs of four floats
inline pack<float, 4>::type mm_mul(pack<float, 4>::type const &x, pack<float, 4>::type const &y) {
    return _mm_mul_ps(x, y);
}

//! \brief Perform multiplication on vectorized packs of eight floats
inline pack<float, 8>::type mm_mul(pack<float, 8>::type const &x, pack<float, 8>::type const &y) {
    return _mm256_mul_ps(x, y);
}

//! \brief Perform multiplication on vectorized packs of two doubles
inline pack<double, 2>::type mm_mul(pack<double, 2>::type const &x, pack<double, 2>::type const &y) {
    return _mm_mul_pd(x, y);
}

//! \brief Perform multiplication on vectorized packs of four doubles
inline pack<double, 4>::type mm_mul(pack<double, 4>::type const &x, pack<double, 4>::type const &y) {
    return _mm256_mul_pd(x, y);
}

/* Division */

//! \brief Perform division on vectorized packs of four floats
inline pack<float, 4>::type mm_div(pack<float, 4>::type const &x,pack<float, 4>::type const &y) {
    return _mm_div_ps(x, y);
}

//! \brief Perform division on vectorized packs of eight floats
inline pack<float, 8>::type mm_div(pack<float, 8>::type const &x, pack<float, 8>::type const &y) {
    return _mm256_div_ps(x, y);
}

//! \brief Perform division on vectorized packs of two doubles
inline pack<double, 2>::type mm_div(pack<double, 2>::type const &x, pack<double, 2>::type const &y) {
    return _mm_div_pd(x, y);
}

//! \brief Perform division on vectorized packs of four doubles
inline pack<double, 4>::type mm_div(pack<double, 4>::type const &x, pack<double, 4>::type const &y) {
    return _mm256_div_pd(x, y);
}


///////////////////////////////////////////
/* Complex operations using vector packs */
///////////////////////////////////////////

// Complex Multiplication

//! \brief Complex multiply two pairs of floats
inline pack<float,4>::type mm_complex_mul(pack<float, 4>::type const &x, pack<float, 4>::type const &y) {
    typename pack<float,4>::type cc = _mm_permute_ps(y, 0b10100000);
    typename pack<float,4>::type ba = _mm_permute_ps(x, 0b10110001);
    typename pack<float,4>::type dd = _mm_permute_ps(y, 0b11110101);
    typename pack<float,4>::type dba = _mm_mul_ps(ba, dd);
    typename pack<float,4>::type mult = _mm_fmaddsub_ps(x, cc, dba);
    return mult;
}

//! \brief Complex multiply four pairs of floats
inline pack<float, 8>::type mm_complex_mul(pack<float, 8>::type const &x, pack<float, 8>::type const &y) {
    typename pack<float,8>::type cc = _mm256_permute_ps(y, 0b10100000);
    typename pack<float,8>::type ba = _mm256_permute_ps(x, 0b10110001);
    typename pack<float,8>::type dd = _mm256_permute_ps(y, 0b11110101);
    typename pack<float,8>::type dba = _mm256_mul_ps(ba, dd);
    typename pack<float,8>::type mult = _mm256_fmaddsub_ps(x, cc, dba);
    return mult;
}

//! \brief Complex multiply one pair of doubles
inline pack<double, 2>::type mm_complex_mul(pack<double, 2>::type const &x, pack<double, 2>::type const &y) {
    typename pack<double,2>::type cc = _mm_permute_pd(y, 0);
    typename pack<double,2>::type ba = _mm_permute_pd(x, 0b01);
    typename pack<double,2>::type dd = _mm_permute_pd(y, 0b11);
    typename pack<double,2>::type dba = _mm_mul_pd(ba, dd);
    typename pack<double,2>::type mult = _mm_fmaddsub_pd(x, cc, dba);
    return mult;
}

//! \brief Complex multiply two pairs of doubles
inline pack<double, 4>::type mm_complex_mul(pack<double, 4>::type const &x, pack<double, 4>::type const &y) {
    typename pack<double,4>::type cc = _mm256_permute_pd(y, 0b0000);
    typename pack<double,4>::type ba = _mm256_permute_pd(x, 0b0101);
    typename pack<double,4>::type dd = _mm256_permute_pd(y, 0b1111);
    typename pack<double,4>::type dba = _mm256_mul_pd(ba, dd);
    typename pack<double,4>::type mult = _mm256_fmaddsub_pd(x, cc, dba);
    return mult;
}

// Squared modulus of the complex numbers in a pack

//! \brief Squared modulus of two single precision complex numbers in a pack
inline pack<float, 4>::type mm_complex_sq_mod(pack<float, 4>::type const &x) {
    return _mm_or_ps(_mm_dp_ps(x, x, 0b11001100), _mm_dp_ps(x, x, 0b00110011));
}

//! \brief Squared modulus of four single precision complex numbers in a pack
inline pack<float, 8>::type mm_complex_sq_mod(pack<float, 8>::type const &x) {
    return _mm256_or_ps(_mm256_dp_ps(x, x, 0b11001100), _mm256_dp_ps(x, x, 0b00110011));
}

//! \brief Squared modulus of one double precision complex number in a pack
inline pack<double, 2>::type mm_complex_sq_mod(pack<double, 2>::type const &x) {
    return _mm_dp_pd(x, x, 0b11111111);
}

//! \brief Squared modulus of two double precision complex numbers in a pack
inline pack<double, 4>::type mm_complex_sq_mod(pack<double, 4>::type const &x) {
    typename pack<double,4>::type a = _mm256_mul_pd(x, x);
    return _mm256_hadd_pd(a, a);
}

// Moduli (with square root) of complex numbers

//! \brief Moduli of two single precision complex numbers in a pack
inline pack<float, 4>::type mm_complex_mod(pack<float, 4>::type const &x) {
    return _mm_sqrt_ps(mm_complex_sq_mod(x));
}

//! \brief Moduli of four single precision complex numbers in a pack
inline pack<float, 8>::type mm_complex_mod(pack<float, 8>::type const &x) {
    return _mm256_sqrt_ps(mm_complex_sq_mod(x));
}

//! \brief Modulus of one double precision complex number in a pack
inline pack<double, 2>::type mm_complex_mod(pack<double, 2>::type const &x) {
    return _mm_sqrt_pd(mm_complex_sq_mod(x));
}

//! \brief Moduli of two double precision complex numbers in a pack
inline pack<double, 4>::type mm_complex_mod(pack<double, 4>::type const &x) {
    return _mm256_sqrt_pd(mm_complex_sq_mod(x));
}

//! \brief Conjugate two single precision complex numbers
inline pack<float, 4>::type mm_complex_conj(pack<float, 4>::type const &x) {
    return _mm_blend_ps(x, -x, 0b1010);
}

//! \brief Conjugate four single precision complex numbers
inline pack<float, 8>::type mm_complex_conj(pack<float, 8>::type const &x) {
    return _mm256_blend_ps(x, -x, 0b10101010);
}

//! \brief Conjugate two double precision complex numbers
inline pack<double, 2>::type mm_complex_conj(pack<double, 2>::type const &x) {
    return _mm_blend_pd(x, -x, 0b10);
}

//! \brief Conjugate four double precision complex numbers
inline pack<double, 4>::type mm_complex_conj(pack<double, 4>::type const &x) {
    return _mm256_blend_pd(x, -x, 0b1010);
}

// Complex division

//! \brief Divide x by y, where x and y are each 2 single precision complex numbers
inline pack<float, 4>::type mm_complex_div(pack<float, 4>::type const &x, pack<float, 4>::type const &y) {
    return _mm_div_ps(mm_complex_mul(x, mm_complex_conj(y)), mm_complex_sq_mod(y));
}

//! \brief Divide x by y, where x and y are each 4 single precision complex numbers
inline pack<float, 8>::type mm_complex_div(pack<float, 8>::type const &x, pack<float, 8>::type const &y) {
    return _mm256_div_ps(mm_complex_mul(x, mm_complex_conj(y)), mm_complex_sq_mod(y));
}

//! \brief Divide x by y, where x and y are each 1 double precision complex number
inline pack<double, 2>::type mm_complex_div(pack<double, 2>::type const &x, pack<double, 2>::type const &y) {
    return _mm_div_pd(mm_complex_mul(x, mm_complex_conj(y)), mm_complex_sq_mod(y));
}

//! \brief Divide x by y, where x and y are each 2 double precision complex numbers
inline pack<double, 4>::type mm_complex_div(pack<double, 4>::type const &x, pack<double, 4>::type const &y) {
    return _mm256_div_pd(mm_complex_mul(x, mm_complex_conj(y)), mm_complex_sq_mod(y));
}

#endif // __AVX__

}
}

#endif // HEFFTE_STOCK_VEC_TYPES_H
