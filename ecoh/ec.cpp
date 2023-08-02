/**
 * @file ec.cpp
 * @author wdotmathree (wdotmathree@gmail.com)
 * @brief Implements routines on elliptic curve points
 * @date 2023-08-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "ec.hpp"

static const gf_t DIV2 = "\x50\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
						 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04";
static const gf_t A_1_4 =
	"\x29\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02";
static const gf_t B = "\xf5\xa2\x79\x3b\x31\x3e\x26\xf6\x5a\x48\x81\xa5\xa2\x9f\x30\x45\x76\xfd"
					  "\x97\xca\x3f\x30\xa0\x19\x8a\xaf\xa4\xa5\x6d\x59\xb8\xc8\x0a\x68\x7b\x02";
static EC_point
	G("\x53\x20\xb1\x86\xcd\xbe\xcd\xf8\x98\xe1\xe2\x80\x9c\xac\x7e\x55\xb8\x25"
	  "\xed\x2e\xec\xdf\xb0\x70\x8c\x4f\x93\xe1\x90\xdd\xb7\x8d\x25\x39\xf9\x05",
	  "\xf4\x12\x81\xbe\x45\xdf\xf0\x13\xc8\x79\x67\x82\xb0\xdd\x0e\x35\x02\xf7"
	  "\x6f\x51\xb4\x02\x0d\xb2\xd4\xe6\x8f\xb9\x1c\x14\x24\xfe\x54\x68\x67\x03");

static const gf_t N;

inline static const int EC_field_size() {
	return 283;
}

inline static void EC_order(gf_t out) {
	GF_copy(out, N);
}

void EC_point::init(void) {
	x = new char[SIZEOF_GF_T];
	y = new char[SIZEOF_GF_T];
}

void EC_point::calc_y(void) {
	char tmp[SIZEOF_GF_T];
	// y = sqrt(x^3 + (A + 1/4)x^2 + B) + x/2
	// x^3 + Cx^2 = x^2(x + C)
	GF_add(tmp, x, A_1_4);
	GF_mul(tmp, tmp, x);
	GF_mul(tmp, tmp, x);
	GF_add(tmp, tmp, B);
	GF_sqrt(y, tmp);
	GF_mul(tmp, x, DIV2);
	GF_add(y, y, tmp);
}

EC_point::EC_point(void) {
	init();
	inf = true;
}

EC_point::EC_point(const EC_point &p) {
	init();
	*this = p;
}

EC_point::EC_point(const gf_t x) {
	init();
	GF_copy(this->x, x);
	inf = false;
	calc_y();
}

EC_point::EC_point(const gf_t x, const gf_t y) {
	init();
	GF_copy(this->x, x);
	GF_copy(this->y, y);
}

EC_point::~EC_point(void) {
	delete[] x;
	delete[] y;
}

void EC_point::set_gen(void) {
	*this = G;
}

void EC_point::set_inf(void) {
	inf = true;
}

void EC_point::set(const gf_t x, const gf_t y) {
	GF_copy(this->x, x);
	GF_copy(this->y, y);
	inf = false;
}

void EC_point::set_x(const gf_t x) {
	GF_copy(this->x, x);
	inf = false;
	calc_y();
}

void EC_point::operator=(const EC_point &p) {
	if (!(inf = p.inf)) {
		GF_copy(this->x, x);
		GF_copy(this->y, y);
	}
}

inline bool EC_point::add_points(const EC_point &b, gf_t out_x, gf_t out_y) {
	if (inf && b.inf)
		return true;
	if (inf) {
		GF_copy(out_x, b.x);
		GF_copy(out_y, b.y);
		return false;
	}
	if (b.inf) {
		GF_copy(out_x, x);
		GF_copy(out_y, y);
		return false;
	}

	char slope[SIZEOF_GF_T], intercept[SIZEOF_GF_T];

	// Calculate slope of line
	GF_sub(out_x, y, b.y);
	GF_sub(out_y, x, b.x);
	GF_div(slope, out_x, out_y);
	// Calculate intercept of line
	GF_mul(intercept, slope, x);
	GF_sub(intercept, intercept, y);
	// Calculate x
	GF_mul(out_x, slope, slope);
	GF_add(out_x, out_x, slope);
	*(uint32_t *)out_x ^= 1;
	GF_sub(out_x, out_x, x);
	GF_sub(out_x, out_x, b.x);
	// Calculate y
	// y = slope * x + intercept + x (y1 = y0 + x due to quadratic shenanigans)
	//   = (slope + 1) * x + intercept
	*(uint32_t *)slope ^= 1;
	GF_mul(out_y, slope, out_x);
	GF_add(out_y, out_y, intercept);
	return false;
}

EC_point EC_point::operator+(const EC_point &b) {
	if (*this == b) {
		EC_point out(*this);
		out.double_self();
		return out;
	}
	EC_point out;
	out.inf = add_points(b, out.x, out.y);
	return out;
}

void EC_point::add_self(const EC_point &b) {
	char tmp_x[SIZEOF_GF_T], tmp_y[SIZEOF_GF_T];
	inf = add_points(b, tmp_x, tmp_y);
	if (!inf) {
		GF_copy(x, tmp_x);
		GF_copy(y, tmp_y);
	}
}

EC_point EC_point::operator*(const gf_t) {
	/// TODO: Implement
	return EC_point();
}

void EC_point::double_self(void) {
	if (inf)
		return;

	char slope[SIZEOF_GF_T], intercept[SIZEOF_GF_T];

	// Calculate slope using formula: y' = (3x^2 + 2Ax) / (2y + x)
	// 3x^2 + 2Ax = x(3x + 2A)
	GF_double(slope, x);
	GF_add(slope, slope, x);
	*(uint32_t *)slope ^= 2;
	GF_mul(intercept, slope, x);
	GF_double(slope, y);
	GF_add(slope, slope, x);
	GF_div(slope, intercept, slope);
	// Calculate intercept of line
	GF_mul(intercept, slope, x);
	GF_sub(intercept, intercept, y);
	// Calculate x (x2 = m^2 + m + A - x1 - x0 but x1 = x0 so x2 = m^2 + m + A)
	GF_mul(x, slope, slope);
	GF_add(x, x, slope);
	*(uint32_t *)x ^= 1;
	// Calculate y
	// y = slope * x + intercept + x (y1 = y0 + x due to quadratic shenanigans)
	//   = (slope + 1) * x + intercept
	*(uint32_t *)slope ^= 1;
	GF_mul(y, slope, x);
	GF_add(y, y, intercept);
}

bool EC_point::valid(void) {
	char lhs[SIZEOF_GF_T], rhs[SIZEOF_GF_T], tmp[SIZEOF_GF_T];

	// rhs = x^3
	GF_mul(lhs, x, x);
	GF_mul(rhs, lhs, x);
	// rhs = x^3 + Ax^2
	GF_add(rhs, rhs, lhs);
	// rhs = x^3 + Ax^2 + B
	GF_add(rhs, rhs, B);
	// lhs = y^2
	GF_mul(lhs, y, y);
	// lhs = y^2 + xy
	GF_mul(tmp, x, y);
	GF_add(lhs, lhs, tmp);

	return GF_equal(lhs, rhs);
}

bool EC_point::operator==(const EC_point &b) {
	return (inf == b.inf) && GF_equal(x, b.x) && GF_equal(y, b.y);
}

void EC_point::print(void) {
	if (inf) {
		std::cout << "(INF,INF)" << std::endl;
	} else {
		std::cout << "(0x";
		GF_print(x);
		std::cout << ", 0x";
		GF_print(y);
		std::cout << ")" << std::dec << std::endl;
	}
}

constexpr void EC_point::get_xy(gf_t *x, gf_t *y) {
	*x = this->x;
	*y = this->y;
}

int main() {
	G.print();
	EC_point a(G.x);
	a.print();
	// EC_point a = G + G;
	// a.print();
	return 0;
}
