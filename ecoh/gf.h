#pragma once
#ifndef GF_H
#define GF_H

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

#define SIZEOF_GF_T 36

#define gf_t char *

void GF_add(gf_t, const gf_t, const gf_t);
void GF_sub(gf_t, const gf_t, const gf_t);
void GF_mul(gf_t, const gf_t, const gf_t);
void GF_div(gf_t, const gf_t, const gf_t);
void GF_inv(gf_t, const gf_t);
void GF_pow(gf_t, const gf_t, const gf_t);
void GF_sqrt(gf_t, const gf_t);

void GF_print(const gf_t);
void GF_copy(gf_t, const gf_t);

#endif
