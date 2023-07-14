#include "pzmem.hpp"

void PZ_MEM::fill_memory() {
	for (unsigned int i = 0; i < (PZ_MEM_SZ >> 3); i++)
		((uint64_t *)memory)[i] = rng();
}

void PZ_MEM::gen_nonce(unsigned int idx, unsigned char *str) {
	for (int i = 0; i < 32; i += 2) {
		str[i] = "0123456789abcdef"[memory[idx] >> 4];
		str[i+1] = "0123456789abcdef"[memory[idx += 997525853] & 0xf];
	}
}

void PZ_MEM::init(int seed) {
	memory = (unsigned char *)malloc(PZ_MEM_SZ);
	rng.seed(seed);
	fill_memory();
}