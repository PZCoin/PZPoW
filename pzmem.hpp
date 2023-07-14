#include <string>
#include <random>

#define PZ_MEM_SZ (1LL<<32)

class PZ_MEM {
private:
	unsigned char *memory;
	unsigned int seed;
	std::mt19937_64 rng;

	void fill_memory();

public:
	void gen_nonce(unsigned int, unsigned char *);

	void init(int);
};