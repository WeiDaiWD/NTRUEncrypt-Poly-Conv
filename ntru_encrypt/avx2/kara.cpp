#include "simd_poly.h"

void
__mm256i_karatsuba__mm256_toom4(
    uint16_t        *r, /* out - a * b in Z[x], must be length 2n */
    uint16_t        *t, /*  in - n coefficients of scratch space */
    uint16_t const  *a, /*  in - polynomial */
    uint16_t const  *b, /*  in - polynomial */
    uint16_t const   n) /*  in - number of coefficients in a and b */
{
    if (n < 384)
    {
        __mm256i_toom4__mm256i_toom3(r, t, a, b, n);
        return;
    }
    if (n > 768)
    {
        printf("degree %d exceeds the maximum (384) allowed\n", n);
        return;
    }
    uint16_t i;
    uint16_t s = 384;
    uint16_t const *a1 = a+s;
    uint16_t const *b1 = b+s;
    uint16_t *t1 = t+s;
    uint16_t *r1 = r+s, *r2 = r+2*s, *r3 = r+3*s;
    uint16_t *buf = t+s*2;


    __m256i m[6];
    for (i=0; i<s; i+=16) {
      m[0] = _mm256_loadu_si256((__m256i *)(a+i));
      m[1] = _mm256_loadu_si256((__m256i *)(a1+i));
      m[2] = _mm256_loadu_si256((__m256i *)(b1+i));
      m[3] = _mm256_loadu_si256((__m256i *)(b+i));
      m[0] = _mm256_sub_epi16(m[0], m[1]);
      m[1] = _mm256_sub_epi16(m[2], m[3]);
      _mm256_storeu_si256((__m256i *)(r+i), m[0]);
      _mm256_storeu_si256((__m256i *)(r1+i), m[1]);
    }

    __mm256i_toom4__mm256i_toom3(t,  buf, r, r1, s);
    __mm256i_toom4__mm256i_toom3(r2,  buf, a1, b1, s);

    for (i=0; i<s; i+=16) {
      m[0] = _mm256_loadu_si256((__m256i *)(r2+i));
      m[1] = _mm256_loadu_si256((__m256i *)(t+i));
      m[2] = _mm256_loadu_si256((__m256i *)(r3+i));
      m[3] = _mm256_loadu_si256((__m256i *)(t1+i));
      m[1] = _mm256_add_epi16(m[1], m[0]);
      m[2] = _mm256_add_epi16(m[2], m[0]);
      m[2] = _mm256_add_epi16(m[2], m[3]);
      _mm256_storeu_si256((__m256i *)(r1+i), m[1]);
      _mm256_storeu_si256((__m256i *)(r2+i), m[2]);
    }

    __mm256i_toom4__mm256i_toom3(t,  buf, a, b, s);

    for (i=0; i<s; i++) {
      r[i] = t[i];
    }
    for (i=0; i<s; i+=16) {
      m[0] = _mm256_loadu_si256((__m256i *)(r1+i));
      m[1] = _mm256_loadu_si256((__m256i *)(t+i));
      m[2] = _mm256_loadu_si256((__m256i *)(t1+i));
      m[3] = _mm256_loadu_si256((__m256i *)(r2+i));
      m[0] = _mm256_add_epi16(m[0], m[1]);
      m[0] = _mm256_add_epi16(m[0], m[2]);
      m[3] = _mm256_add_epi16(m[3], m[2]);
      _mm256_storeu_si256((__m256i *)(r1+i), m[0]);
      _mm256_storeu_si256((__m256i *)(r2+i), m[3]);
    }

    return;
}
