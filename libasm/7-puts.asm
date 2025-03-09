; puts
; @rdi: memory address of string
;
; Return: rax with number of characters printed, negative if error

section .text
	extern asm_strlen
	global asm_puts

asm_puts:
	push rdi				; saves
	call asm_strlen
	mov rdx, rax

	pop rsi
	mov rax, 1				; Write syscall
	mov rdi, 1				; write to StdOut
	syscall

	ret
