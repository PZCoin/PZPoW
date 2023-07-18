#include <iostream>
#include <unistd.h>
#include "pzmem.hpp"

PZ_MEM pz;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <seed>\n";
		return 1;
	}
	clock_t start, end;
	start = clock();
	pz.init(std::stoll(argv[1]));
	end = clock();
	std::cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
	unsigned char *tmpnonce = new unsigned char[42];
	tmpnonce[40] = '\n';
	tmpnonce[41] = 0;
	start = clock();
	for (int i = 0; i < 26843546; i++) {
		pz.gen_nonce(i, tmpnonce);
	}
	end = clock();
	std::cout << 26.843546 / ((double)(end - start) / CLOCKS_PER_SEC) << " MN/s\n";
	std::cout << (double)CLOCKS_PER_SEC / (end - start) << " GB/s\n";
	for (int i = 0; i < 4; i++) {
		pz.gen_nonce(i, tmpnonce);
		std::cout << tmpnonce;
	}

	return 0;
}