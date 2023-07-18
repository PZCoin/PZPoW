all:
	gcc -c -o gf.c.o gf.c -march=native -g -fsanitize=address
	nasm -felf64 gf.asm -o gf.asm.o
	gcc -o gf gf.c.o gf.asm.o -fsanitize=address
