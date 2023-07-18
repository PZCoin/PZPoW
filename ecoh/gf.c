#include "gf.h"
#include <stdio.h>
#include <string.h>

// Exponents for calculating square roots and inverses
static const gf_t GF_SQRT_EXP =
	"\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
static const gf_t GF_INV_EXP =
	"\x07\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
	"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xfe";

static void inline _zero_doublelen(char *buf) {
	_mm256_storeu_si256((__m256i *)buf, _mm256_setzero_si256());
	_mm256_storeu_si256((__m256i *)(buf + 32), _mm256_setzero_si256());
	*(uint64_t *)(buf + 64) = 0;
}

static void inline _zero_len(char *buf) {
	_mm256_storeu_si256((__m256i *)buf, _mm256_setzero_si256());
	*(uint32_t *)(buf + 32) = 0;
}

void GF_mul(gf_t r, const gf_t x, const gf_t y) {
	__attribute__((aligned(32))) char tmp[SIZEOF_GF_T * 2];
	_zero_doublelen(tmp);

	// multiply
	for (int i = 0; i < 5; i++) {
		if (__builtin_expect(i == 4, 0)) {
			asm("vpxor %%xmm0, %%xmm0, %%xmm0;"
				"vmovd (%0), %%xmm0" ::"r"(x + i * 8)
				: "ymm0");
		} else {
			asm("vmovq (%0), %%xmm0" ::"r"(x + i * 8) : "ymm0");
		}
		for (int j = 0; j < 5; j++) {
			if (__builtin_expect(j == 4, 0)) {
				asm("vpxor %%xmm1, %%xmm1, %%xmm1;"
					"vmovd (%0), %%xmm1" ::"r"(y + j * 8)
					: "ymm1");
			} else {
				asm("vmovq (%0), %%xmm1" ::"r"(y + j * 8) : "ymm1");
			}
			asm(".intel_syntax noprefix;"
				"vpclmulqdq xmm2, xmm0, xmm1, 0x00;"
				"vpxor xmm2, xmm2, [%1];"
				"vmovdqu [%1], xmm2;"
				".att_syntax prefix;"
				: "+m"(tmp)
				: "r"(tmp + (i + j) * 8)
				: "ymm0", "ymm1", "ymm2");
		}
	}

	// reduce
	uint32_t carry = 0;
	for (int i = 72 - 4; i >= 36; i -= 4) {
		uint64_t bits = (uint64_t)(*(uint32_t *)(tmp + i)) << 16;
		bits = bits << 9 ^ bits << 4 ^ bits << 2 ^ bits >> 3;
		uint32_t temp_carry = bits << 16;
		bits = (bits >> 16) ^ carry;
		carry = temp_carry;
		*(uint64_t *)(tmp + i - 35) ^= bits;
	}
	uint32_t bits = tmp[35] & 0xf8;
	bits = bits << 9 ^ bits << 4 ^ bits << 2 ^ bits >> 3;
	carry = (carry >> 24) ^ bits;
	*(uint32_t *)tmp ^= carry;
	tmp[35] &= 0x07;

	_mm256_storeu_si256((__m256i *)r, _mm256_load_si256((__m256i *)tmp));
	*(uint32_t *)(r + 32) = *(uint32_t *)(tmp + 32);
}

void GF_div(gf_t r, const gf_t x, const gf_t y) {
	gf_t tmp;
	GF_inv(tmp, y);
	GF_mul(r, x, tmp);
}

inline void GF_inv(gf_t r, const gf_t x) {
	GF_pow(r, x, GF_INV_EXP);
}

void GF_pow(gf_t r, const gf_t x, const gf_t y) {
	gf_t old_r = r;
	_zero_len(r);
	r[0] = 1;
	char tmp[SIZEOF_GF_T];
	char tmpr[SIZEOF_GF_T];
	GF_copy(tmp, x);
	for (int i = 0; i <= SIZEOF_GF_T - 8; i += 8) {
		uint64_t exp;
		int j;
		if (__builtin_expect(i == 32, 0)) {
			exp = *(uint32_t *)(y + 32);
			j = 32;
		} else {
			exp = *(uint64_t *)(y + i);
			j = 64;
		}
		while (j--) {
			GF_mul(tmpr, r, r);
			if (exp & 1) {
				GF_mul(r, tmpr, tmp);
			} else {
				GF_copy(r, tmpr);
			}
			exp >>= 1;
			asm(".intel_syntax noprefix;"
				"shlq [%0], 1;"
				"rclq [%0+8], 1;"
				"rclq [%0+16], 1;"
				"rclq [%0+24], 1;"
				"rclq [%0+32], 1;"
				"rclq [%0+40], 1;"
				"rclq [%0+48], 1;"
				"rclq [%0+56], 1;"
				"rclq [%0+64], 1;"
				".att_syntax prefix;" ::"r"(tmp));
			j--;
			GF_mul(r, tmpr, tmpr);
			if (exp & 1) {
				GF_mul(tmpr, r, tmp);
			} else {
				GF_copy(tmpr, r);
			}
			exp >>= 1;
			asm(".intel_syntax noprefix;"
				"shlq [%0], 1;"
				"rclq [%0+8], 1;"
				"rclq [%0+16], 1;"
				"rclq [%0+24], 1;"
				"rclq [%0+32], 1;"
				"rclq [%0+40], 1;"
				"rclq [%0+48], 1;"
				"rclq [%0+56], 1;"
				"rclq [%0+64], 1;"
				".att_syntax prefix;" ::"r"(tmp));
		}
	}
	if (old_r != r) {
		GF_copy(old_r, r);
	}
}

inline void GF_sqrt(gf_t r, const gf_t x) {
	GF_pow(r, x, GF_SQRT_EXP);
}

void GF_print(const gf_t x) {
	for (int i = 35; i >= 0; i--) {
		printf("%02x", x[i] & 0xff);
	}
	printf("\n");
}

int main() {
	char a[SIZEOF_GF_T] = {0};
	char b[SIZEOF_GF_T] = {0};
	char c[SIZEOF_GF_T] = {0};

	a[0] = 2;
	b[0] = 2;

	GF_pow(c, a, b);
	GF_print(c);
}
