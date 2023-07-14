#include <iostream>
#include <random>
#include <string>

#define MEM_SZ (1LL<<24)
#define NUM_THREADS 1

__device__ void rng(long long *seed, long long &res) {
	*seed = (*seed * 0x5DEECE66DLL + 0xBLL) & ((1LL << 48) - 1);
	res = *seed;
}

__global__ void fill_memory(unsigned char *memory, long long *seed) {
	for (unsigned int i = 0; i < (MEM_SZ>>3); i++) {
		long long res;
		rng(seed, res);
		((uint64_t *)memory)[i] = res;
	}
}

__global__ void gen_nonce(unsigned int idx, unsigned char *str, unsigned char *memory, bool print=false) {
	for (int i = 0; i < 32; i += 2) {
		str[i] = "0123456789abcdef"[memory[idx] >> 4];
		str[i+1] = "0123456789abcdef"[memory[(idx += 997525853) %= MEM_SZ] & 0xf];
	}
	if (print) 
		printf("%s\n", str);
}

__global__ void zero(unsigned char *str) {
	str[32] = 0;
}

__global__ void ihatemylife(long long tmp, long long *val) {
	*val = tmp;
}

int main() {
	unsigned char *memory;
	auto res = cudaMalloc(&memory, MEM_SZ);
	if (res != cudaSuccess) {
		std::cout << "Error allocating memory: " << cudaGetErrorString(res) << "\n";
		return 1;
	}
	std::cout << "Seed: ";
	long long *seed, tmp;
	res = cudaMalloc(&seed, 8);
	if (res != cudaSuccess) {
		std::cout << "Error allocating memory: " << cudaGetErrorString(res) << "\n";
		return 1;
	}
	std::cin >> tmp;
	ihatemylife<<<1, 1>>>(tmp, seed);
	std::cout << "Filling memory...\n";
	clock_t start = clock();
	fill_memory<<<1, 1>>>(memory, seed);
	// wait for kernel to finish
	cudaDeviceSynchronize();
	clock_t end = clock();
	std::cout << "Success! " << ((double)(end-start) / CLOCKS_PER_SEC) << " seconds elapsed.\n";
	std::cout << "Testing nonce generation speed...\n";
	unsigned char *tmpnonce;
	res = cudaMalloc(&tmpnonce, 33);
	if (res != cudaSuccess) {
		std::cout << "Error allocating memory: " << cudaGetErrorString(res) << "\n";
		return 1;
	}
	zero<<<1, 1>>>(tmpnonce);
	// error is below here
	start = clock();
	for (int i = 0; i < 33554432; i++) {
		gen_nonce<<<1, NUM_THREADS>>>(i, tmpnonce, memory);
	}
	cudaDeviceSynchronize();
	end = clock();
	// errir is above here
	res = cudaGetLastError();
	if (res != cudaSuccess) {
		std::cout << "Error: " << cudaGetErrorString(res) << "\n";
		return 1;
	}
	double speed = (double)CLOCKS_PER_SEC / (end - start);
	speed *= NUM_THREADS;
	std::cout << "Speed: " << std::fixed << speed << " GB / s\n";
	std::cout << "Speed: " << speed * 33.554432 << " MNonces / sec\n";
	for (int i = 0; i < 4; i++) {
		gen_nonce<<<1, 1>>>(i, tmpnonce, memory, true);
		cudaDeviceSynchronize();
	}
	// get last error
	res = cudaGetLastError();
	if (res != cudaSuccess) {
		std::cout << "Error: " << cudaGetErrorString(res) << "\n";
		return 1;
	}
	return 0;
}