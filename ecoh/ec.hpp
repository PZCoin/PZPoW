#pragma once
#ifndef EC_H
#define EC_H

#include "gf.hpp"
#include <cstdio>
#include <iomanip>
#include <iostream>

class EC_point {
private:
public:
	gf_t x;
	gf_t y;
	bool inf;
	void init(void);
	void calc_y(void);
	inline bool add_points(const EC_point &, gf_t, gf_t);

public:
	EC_point(void);
	EC_point(const EC_point &);
	EC_point(const gf_t);
	EC_point(const gf_t, const gf_t);
	~EC_point(void);

	/**
	 * @brief Set the current point to the generator point
	 */
	void set_gen(void);
	/**
	 * @brief Set the current point to the point at infinity
	 */
	void set_inf(void);
	void set(const gf_t, const gf_t);
	void operator=(const EC_point &);
	/**
	 * @brief Set the x value of the current point, calculate the y value
	 */
	void set_x(const gf_t);
	EC_point operator+(const EC_point &);
	void add_self(const EC_point &);
	EC_point operator*(const gf_t);
	void double_self(void);
	bool valid(void);
	bool operator==(const EC_point &);
	void print(void);
	constexpr void get_xy(gf_t *, gf_t *);
};

inline static const int EC_field_size(void);

inline static void EC_order(gf_t);

#endif
