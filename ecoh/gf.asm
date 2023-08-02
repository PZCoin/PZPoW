; @file gf.hpp
; @author wdotmathree (wdotmathree@gmail.com)
; @brief Enables operations on elements over GF(2^283)
; @date 2023-08-01
;
; @copyright Copyright (c) 2023

section .text
global GF_equal
global GF_add
global GF_sub
global GF_double
global GF_copy

align 16
GF_equal:
	xor eax, eax
	xor edx, edx
	xor ecx, ecx
	mov r8, [rdi]
	cmp r8, [rsi]
	sete al
	mov r8, [rdi+8]
	cmp r8, [rsi+8]
	sete dl
	and eax, edx
	mov r8, [rdi+16]
	cmp r8, [rsi+16]
	sete cl
	mov r8, [rdi+24]
	cmp r8, [rsi+24]
	sete dl
	and ecx, edx
	mov ecx, [rdi+32]
	cmp ecx, [rsi+32]
	sete dl
	and eax, edx
	and eax, ecx
	ret

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

align 16
GF_double:
	mov rax, [rsi]
	shl rax, 1
	mov [rdi], rax
	mov rax, [rsi+8]
	rcl rax, 1
	mov [rdi+8], rax
	mov rax, [rsi+16]
	rcl rax, 1
	mov [rdi+16], rax
	mov rax, [rsi+24]
	rcl rax, 1
	mov [rdi+24], rax
	mov eax, [rsi+32]
	rcl eax, 1
	mov [rdi+32], eax
	ret

align 16
GF_copy:
	vmovdqu ymm0, [rsi]
	mov eax, [rsi+32]
	vmovdqu [rdi], ymm0
	mov [rdi+32], eax
	ret
