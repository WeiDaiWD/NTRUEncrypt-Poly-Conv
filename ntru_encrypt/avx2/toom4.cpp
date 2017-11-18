/*
 * toom4.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: zhenfei
 */
#include "simd_poly.h"

int
__mm256i_toom4__mm256i_toom3(
    uint16_t        *r, /* out - a * b in Z[x], must be length 2n */
    uint16_t        *t, /*  in - n coefficients of scratch space */
    uint16_t const  *a, /*  in - polynomial */
    uint16_t const  *b, /*  in - polynomial */
    uint16_t const   n) /*  in - number of coefficients in a and b */
{
    if (n < 96)
    {
        grade_school_mul(r, a, b, n);
        //__m256i_grade_school_mul_32(r, a, b, n);
        return -1;
    }
    if (n>384)
    {
        printf("degree exceeds the maximum (384) allowed\n");
        return -1;
    }
    uint16_t s = 96, s2 = 192;
    uint16_t i;
    uint16_t const *a1 = a+s, *a2 = a+2*s, *a3 = a+3*s;
    uint16_t const *b1 = b+s, *b2 = b+2*s, *b3 = b+3*s;
    uint16_t *r1 = r+s, *r2 = r+2*s, *r4 = r+4*s, *r6 = r+6*s, *r7 = r+7*s;
    uint16_t *t3 = t+2*s, *t5 = t+4*s;
    uint16_t *e = t+6*s; // for karatsuba only

    __m256i  mr0[12],
             mr2[12],
             mr4[12],
             mr6[12];
    __m256i  mt0[12],
             mt3[12],
             mt5[12];
    __m256i  mtmp1, mtmp2, mtmp3;


    // +-1 ---- t: -, r2: +
    for (i=0;i<6;i++)
    {
        /*
         * r1[i] = a[i]+a2[i];
         * x = a1[i]+a3[i];
         * r[i] = r1[i]+x;
         * r1[i] -= x;
         */

        mr0[i+6] = _mm256_loadu_si256((__m256i *)a +i);
        mtmp1  = _mm256_loadu_si256((__m256i *)a2+i);
        mr0[i+6] = _mm256_add_epi16 (mr0[i+6], mtmp1);
        mtmp1  = _mm256_loadu_si256((__m256i *)a1+i);
        mtmp2  = _mm256_loadu_si256((__m256i *)a3+i);
        mtmp1  = _mm256_add_epi16 (mtmp1, mtmp2);
        mr0[i] = _mm256_add_epi16 (mr0[i+6], mtmp1);
        mr0[i+6] = _mm256_sub_epi16 (mr0[i+6], mtmp1);


        /*
         * put back
         */
        _mm256_storeu_si256((__m256i *)r+i,  mr0[i]);
        _mm256_storeu_si256((__m256i *)r1+i, mr0[i+6]);


        /*
         * r7[i] = b[i]+b2[i];
         * x = b1[i]+b3[i];
         * r6[i] = r7[i]+x;
         * r7[i] -= x;
         */
        mr6[i+6] = _mm256_loadu_si256((__m256i *)b +i);
        mtmp1  = _mm256_loadu_si256((__m256i *)b2+i);
        mr6[i+6] = _mm256_add_epi16 (mr6[i+6], mtmp1);
        mtmp1  = _mm256_loadu_si256((__m256i *)b1+i);
        mtmp2  = _mm256_loadu_si256((__m256i *)b3+i);
        mtmp1  = _mm256_add_epi16 (mtmp1, mtmp2);
        mr6[i] = _mm256_add_epi16 (mr6[i+6], mtmp1);
        mr6[i+6] = _mm256_sub_epi16 (mr6[i+6], mtmp1);


        /*
         * put back
         */
        _mm256_storeu_si256((__m256i *)r6+i, mr6[i]);
        _mm256_storeu_si256((__m256i *)r7+i, mr6[i+6]);
    }

    __mm256i_toom3__mm256i_SB(t, e, r, r6, s);
    __mm256i_toom3__mm256i_SB(t5,e, r1, r7, s);


    for (i=0;i<12;i++)
    {
        /*
         * r2[i] = (t[i]+t5[i]) >> 1;
         */
        mt0[i] = _mm256_loadu_si256((__m256i *)t +i);
        mt5[i] = _mm256_loadu_si256((__m256i *)t5 +i);

        mr2[i] = _mm256_add_epi16 (mt0[i], mt5[i]);
        mr2[i] = _mm256_srai_epi16(mr2[i], 1);

        /*
         * t[i] = (t[i]-t5[i]) >> 1;
         */
        mt0[i] = _mm256_sub_epi16 (mt0[i], mt5[i]);
        mt0[i] = _mm256_srai_epi16(mt0[i], 1);

    }


    // +-2 ---- t3: -, r4: +

    for (i=0;i<6;i++)
    {
        /*
         * r1[i] = a[i]+(a2[i] << 2);
         * x = (a1[i] << 1)+(a3[i] << 3);
         * r[i] = r1[i]+x;
         * r1[i] -= x;
         */
        mr0[i+6] = _mm256_loadu_si256((__m256i *)a +i);
        mtmp1    = _mm256_loadu_si256((__m256i *)a2+i);
        mtmp1    = _mm256_slli_epi16 (mtmp1, 2);
        mr0[i+6] = _mm256_add_epi16 (mr0[i+6], mtmp1);

        mtmp1  = _mm256_loadu_si256((__m256i *)a1+i);
        mtmp1  = _mm256_slli_epi16 (mtmp1, 1);
        mtmp2  = _mm256_loadu_si256((__m256i *)a3+i);
        mtmp2  = _mm256_slli_epi16 (mtmp2, 3);
        mtmp1  = _mm256_add_epi16 (mtmp1, mtmp2);

        mr0[i]   = _mm256_add_epi16 (mr0[i+6], mtmp1);
        mr0[i+6] = _mm256_sub_epi16 (mr0[i+6], mtmp1);

        /*
         * put back
         */
        _mm256_storeu_si256((__m256i *)r+i,  mr0[i]);
        _mm256_storeu_si256((__m256i *)r1+i, mr0[i+6]);

        /*
         * r7[i] = b[i]+(b2[i] << 2);
         * x = (b1[i] << 1)+(b3[i] << 3);
         * r6[i] = r7[i]+x;
         * r7[i] -= x;
         */
        mr6[i+6] = _mm256_loadu_si256((__m256i *)b +i);
        mtmp1    = _mm256_loadu_si256((__m256i *)b2+i);
        mtmp1    = _mm256_slli_epi16 (mtmp1, 2);
        mr6[i+6] = _mm256_add_epi16 (mr6[i+6], mtmp1);

        mtmp1  = _mm256_loadu_si256((__m256i *)b1+i);
        mtmp1  = _mm256_slli_epi16 (mtmp1, 1);
        mtmp2  = _mm256_loadu_si256((__m256i *)b3+i);
        mtmp2  = _mm256_slli_epi16 (mtmp2, 3);
        mtmp1  = _mm256_add_epi16 (mtmp1, mtmp2);

        mr6[i]   = _mm256_add_epi16 (mr6[i+6], mtmp1);
        mr6[i+6] = _mm256_sub_epi16 (mr6[i+6], mtmp1);

        /*
         * put back
         */
        _mm256_storeu_si256((__m256i *)r6+i, mr6[i]);
        _mm256_storeu_si256((__m256i *)r7+i, mr6[i+6]);

    }

    __mm256i_toom3__mm256i_SB(t3, e, r, r6, s);
    __mm256i_toom3__mm256i_SB(t5, e, r1, r7, s);

    for (i=0;i<12;i++)
    {
        /*
         * r4[i] = (t3[i]+t5[i]) >> 1;
         */
        mt3[i] = _mm256_loadu_si256((__m256i *)t3 +i);
        mt5[i] = _mm256_loadu_si256((__m256i *)t5 +i);
        mr4[i] = _mm256_add_epi16 (mt3[i], mt5[i]);
        mr4[i] = _mm256_srai_epi16(mr4[i], 1);
        /*
         * t3[i] = (t3[i]-t5[i]) >> 2;
         */
        mt3[i] = _mm256_sub_epi16 (mt3[i], mt5[i]);
        mt3[i] = _mm256_srai_epi16(mt3[i], 2);

    }


    // +3 ---- t5

    mtmp2  = _mm256_set1_epi16 (3);
    for (i=0;i<6;i++)
    {
        /*
         * r[i] = (((a3[i]*3+a2[i])*3)+a1[i])*3+a[i];
         */
        mtmp1  = _mm256_loadu_si256((__m256i *)a3 +i);
        mr0[i] = _mm256_mullo_epi16( mtmp1,mtmp2);
        mtmp1  = _mm256_loadu_si256((__m256i *)a2 +i);
        mr0[i] = _mm256_add_epi16 (mr0[i], mtmp1);
        mr0[i] = _mm256_mullo_epi16( mr0[i],mtmp2);
        mtmp1  = _mm256_loadu_si256((__m256i *)a1 +i);
        mr0[i] = _mm256_add_epi16 (mr0[i], mtmp1);
        mr0[i] = _mm256_mullo_epi16( mr0[i],mtmp2);
        mtmp1  = _mm256_loadu_si256((__m256i *)a  +i);
        mr0[i] = _mm256_add_epi16 (mr0[i], mtmp1);

        /*
         * put back
         */
        _mm256_storeu_si256((__m256i *)r+i,  mr0[i]);

        /*
         * r6[i] = (((b3[i]*3+b2[i])*3)+b1[i])*3+b[i];
         */

        mtmp1  = _mm256_loadu_si256((__m256i *)b3 +i);
        mr6[i] = _mm256_mullo_epi16( mtmp1,mtmp2);
        mtmp1  = _mm256_loadu_si256((__m256i *)b2 +i);
        mr6[i] = _mm256_add_epi16 (mr6[i], mtmp1);
        mr6[i] = _mm256_mullo_epi16( mr6[i],mtmp2);
        mtmp1  = _mm256_loadu_si256((__m256i *)b1 +i);
        mr6[i] = _mm256_add_epi16 (mr6[i], mtmp1);
        mr6[i] = _mm256_mullo_epi16( mr6[i],mtmp2);
        mtmp1  = _mm256_loadu_si256((__m256i *)b  +i);
        mr6[i] = _mm256_add_epi16 (mr6[i], mtmp1);

        /*
         * put back
         */
        _mm256_storeu_si256((__m256i *)r6+i,  mr6[i]);
    }

    __mm256i_toom3__mm256i_SB(t5, e, r, r6, s); // t5 = H0+3H1+9H2+27H3+81H4+243H5+728H6
    __mm256i_toom3__mm256i_SB(r, e, a, b, s);   // r = H0
    __mm256i_toom3__mm256i_SB(r6, e, a3, b3, s); // r6 = H6


    // solve H1~H5

    mtmp2  = _mm256_set1_epi16 (43691);
    mtmp3  = _mm256_set1_epi16 (9);
    for (i=0; i<12; i++)
    {
        /*
         * r2[i] -= r[i]+r6[i];
         */
        mr0[i] = _mm256_loadu_si256((__m256i *)r +i);
        mr6[i] = _mm256_loadu_si256((__m256i *)r6 +i);
        mt5[i] = _mm256_loadu_si256((__m256i *)t5 +i);

        mtmp1  = _mm256_add_epi16(mr0[i], mr6[i]);
        mr2[i] = _mm256_sub_epi16(mr2[i], mtmp1);

        /*
         * r4[i] = (((r4[i]-r[i]-(r6[i] << 6)) >> 2)-r2[i])*43691;
         */
        mr4[i] = _mm256_sub_epi16(mr4[i], mr0[i]);
        mtmp1  = _mm256_slli_epi16 (mr6[i], 6);
        mr4[i] = _mm256_sub_epi16(mr4[i], mtmp1);
        mr4[i] = _mm256_srai_epi16 (mr4[i], 2);
        mr4[i] = _mm256_sub_epi16(mr4[i], mr2[i]);
        mr4[i] = _mm256_mullo_epi16(mr4[i], mtmp2);

        /*
         * r2[i] -= r4[i];
         */
        mr2[i] = _mm256_sub_epi16(mr2[i], mr4[i]);

        /*
         * t3[i] = (t3[i]-t[i])*43691;
         */
        mt3[i] = _mm256_sub_epi16(mt3[i], mt0[i]);
        mt3[i] = _mm256_mullo_epi16(mt3[i], mtmp2);

        /*
         * t5[i] = ((t5[i]-r[i]-9*(r2[i]+9*(r4[i]+9*r6[i])))*43691-t[i]) >> 3;
         */
        mtmp1  = _mm256_mullo_epi16(mr6[i], mtmp3);
        mtmp1  = _mm256_add_epi16(mr4[i], mtmp1);
        mtmp1  = _mm256_mullo_epi16(mtmp1, mtmp3);
        mtmp1  = _mm256_add_epi16(mr2[i], mtmp1);
        mtmp1  = _mm256_mullo_epi16(mtmp1, mtmp3);
        mtmp1  = _mm256_add_epi16(mr0[i], mtmp1);
        mt5[i] = _mm256_sub_epi16(mt5[i], mtmp1);
        mt5[i] = _mm256_mullo_epi16(mt5[i], mtmp2);
        mt5[i] = _mm256_sub_epi16(mt5[i], mt0[i]);
        mt5[i] = _mm256_srai_epi16 (mt5[i], 3);

        /*
         * t3[i] = (t3[i] << 1)-t5[i];
         */
        mt3[i] = _mm256_slli_epi16(mt3[i], 1);
        mt3[i] = _mm256_sub_epi16(mt3[i], mt5[i]);
    }

    mtmp2  = _mm256_set1_epi16 (52429);
    for (i=0; i<12; i++)
    {
        /*
         * t5[i] = ((t5[i]-t3[i])>>1)*52429;
         */
        mt5[i] = _mm256_sub_epi16(mt5[i], mt3[i]);
        mt5[i] = _mm256_srai_epi16 (mt5[i], 1);
        mt5[i] = _mm256_mullo_epi16(mt5[i], mtmp2);

        /*
         * t[i] -= t3[i]+t5[i];
         */
        mtmp1  = _mm256_add_epi16(mt3[i], mt5[i]);
        mt0[i] = _mm256_sub_epi16(mt0[i], mtmp1);

    }

    for (i=0;i<6;i++)
    {
        /*
         * (r+s)[i] += t[i];
         */
        mr0[i+6] = _mm256_add_epi16(mr0[i+6],mt0[i]);
        mr2[i]   = _mm256_add_epi16(mr2[i],mt0[i+6]);
        /*
         * (r+3*s)[i] += t3[i];
         */
        mr2[i+6] = _mm256_add_epi16(mr2[i+6],mt3[i]);
        mr4[i]   = _mm256_add_epi16(mr4[i],mt3[i+6]);
        /*
         * (r+5*s)[i] += t5[i];
         */
        mr4[i+6] = _mm256_add_epi16(mr4[i+6],mt5[i]);
        mr6[i]   = _mm256_add_epi16(mr6[i],mt5[i+6]);
    }

    for(i=0; i<12; i++)
    {
        _mm256_storeu_si256((__m256i *)r+i,   mr0[i]);
        _mm256_storeu_si256((__m256i *)r2+i,  mr2[i]);
        _mm256_storeu_si256((__m256i *)r4+i,  mr4[i]);
        _mm256_storeu_si256((__m256i *)r6+i,  mr6[i]);

    }
    return 0;

}
