; memcpy
; @rdi: dest string
; @rsi: src string
; @rdx: number of characters to be copied
;
; return: rax with memory locaiton of dest

section .text
	global asm_memcpy

asm_memcpy:
	; check if either is null
	; loop through src until either is null or count is reached
	; return rax
	cmp rdx, 0
	je .fin
	jle .fin

	mov rcx, 0

.loop:
	mov al, byte [rdi + rcx]
	mov bl, byte [rsi + rcx]

	cmp al, 0
	je .fin

	cmp bl, 0
	je .fin

	mov byte [rdi + rcx], bl ; copies *src to *dest

	inc rcx
	cmp rcx, rdx
	je .fin

	jmp .loop

.fin:
	mov rax, rdi
	ret
