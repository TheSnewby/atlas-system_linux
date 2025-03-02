; strlen
; rdi always has first param stored
; rax has return value

section .text
	global asm_strlen
asm_strlen:
	mov rax, 0

.loop:
	cmp byte [rdi], 0 ; compare current byte of rax with null byte
	je .done ; jump to done if equal
	inc rax ; increment counter
	inc rdi ; increment string pointer
	jmp .loop ; jump

.done:
	ret
