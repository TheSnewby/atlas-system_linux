; puts
; @rdi: memory address of string
;
; Return: rax with number of characters printed, negative if error

section .text
	extern asm_strlen
	global asm_puts

asm_puts:
	call asm_strlen			; length of string
	cmp rax, 0
	; cmovl rdx, -1				; if <= 0, return an error
	; cmovge rdx, rax			; store the length into the string counter register
	mov rdx, rax

	mov rsi, rdi			; load memory location into source pointer

	mov rax, 1				; Write syscall
	mov rdi, 1				; write to StdOut
	syscall

	mov rax, rdx
	ret

