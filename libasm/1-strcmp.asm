; strcmp()
; @rdi: first  param
; @rsi: second param
; return: rax an integer less than, equal to, or greater than zero if s1
; 	(or the first n bytes thereof) is found, respectively,  to  be  less  than,
;	to match, or be greater than s2

section .text
	global asm_strcmp
	BITS 64

asm_strcmp:
.loop:
	movzx rax, byte [rdi] ; zero extended to handle signed integer
	movzx rbx, byte [rsi]

	cmp al, bl
	jne .notequal

	test al, al ; test is 0 if rdi is a null byte
	je .equal ; jump if equal

	inc rdi
	inc rsi
	jmp .loop

.notequal:
	sub rax, rbx
	cmp rax, 0
	jg .greater
	jl .lesser

.greater:
	mov rax, 1
	ret

.lesser:
	mov rax, -1
	ret

.equal:
	xor rax, rax
	ret
