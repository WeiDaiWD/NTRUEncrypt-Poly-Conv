
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
