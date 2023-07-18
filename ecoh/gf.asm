section .text
global GF_add
global GF_sub
global GF_copy

align 16
GF_add:
GF_sub:
	vmovdqu ymm0, [rsi]
	mov eax, [rsi+32]
	vpxor ymm0, ymm0, [rdx]
	xor eax, [rdx+32]
	vmovdqu [rdi], ymm0
	mov [rdi+32], eax
	ret

GF_copy:
	vmovdqu ymm0, [rsi]
	vmovdqu [rdi], ymm0
	mov eax, [rsi+32]
	mov [rdi+32], eax
	ret
