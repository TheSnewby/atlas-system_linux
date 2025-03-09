; puts
; @rdi: memory address of string
;
; Return: rax with number of characters printed, negative if error

section .text
	global asm_puts

asm_puts:
	call asm_strlen			; length of string
	cmp rax, 0
	jl .error				; if <= 0, return an error
	mov rcx, rax			; store the length into the string counter register
	push rax				; store value to later retrieval

	mov rsp, rdi

	mov rsi, rsp
	mov rax, 1
	mov rdi, 1
	mov rdx, rcx
	syscall


.error:
	mov rax, -1
	ret

.fin:
	pop rax					; maybe?
	ret
