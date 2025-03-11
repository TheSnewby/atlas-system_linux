; strcspn - returns length of initial segment of s that contains no chars from reject string
; @rdi string s
; @rsi string reject
;
; returns: length

section .text
	global asm_strcspn

asm_strcspn:
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
	je .outerloopincrement
	cmp bl, cl
	je .fin

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
