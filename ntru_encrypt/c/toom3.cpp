#include "simd_poly.h"

int
toom3(
    uint16_t        *r, /* out - a * b in Z[x], must be length 2n */
    uint16_t        *t, /*  in - n coefficients of scratch space */
    uint16_t const  *a, /*  in - polynomial */
    uint16_t const  *b, /*  in - polynomial */
    uint16_t const   n) /*  in - number of coefficients in a and b */
{
    if (n>96)
    {
        printf("degree exceeds the maximum (96) allowed\n");
        return -1;
    }
    if (n<=32)
    {
        grade_school_mul(r, a, b, n);
        return 0;
    }

    uint16_t    i;
    uint16_t    s = 32;
    uint16_t    s2 = 64;
    uint16_t  const  *a1 = a+s, *a2 = a+s2;
    uint16_t  const  *b1 = b+s, *b2 = b+s2;
    uint16_t    *r1 = r+s, *r2 = r+s2, *r3 = r+s*3, *r4 = r+s*4, *r5 = r+s*5;
    uint16_t    *t2 = t+s2, *t4 = t+s2*2, *t6 = t+s2*3, *t8 = t+s2*4;

    /*
     * t  = w0   = a(0) * b(0)
     * t8 = w4   = a(inf) * b(inf)
     *           = a2     * b2
     */

    grade_school_mul(t, a, b, s);
    grade_school_mul(t8, a2, b2, s);

#ifdef DEBUG
    printf("t:");
    print32poly(t);
    print32poly(t+32);
    printf("t8:");
    print32poly(t8);
    print32poly(t8+32);
#endif
    /*
     * t2 = a(1) *b(1)
     *    = (a0+a1+a2)*(b0+b1+b2)
     *    = r         * r2
     * t4 = a(-1) * b(-1)
     *    = (a0-a1+a2)*(b0-b1+b2)
     *    = r1        * r3
     */
    for (i=0;i<s;i++)
    {
        r[i]  = a[i]  + a2[i];
        r1[i] = r[i]  - a1[i];
        r[i]  = r[i]  + a1[i];
        r2[i] = b[i]  + b2[i];
        r3[i] = r2[i] - b1[i];
        r2[i] = r2[i] + b1[i];
    }
    grade_school_mul(t2, r, r2, s);
    grade_school_mul(t4, r1, r3, s);

#ifdef DEBUG
    printf("t2:");
    print32poly(t2);
    print32poly(t2+32);
    printf("t4:");
    print32poly(t4);
    print32poly(t4+32);
#endif


    /*
     * r  = (t2+t4)/2 - w0 - w4
     *    = w2
     * r2 = (t2-t4)/2
     *    = w1 + w3
     */
    for (i=0;i<s2;i++)
    {
        r[i]  = (t2[i]  + t4[i])/2 - t[i] - t8[i];
        r2[i] = (t2[i]  - t4[i])/2;
    }

#ifdef DEBUG
    printf("r:");
    print32poly(r);
    print32poly(r+32);
    printf("r2:");
    print32poly(r2);
    print32poly(r2+32);
#endif

    /*
     * t6 = a(2) *b(2)
     *    = (a0+2a1+4a2)*(b0+2b1+4b2)
     *    = r4          * r5
     */
    for (i=0;i<s;i++)
    {
        r4[i]  = a[i]  + 2*a1[i] + 4*a2[i];
        r5[i]  = b[i]  + 2*b1[i] + 4*b2[i];
    }
    grade_school_mul(t6, r4, r5, s);

    /*
     * t6 = w1 + 4*w3
     *    = (t6 - w0 - 4*w2 - 16*w4)/2
     *    = (t6 - t0 - 4*r  - 16*t8)/2
     */

    for (i=0;i<s2;i++)
    {
        t6[i]  = (t6[i] - t[i]-4*r[i]-16*t8[i])/2;
    }


#ifdef DEBUG
    printf("t6:");
    print32poly(t6);
    print32poly(t6+32);
#endif

    /*
     * t2 = w3 = (t6 - r2)/3
     *    = (t6 - t4) * 43691
     * t4 = w1 = (4*r2-t6)/3
     *    = (4*r4-t6) * 43691
     */
    for (i=0;i<s2;i++)
    {
        t2[i]  = (t6[i] - r2[i])*43691;
        t4[i]  = ((4*r2[i] - t6[i])*43691);
    }
#ifdef DEBUG
    printf("t2:");
    print32poly(t2);
    print32poly(t2+32);
    printf("t4:");
    print32poly(t4);
    print32poly(t4+32);
#endif

    /*
     * now we have
     *  t0 = w0
     *  t4 = w1
     *  r  = w2
     *  t2 = w3
     *  t8 = w4
     * putting them back
     */

    memcpy(r2, r,  sizeof(uint16_t)*s2);
    memcpy(r,  t,  sizeof(uint16_t)*s2);
    memcpy(r4, t8, sizeof(uint16_t)*s2);


    for (i=0;i<s2;i++)
    {
        r1[i] += t4[i];
        r3[i] += t2[i];
    }

    return 0;
}
