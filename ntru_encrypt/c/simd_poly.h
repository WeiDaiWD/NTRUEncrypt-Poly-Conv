#ifndef _SIMD_POLY_H_
#define _SIMD_POLY_H_
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */
#include <immintrin.h>
using namespace std;

void
karatsuba_toom4(
    uint16_t        *r, /* out - a * b in Z[x], must be length 2n */
    uint16_t        *t, /*  in - n coefficients of scratch space */
    uint16_t const  *a, /*  in - polynomial */
    uint16_t const  *b, /*  in - polynomial */
    uint16_t const   n);/*  in - number of coefficients in a and b */

/* ******************** other functions ***************************/

/* ******************** school book area ************************ */

void
grade_school_mul(
    uint16_t        *res1,  /* out - a * b in Z[x], must be length 2N */
    uint16_t const  *a,     /*  in - polynomial */
    uint16_t const  *b,     /*  in - polynomial */
    uint16_t const   N);    /*  in - number of coefficients in a and b */

/* ******************** toom 3 area ************************ */

/*
 * toom3 multiplication with uint16_t coefficients,
 * only usable for degree less than 96,
 * uses uint16_t based school book multiplications when
 * degree drops below 32
 * okay speed
 */
int
toom3(
    uint16_t        *r, /* out - a * b in Z[x], must be length 2n */
    uint16_t        *t, /*  in - n coefficients of scratch space */
    uint16_t const  *a, /*  in - polynomial */
    uint16_t const  *b, /*  in - polynomial */
    uint16_t const   n);/*  in - number of coefficients in a and b */

/*
 * toom4 multiplication with uint16_t coefficients,
 * only usable for degree less than 384,
 * uses toom3 multiplications when
 * degree drops below 96
 * okay speed
 */

int
toom4_toom3(
    uint16_t        *r, /* out - a * b in Z[x], must be length 2n */
    uint16_t        *t, /*  in - n coefficients of scratch space */
    uint16_t const  *a, /*  in - polynomial */
    uint16_t const  *b, /*  in - polynomial */
    uint16_t const   n);/*  in - number of coefficients in a and b */

#endif /*SIMD_POLY_H_*/
