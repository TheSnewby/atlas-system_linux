; strchr
; @rdi: string
; @rsi: character
;
; returns: memory location of character in string in rax, or NULL if not found

section .text
	global asm_strchr

asm_strchr:
	; while not at null byte, check for character at location
	; return location if found, otherwise return 0?
.loop:
	mov al, byte [rdi]

	cmp al, sil ; sil is 8-bit register of rsi
	je .found

	cmp al, 0
	je .notfound

	inc rdi

	jmp .loop

.notfound:
	xor rax, rax
	ret

.found:
	mov rax, rdi
	ret
