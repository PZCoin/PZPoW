/**
 * @file gf.hpp
 * @author wdotmathree (wdotmathree@gmail.com)
 * @brief Enables operations on elements over GF(2^283)
 * @date 2023-08-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#ifndef GF_H
#define GF_H

#include <cstdint>
#include <cstdio>
#include <immintrin.h>

#define SIZEOF_GF_T 36

#define gf_t char *

/**
 * @brief Tests if two elements are equal
 * @param a First element
 * @param b Second element
 * @return True if equal, false otherwise
 */
extern "C" bool GF_equal(const gf_t, const gf_t);
/**
 * @brief Performs addition of two elements
 * @param out Output element
 * @param a First element
 * @param b Second element
 */
extern "C" void GF_add(gf_t, const gf_t, const gf_t);
/**
 * @brief Performs subtraction of two elements
 * @param out Output element
 * @param a First element
 * @param b Second element
 */
extern "C" void GF_sub(gf_t, const gf_t, const gf_t);
/**
 * @brief Performs multiplication of two elements
 * @param out Output element
 * @param a First element
 * @param b Second element
 * @note Output cannot be the same as either inputs
 */
void GF_mul(gf_t, const gf_t, const gf_t);
/**
 * @brief Doubles an element (multiplies by 2)
 * @param out Output element
 * @param a Input element
 */
extern "C" void GF_double(gf_t, const gf_t);
/**
 * @brief Performs division of two elements
 * @param out Quotient
 * @param a Dividend
 * @param b Divisor
 */
void GF_div(gf_t, const gf_t, const gf_t);
/**
 * @brief Performs inversion of an element
 * @param out Inverse
 * @param a Input
 */
void GF_inv(gf_t, const gf_t);
/**
 * @brief Performs exponentiation of an element
 * @param out Output
 * @param a Base
 * @param b Exponent
 * @note Output cannot be the same as exponent
 */
void GF_pow(gf_t, const gf_t, const gf_t);
/**
 * @brief Performs square root of an element
 * @param out Output
 * @param a Input
 */
void GF_sqrt(gf_t, const gf_t);

/**
 * @brief Prints an element
 * @param a Element to print
 */
void GF_print(const gf_t);
/**
 * @brief Copies an element
 * @param out Output
 * @param a Input
 */
extern "C" void GF_copy(gf_t, const gf_t);

#endif
