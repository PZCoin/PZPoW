# DEBUG=
# OPTIMIZE=-O2

DEBUG=-g
OPTIMIZE=

all:
	g++ -c -o gf.c.o gf.c -march=native ${DEBUG} ${OPTIMIZE} -fsanitize=address
	nasm -felf64 gf.asm -o gf.asm.o ${DEBUG}
	g++ -c -o ec.cpp.o ec.cpp -march=native ${DEBUG} ${OPTIMIZE} -fsanitize=address
	nasm -felf64 ec.asm -o ec.asm.o ${DEBUG}
	g++ -o ec ec.cpp.o ec.asm.o gf.c.o gf.asm.o ${DEBUG} ${OPTIMIZE} -fsanitize=address
