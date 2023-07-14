#include <iostream>
#include <random>
#include <string>

#define MEM_SZ (1LL<<32)

unsigned char *memory;
std::mt19937_64 rng;

void fill_memory() {
	for (unsigned int i = 0; i < (MEM_SZ>>3); i++) {
		((uint64_t*)memory)[i] = rng();
	}
}

void gen_nonce(unsigned int idx, unsigned char *str) {
	for (int i = 0; i < 32; i += 2) {
		str[i] = "0123456789abcdef"[memory[idx] >> 4];
		str[i+1] = "0123456789abcdef"[memory[idx += 997525853] & 0xf];
	}
}

int main() {
	memory = (unsigned char *)malloc(MEM_SZ);
	std::cout << "Filling memory...\n";
	clock_t start = clock();
	fill_memory();
	clock_t end = clock();
	std::cout << "Success! " << ((double)(end-start) / CLOCKS_PER_SEC) << " seconds elapsed.\nChoose a seed for the RNG: ";
	int seed;
	std::cin >> seed;
	rng.seed(seed);
	std::cout << "Testing nonce generation speed...\n";
	unsigned char *tmpnonce = (unsigned char *)malloc(33);
	tmpnonce[32] = 0;
	start = clock();
	for (int i = 0; i < 67108864; i++) {
		gen_nonce(i, tmpnonce);
	}
	end = clock();
	double speed = (double)CLOCKS_PER_SEC / (end - start);
	std::cout << "Speed: " << std::fixed << speed << " GB / s\n";
	std::cout << "Speed: " << speed * 67.108864 << " MNonces / sec\n";
	for (int i = 0; i < 16; i++) {
		gen_nonce(i, tmpnonce);
		std::cout << tmpnonce << "\n";
	}
	return 0;
}