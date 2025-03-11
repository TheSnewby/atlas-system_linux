; strpbrk - find the first location in string s that contains any char from accept
; @rdi string s
; @rsi string accept
;
; returns: location of accept segment in string s, or null if not found

section .text
	global asm_strpbrk

asm_strpbrk:
	push rdi
	push rsi

.outerloop:
	mov bl, byte [rdi]

	pop rsi
	push rsi

	cmp bl, 0
	je .notfound

.innerloop:
	mov cl, byte [rsi]
	cmp cl, 0
	je .outerloopincrement
	cmp bl, cl
	je .found

	inc rsi
	jmp .innerloop

.outerloopincrement:
	inc rdi
	jmp .outerloop

.found:
	mov rax, rdi
	pop rsi
	pop rdi
	ret

.notfound:
	xor rax, rax
	pop rsi
	pop rdi
	ret
