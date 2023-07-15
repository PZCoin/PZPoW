# PZPoW
Proof of work system for PZCoin

## How it works

1. Generate 4GB of random data from a seed (block number) and store it in memory
2. Generate 32-character (hex) nonces from this data by doing jumps of size 997525853 in the data (wraps around)
3. Select a random 8-character (hex) extra nonce from to append to the 32-byte nonce
4. The 40-byte nonce can now be hashed with the block header using ECOH to find a valid hash