#pragma once
#ifndef GF_H
#define GF_H

#include <cstdint>
#include <cstdio>
#include <immintrin.h>

#define SIZEOF_GF_T 36

#define gf_t char *

extern "C" bool GF_equal(const gf_t, const gf_t);
extern "C" void GF_add(gf_t, const gf_t, const gf_t);
extern "C" void GF_sub(gf_t, const gf_t, const gf_t);
/**
 * @note Output cannot be the same as either inputs
 */
void GF_mul(gf_t, const gf_t, const gf_t);
extern "C" void GF_double(gf_t, const gf_t);
void GF_div(gf_t, const gf_t, const gf_t);
void GF_inv(gf_t, const gf_t);
/**
 * @note Output cannot be the same as exponent
 */
void GF_pow(gf_t, const gf_t, const gf_t);
void GF_sqrt(gf_t, const gf_t);

void GF_print(const gf_t);
extern "C" void GF_copy(gf_t, const gf_t);

#endif
