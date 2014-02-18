/*  libsimdpp
    Copyright (C) 2013  Povilas Kanapickas povilas@radix.lt
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LIBSIMDPP_SIMDPP_CORE_F_ISNAN2_H
#define LIBSIMDPP_SIMDPP_CORE_F_ISNAN2_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/types.h>
#include <simdpp/core/bit_or.h>
#include <simdpp/core/f_isnan.h>
#include <simdpp/null/math.h>

namespace simdpp {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace SIMDPP_ARCH_NAMESPACE {
#endif

/// @{
/** Checks whether corresponding elements in either @a a or @a b are IEEE754 NaN.

    @code
    r0 = isnan(a0) || isnan(b0) ? 0xffffffff : 0
    ...
    rN = isnan(aN) || isnan(bN) ? 0xffffffff : 0
    @endcode

    @par 128-bit version:
    @icost{NEON, ALTIVEC, 3}

    @par 256-bit version:
    @icost{SSE2-SSE4.1, 2}
    @icost{NEON, ALTIVEC, 6}
*/
inline mask_float32x4 isnan2(float32x4 a, float32x4 b)
{
#if SIMDPP_USE_NULL
    return null::isnan2(a, b);
#elif SIMDPP_USE_AVX
    return _mm_cmp_ps(a, b, _CMP_UNORD_Q);
#elif SIMDPP_USE_SSE2
    return (mask_float32x4) _mm_cmpunord_ps(a, b);
#elif SIMDPP_USE_NEON || SIMDPP_USE_ALTIVEC
    return bit_or(isnan(a), isnan(b));
#endif
}

#if SIMDPP_USE_AVX
inline mask_float32x8 isnan2(float32x8 a, float32x8 b)
{
    return _mm256_cmp_ps(a, b, _CMP_UNORD_Q);
}
#endif

template<unsigned N>
mask_float32<N> isnan2(float32<N> a, float32<N> b)
{
    SIMDPP_VEC_ARRAY_IMPL2(mask_float32<N>, isnan2, a, b);
}
/// @}

/// @{
/** Checks whether corresponding elements in either @a a or @a b are IEEE754
    NaN.

    @code
    r0 = isnan(a0) || isnan(b0) ? 0xffffffffffffffff : 0
    ...
    rN = isnan(aN) || isnan(bN) ? 0xffffffffffffffff : 0
    @endcode

    @par 128-bit version:
    @novec{NEON, ALTIVEC}

    @par 256-bit version:
    @novec{NEON, ALTIVEC}
    @icost{SSE2-SSE4.1, 2}
*/
inline mask_float64x2 isnan2(float64x2 a, float64x2 b)
{
#if SIMDPP_USE_NULL || SIMDPP_USE_NEON || SIMDPP_USE_ALTIVEC
    return null::isnan2(a, b);
#elif SIMDPP_USE_AVX
    return _mm_cmp_pd(a, b, _CMP_UNORD_Q);
#elif SIMDPP_USE_SSE2
    return _mm_cmpunord_pd(a, b);
#endif
}

#if SIMDPP_USE_AVX
inline mask_float64x4 isnan2(float64x4 a, float64x4 b)
{
    return _mm256_cmp_pd(a, b, _CMP_UNORD_Q);
}
#endif

template<unsigned N>
mask_float64<N> isnan2(float64<N> a, float64<N> b)
{
    SIMDPP_VEC_ARRAY_IMPL2(mask_float64<N>, isnan2, a, b);
}
/// @}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
} // namespace SIMDPP_ARCH_NAMESPACE
#endif
} // namespace simdpp

#endif

