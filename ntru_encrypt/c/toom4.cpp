/*
 * toom4.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: zhenfei
 */
#include "simd_poly.h"

int
toom4_toom3(
    uint16_t        *r, /* out - a * b in Z[x], must be length 2n */
    uint16_t        *t, /*  in - n coefficients of scratch space */
    uint16_t const  *a, /*  in - polynomial */
    uint16_t const  *b, /*  in - polynomial */
    uint16_t const   n) /*  in - number of coefficients in a and b */
{
    if (n < 96)
    {
        grade_school_mul(r, a, b, n);
        return -1;
    }
    if (n>384)
    {
        printf("degree exceeds the maximum (384) allowed\n");
        return -1;
    }

    uint16_t s = 96, s2 = 192;
    uint16_t i;
    uint16_t x; // swap space
    uint16_t const *a1 = a+s, *a2 = a+2*s, *a3 = a+3*s;
    uint16_t const *b1 = b+s, *b2 = b+2*s, *b3 = b+3*s;
    uint16_t *r1 = r+s, *r2 = r+2*s, *r4 = r+4*s, *r6 = r+6*s, *r7 = r+7*s;
    uint16_t *t3 = t+2*s, *t5 = t+4*s;
    uint16_t *e = t+6*s; // for karatsuba only*/

    // +-1 ---- t: -, r2: +
    for(i=0; i<s; i++)
    {
        r1[i] = a[i]+a2[i];
        x = a1[i]+a3[i];
        r[i] = r1[i]+x;
        r1[i] -= x;
        r7[i] = b[i]+b2[i];
        x = b1[i]+b3[i];
        r6[i] = r7[i]+x;
        r7[i] -= x;
    }
    toom3(t, e, r, r6, s);
    toom3(t5, e, r1, r7, s);
    for(i=0; i<s2; i++)
    {
        r2[i] = (t[i]+t5[i]) >> 1;
        t[i] = (t[i]-t5[i]) >> 1;
    }
    // +-2 ---- t3: -, r4: +
    for(i=0; i<s; i++)
    {
        r1[i] = a[i]+(a2[i] << 2);
        x = (a1[i] << 1)+(a3[i] << 3);
        r[i] = r1[i]+x;
        r1[i] -= x;
        r7[i] = b[i]+(b2[i] << 2);
        x = (b1[i] << 1)+(b3[i] << 3);
        r6[i] = r7[i]+x;
        r7[i] -= x;
    }
    toom3(t3, e,r, r6, s);
    toom3(t5, e,r1, r7, s);
    for(i=0; i<s2; i++)
    {
        r4[i] = (t3[i]+t5[i]) >> 1;
        t3[i] = (t3[i]-t5[i]) >> 2;
    }
    // +3 ---- t5
    for(i=0; i<s; i++)
    {
        r[i] = (((a3[i]*3+a2[i])*3)+a1[i])*3+a[i];
        r6[i] = (((b3[i]*3+b2[i])*3)+b1[i])*3+b[i];
    }
    toom3(t5,e,  r, r6, s); // t5 = H0+3H1+9H2+27H3+81H4+243H5+728H6
    toom3(r, e, a, b, s); // r = H0
    toom3(r6, e, a3, b3, s); // r6 = H6
    // solve H1~H5
    for(i=0; i<s2; i++)
    {
        r2[i] -= r[i]+r6[i];
        r4[i] = (((r4[i]-r[i]-(r6[i] << 6)) >> 2)-r2[i])*43691;
        r2[i] -= r4[i];
        t3[i] = (t3[i]-t[i])*43691;
        t5[i] = ((t5[i]-r[i]-9*(r2[i]+9*(r4[i]+9*r6[i])))*43691-t[i]) >> 3;
        t3[i] = (t3[i] << 1)-t5[i];
        t5[i] = ((t5[i]-t3[i])>>1)*52429;
        t[i] -= t3[i]+t5[i];
    }
    for(i=0; i<s2; i++)
    {
        (r+s)[i] += t[i];
        (r+3*s)[i] += t3[i];
        (r+5*s)[i] += t5[i];
    }
    return 0;

}
