#include <iostream>
#include <unistd.h>
#include "pzmem.hpp"

PZ_MEM pz;

int main(int argc, char *argv[]) {
	pz.init(argv[1][0] - '0');
	unsigned char *tmpnonce = (unsigned char *)malloc(42);
	tmpnonce[40] = '\n';
	tmpnonce[41] = 0;
	clock_t start = clock();
	for (int i = 0; i < 26843545; i++) {
		pz.gen_nonce(i, tmpnonce);
		// std::cout << tmpnonce;
	}
	clock_t end = clock();
	std::cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
	std::cout << 26.843545 / ((double)(end - start) / CLOCKS_PER_SEC) << "MN/s\n";
	std::cout << tmpnonce; // To prevent the compiler from optimizing out the loop

	return 0;
}