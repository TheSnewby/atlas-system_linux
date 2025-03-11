; strspn - returns length of initial segment of s that contains only chars from accept
; @rdi string s
; @rsi string accept
;
; returns: length

section .text
	global asm_strspn

asm_strspn:
	push rdi
	push rsi
	xor rax, rax

.outerloop:
	mov bl, byte [rdi]

	pop rsi
	push rsi

	cmp bl, 0
	je .fin

.innerloop:
	mov cl, byte [rsi]
	cmp cl, 0
	je .fin
	cmp bl, cl
	je .outerloopincrement

	inc rsi
	jmp .innerloop

.outerloopincrement:
	inc rdi
	inc rax
	jmp .outerloop

.fin:
	pop rsi
	pop rdi
	ret
