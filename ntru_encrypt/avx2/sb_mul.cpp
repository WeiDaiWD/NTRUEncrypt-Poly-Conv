
#include "simd_poly.h"



void
grade_school_mul(
    uint16_t        *res1,  /* out - a * b in Z[x], must be length 2N */
    uint16_t const  *a,     /*  in - polynomial */
    uint16_t const  *b,     /*  in - polynomial */
    uint16_t const   N)     /*  in - number of coefficients in a and b */
{
    uint16_t i;
    uint16_t j;

    for(j=0; j<N; j++)
    {
        res1[j] = a[0]*b[j];
    }
    for(i=1; i<N; i++)
    {
        res1[i+N-1] = 0;
        for(j=0; j<N; j++)
        {
            res1[i+j] += a[i]*b[j];
        }
    }
    res1[2*N-1] = 0;

    return;
}

void
__m256i_grade_school_mul_16(
    uint16_t        *res1,  /* out - a * b in Z[x], must be length 2N */
    uint16_t        *buf,   /* buf size >= 32 bytes */
    uint16_t const  *a,     /*  in - polynomial */
    uint16_t const  *b,     /*  in - polynomial */
    uint16_t const   N)     /*  in - number of coefficients in a and b <= 16*/
{
    uint16_t i;
    uint16_t j;
    uint16_t n;
    uint16_t *buf1;
    memset(buf, 0, 32*sizeof(uint16_t));
    memcpy(buf, a, N*sizeof(uint16_t));
    buf1 = buf + 16;
    __m256i a256low;
    __m256i a256high;
    __m256i r256low;
    __m256i r256high;
    __m256i tmp;

    a256low = _mm256_loadu_si256((__m256i *)(buf));
    r256high = _mm256_set1_epi16(0);
    tmp = _mm256_set1_epi16(b[0]);
    r256low = _mm256_mullo_epi16 (tmp,a256low);


    for (i=1;i<N;i++)
    {
        tmp = _mm256_set1_epi16(b[i]);
        memcpy(buf+i, a, N*sizeof(uint16_t));       // shifting
        memset(buf, 0, i*sizeof(uint16_t));
        a256low = _mm256_loadu_si256((__m256i *)(buf));
        a256high = _mm256_loadu_si256((__m256i *)(buf1));
        r256low =_mm256_add_epi16 (r256low, _mm256_mullo_epi16 (tmp,a256low));
        r256high = _mm256_add_epi16 (r256high,_mm256_mullo_epi16 (tmp,a256high));
    }
    _mm256_storeu_si256((__m256i *)(res1+16), r256high);
    _mm256_storeu_si256((__m256i *)res1, (r256low));

    return;
}

void
__m256i_grade_school_mul_32(
    uint16_t        *res1,  /* out - a * b in Z[x], must be length 2N */
    uint16_t        *buf,   /* buf size >= 64 bytes */
    uint16_t const  *a,     /*  in - polynomial */
    uint16_t const  *b,     /*  in - polynomial */
    uint16_t const   N)     /*  in - number of coefficients in a and b <= 32*/
{
    uint16_t i;
    uint16_t j;
    uint16_t n;
    uint16_t sizen = N*sizeof(uint16_t);
    uint16_t *buf1, *buf2, *buf3;
    memset(buf, 0, sizen*2);
    memcpy(buf, a, sizen);

    buf1 = buf + 16;
    buf2 = buf + 32;
    buf3 = buf + 48;

    __m256i r256[4];
    __m256i tmp, tmp2;

    r256[2] = _mm256_set1_epi16(0);
    r256[3] = _mm256_set1_epi16(0);

    tmp     = _mm256_set1_epi16(b[0]);
    tmp2    = _mm256_loadu_si256((__m256i *)(buf));
    r256[0] = _mm256_mullo_epi16 (tmp,tmp2);

    tmp2    = _mm256_loadu_si256((__m256i *)(buf1));
    r256[1] = _mm256_mullo_epi16 (tmp,tmp2);

    for (i=1;i<16;i++)
    {
        tmp     = _mm256_set1_epi16(b[i]);

        memmove (buf+i,buf+i-1,sizen);
        buf[i-1]= 0;

        tmp2    = _mm256_loadu_si256((__m256i *)(buf));
        r256[0] = _mm256_add_epi16 (r256[0], _mm256_mullo_epi16 (tmp,tmp2));

        tmp2    = _mm256_loadu_si256((__m256i *)(buf1));
        r256[1] = _mm256_add_epi16 (r256[1], _mm256_mullo_epi16 (tmp,tmp2));

        tmp2    = _mm256_loadu_si256((__m256i *)(buf2));
        r256[2] = _mm256_add_epi16 (r256[2], _mm256_mullo_epi16 (tmp,tmp2));
    }

    tmp     = _mm256_set1_epi16(b[16]);
    memmove (buf+16,buf+15,sizen);
    buf[15] = 0;

    tmp2    = _mm256_loadu_si256((__m256i *)(buf1));
    r256[1] = _mm256_add_epi16 (r256[1], _mm256_mullo_epi16 (tmp,tmp2));

    tmp2    = _mm256_loadu_si256((__m256i *)(buf2));
    r256[2] = _mm256_add_epi16 (r256[2], _mm256_mullo_epi16 (tmp,tmp2));


    for (i=17;i<32;i++)
    {
        tmp     = _mm256_set1_epi16(b[i]);

        memmove (buf+i,buf+i-1,sizen);
        buf[i-1]= 0;

        tmp2    = _mm256_loadu_si256((__m256i *)(buf1));
        r256[1] = _mm256_add_epi16 (r256[1], _mm256_mullo_epi16 (tmp,tmp2));

        tmp2    = _mm256_loadu_si256((__m256i *)(buf2));
        r256[2] = _mm256_add_epi16 (r256[2], _mm256_mullo_epi16 (tmp,tmp2));

        tmp2    = _mm256_loadu_si256((__m256i *)(buf3));
        r256[3] = _mm256_add_epi16 (r256[3], _mm256_mullo_epi16 (tmp,tmp2));
    }



    _mm256_storeu_si256((__m256i *)res1, r256[0]);
    _mm256_storeu_si256((__m256i *)res1+1, r256[1]);
    _mm256_storeu_si256((__m256i *)res1+2, r256[2]);
    _mm256_storeu_si256((__m256i *)res1+3, r256[3]);
    return;
}

void
__m256i_grade_school_mul_32_old(
    uint16_t        *res1,  /* out - a * b in Z[x], must be length 2N */
    uint16_t        *buf,   /* buf size >= 64 bytes */
    uint16_t const  *a,     /*  in - polynomial */
    uint16_t const  *b,     /*  in - polynomial */
    uint16_t const   N)     /*  in - number of coefficients in a and b <= 32*/
{
    uint16_t i;
    uint16_t j;
    uint16_t n;
    uint16_t *buf1, *buf2, *buf3;
    memset(buf, 0, 64*sizeof(uint16_t));
    memcpy(buf, a, N*sizeof(uint16_t));
    buf1 = buf + 16;
    buf2 = buf + 32;
    buf3 = buf + 48;

    __m256i a256[4];
    __m256i r256[4];
    __m256i tmp;

    a256[0] = _mm256_loadu_si256((__m256i *)(buf));
    a256[1] = _mm256_loadu_si256((__m256i *)(buf1));

    tmp = _mm256_set1_epi16(b[0]);
    r256[0] = _mm256_mullo_epi16 (tmp,a256[0]);
    r256[1] = _mm256_mullo_epi16 (tmp,a256[1]);
    r256[2] = _mm256_set1_epi16(0);
    r256[3] = _mm256_set1_epi16(0);

    for (i=1;i<N;i++)
    {
        tmp = _mm256_set1_epi16(b[i]);
        memcpy(buf+i, a, N*sizeof(uint16_t));       // shifting
        memset(buf, 0, i*sizeof(uint16_t));
        a256[0] = _mm256_loadu_si256((__m256i *)(buf));
        a256[1] = _mm256_loadu_si256((__m256i *)(buf1));
        a256[2] = _mm256_loadu_si256((__m256i *)(buf2));
        a256[3] = _mm256_loadu_si256((__m256i *)(buf3));
        r256[0] = _mm256_add_epi16 (r256[0], _mm256_mullo_epi16 (tmp,a256[0]));
        r256[1] = _mm256_add_epi16 (r256[1], _mm256_mullo_epi16 (tmp,a256[1]));
        r256[2] = _mm256_add_epi16 (r256[2], _mm256_mullo_epi16 (tmp,a256[2]));
        r256[3] = _mm256_add_epi16 (r256[3], _mm256_mullo_epi16 (tmp,a256[3]));
   }

    _mm256_storeu_si256((__m256i *)res1, r256[0]);
    _mm256_storeu_si256((__m256i *)(res1+16), r256[1]);
    _mm256_storeu_si256((__m256i *)(res1+32), r256[2]);
    _mm256_storeu_si256((__m256i *)(res1+48), r256[3]);
    return;
}
