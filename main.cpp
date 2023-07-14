#include <iostream>
#include "pzmem.hpp"

PZ_MEM pz;

int main(int argc, char *argv[]) {
	pz.init(argv[1][0] - '0');
	unsigned char *tmpnonce = (unsigned char *)malloc(34);
	tmpnonce[32] = '\n';
	tmpnonce[33] = 0;
	for (int i = 0; i < 16; i++) {
		pz.gen_nonce(i, tmpnonce);
		std::cout << tmpnonce;
	}
	return 0;
}