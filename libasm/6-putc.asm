; putchar
; @rdi: int cast to unsigned char
;
; Return: rax with 1 on success

section .text
	global asm_putc

asm_putc:
	cmp rdi, 0
	jle .error

	cmp rdi, 122
	jge .error

	add rsp, -1			; Increase stack by 1 byte to store a single char
	mov [rsp], dil		; Address of the character to be printed
	mov rsi, rsp		; load memory location into source pointer

	mov rax, 1 			; Write syscall
	mov rdi, 1			; write to StdOut
	mov rdx, 1			; Write 1 byte
	syscall				; syscall #rax

	add rsp, 1			; reset stack

	ret

.error:
	mov rax, -1
	ret
