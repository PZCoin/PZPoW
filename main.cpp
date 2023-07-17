#include <iostream>
#include <unistd.h>
#include "pzmem.hpp"

PZ_MEM pz;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <seed>\n";
		return 1;
	}
	pz.init(std::stoll(argv[1]));
	unsigned char *tmpnonce = new unsigned char[42];
	tmpnonce[40] = '\n';
	tmpnonce[41] = 0;
	clock_t start = clock();
	for (int i = 0; i < 26843546; i++) {
		pz.gen_nonce(i, tmpnonce);
	}
	clock_t end = clock();
	std::cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
	std::cout << 26.843546 / ((double)(end - start) / CLOCKS_PER_SEC) << " MN/s\n";
	std::cout << (double)CLOCKS_PER_SEC / (end - start) << " GB/s\n";
	for (int i = 0; i < 4; i++) {
		pz.gen_nonce(i, tmpnonce);
		std::cout << tmpnonce;
	}

	return 0;
}