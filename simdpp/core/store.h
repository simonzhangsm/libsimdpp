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

#ifndef LIBSIMDPP_SIMDPP_CORE_STORE_H
#define LIBSIMDPP_SIMDPP_CORE_STORE_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/types.h>
#include <simdpp/null/memory.h>
#include <simdpp/detail/align.h>

namespace simdpp {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace SIMDPP_ARCH_NAMESPACE {
#endif


/// @{
/** Stores a 128-bit or 256-bit integer vector to an aligned memory location.

    @par 128-bit version:

    @code
    *(p) = a[0..127]
    @endcode
    @a p must be aligned to 16 bytes.

    @par 256-bit version:

    @code
    *(p) = a[0..255]
    @endcode
    @a p must be aligned to 32 bytes.
    @icost{SSE2-SSE4.1, NEON, ALTIVEC, 2}
    @icost{AVX (integer vectors), 2}
*/
inline void store(void* p, gint8x16 a)
{
    p = detail::assume_aligned(p, 16);
#if SIMDPP_USE_NULL
    null::store(p, a);
#elif SIMDPP_USE_SSE2
    _mm_store_si128(reinterpret_cast<__m128i*>(p), a);
#elif SIMDPP_USE_NEON
    vst1q_u64(reinterpret_cast<uint64_t*>(p), vreinterpretq_u64_u32(a));
#elif SIMDPP_USE_ALTIVEC
    vec_stl((__vector uint8_t)a, 0, reinterpret_cast<uint8_t*>(p));
#endif
}

#if SIMDPP_USE_AVX2
inline void store(void* p, gint8x32 a)
{
    p = detail::assume_aligned(p, 32);
    _mm256_store_si256(reinterpret_cast<__m256i*>(p), a);
}
#endif

template<unsigned N>
void store(void* p, gint8<N> a)
{
    unsigned veclen = sizeof(typename gint8<N>::base_vector_type);

    p = detail::assume_aligned(p, veclen);
    char* q = reinterpret_cast<char*>(p);
    for (unsigned i = 0; i < gint8<N>::vec_length; ++i) {
        store(q, a[i]);
        q += veclen;
    }
}

template<unsigned N>
void store(void* p, gint16<N> a) { store(p, gint8<N*2>(a)); }
template<unsigned N>
void store(void* p, gint32<N> a) { store(p, gint8<N*4>(a)); }
template<unsigned N>
void store(void* p, gint64<N> a) { store(p, gint8<N*8>(a)); }

inline void store(float *p, float32x4 a)
{
    p = detail::assume_aligned(p, 16);
#if SIMDPP_USE_NULL
    null::store(p, a);
#elif SIMDPP_USE_SSE2
    _mm_store_ps(p, a);
#elif SIMDPP_USE_NEON
    vst1q_f32(p, a);
#elif SIMDPP_USE_ALTIVEC
    vec_stl((__vector float)a, 0, p);
#endif
}

#if SIMDPP_USE_AVX
inline void store(float* p, float32x8 a)
{
    p = detail::assume_aligned(p, 32);
    _mm256_store_ps(p, a);
}
#endif

template<unsigned N>
void store(float* p, float32<N> a)
{
    unsigned veclen = sizeof(typename float32<N>::base_vector_type);

    p = detail::assume_aligned(p, veclen);
    for (unsigned i = 0; i < gint8<N>::vec_length; ++i) {
        store(p, a[i]);
        p += veclen / sizeof(float);
    }
}

inline void store(double *p, float64x2 a)
{
    p = detail::assume_aligned(p, 16);
#if SIMDPP_USE_NULL || SIMDPP_USE_NEON || SIMDPP_USE_ALTIVEC
    null::store(p, a);
#elif SIMDPP_USE_SSE2
    _mm_store_pd(p, a);
#endif
}

#if SIMDPP_USE_AVX
inline void store(double* p, float64x4 a)
{
    p = detail::assume_aligned(p, 32);
    _mm256_store_pd(p, a);
}
#endif

template<unsigned N>
void store(double* p, float64<N> a)
{
    unsigned veclen = sizeof(typename float64<N>::base_vector_type);

    p = detail::assume_aligned(p, veclen);
    for (unsigned i = 0; i < float64<N>::vec_length; ++i) {
        store(p, a[i]);
        p += veclen / sizeof(double);
    }
}

/// @}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
} // namespace SIMDPP_ARCH_NAMESPACE
#endif
} // namespace simdpp

#endif

