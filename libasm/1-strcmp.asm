; strcmp()
; @rdi: first  param
; @rsi: second param
; return: rax an integer less than, equal to, or greater than zero if s1
; 	(or the first n bytes thereof) is found, respectively,  to  be  less  than,
;	to match, or be greater than s2

section .text
	global asm_strcmp

asm_strcmp:
.loop:
	movzx eax, byte [rdi] ; zero extended to handle signed integer
	movzx ebx, byte [rsi]

	cmp al, bl
	jne .notequal

	test al, al ; test is 0 if rdi is a null byte
	je .equal ; jump if equal

	inc rdi
	inc rsi
	jmp .loop

.notequal:
	sub eax, ebx
	cmp eax, 0
	jg .greater
	jl .lesser

.greater:
	mov eax, 1
	ret

.lesser:
	mov eax, -1
	ret

.equal:
	xor eax, eax
	ret
