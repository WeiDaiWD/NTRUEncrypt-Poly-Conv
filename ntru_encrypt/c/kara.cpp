#include "simd_poly.h"


/*static int i;
#define s (n/2)
#define a1 (a+s)
#define b1 (b+s)
#define t1 (t+s)
#define r1 (r+s)
#define r2 (r+2*s)
#define r3 (r+3*s)
*/

static int count = 0;

void
karatsuba_toom4(
    uint16_t        *r, /* out - a * b in Z[x], must be length 2n */
    uint16_t        *t, /*  in - n coefficients of scratch space */
    uint16_t const  *a, /*  in - polynomial */
    uint16_t const  *b, /*  in - polynomial */
    uint16_t const   n) /*  in - number of coefficients in a and b */

{
    if (n < 32)
    {
        grade_school_mul(r, a, b, n);
        return;
    }
    int i;
    uint16_t s = n/2;
    uint16_t const *a1 = a+s;
    uint16_t const *b1 = b+s;
    uint16_t *t1 = t+s;
    uint16_t *r1 = r+s, *r2 = r+2*s, *r3 = r+3*s;

    for(i=0; i<s; i++)
    {
        r[i] = a[i]-a1[i];
        r1[i] = b1[i]-b[i];
    }
    toom4_toom3(t, r2, r, r1, s);
    toom4_toom3(r2, r, a1, b1, s);
    for(i=0; i<s; i++)
    {
        r1[i] = r2[i] + t[i];
        r2[i] += r3[i] + t1[i];
    }
    toom4_toom3(t, r, a, b, s);
    for(i=0; i<s; i++)
    {
        r[i] = t[i];
        r1[i] += t[i] + t1[i];
        r2[i] += t1[i];
    }
    return;
}
