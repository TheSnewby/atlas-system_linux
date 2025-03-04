; strncmp()
; @rdi: first param		1st string
; @rsi: second param	2nd string
; @rdx: third param		number of bytes to compare (n)
; return: rax an integer less than, equal to, or greater than zero if s1
; 	(or the first n bytes thereof) is found, respectively,  to  be  less  than,
;	to match, or be greater than s2

section .text
	global asm_strncmp
	BITS 64

asm_strncmp:
	mov rcx, 0
.loop:
	movzx rax, byte [rdi] ; zero extended to handle signed integer
	movzx rbx, byte [rsi]

	cmp rcx, rdx ; compare counter with n
	je .done

	cmp al, bl
	jne .done

	test al, al ; test is 0 if rdi is a null byte
	je .done ; jump if equal

	inc rdi
	inc rsi
	inc rcx
	jmp .loop

.done:
	sub rax, rbx
	ret
; 	jg .greater
; 	jl .lesser
; 	je .equal

; .greater:
; 	mov rax, 1
; 	ret

; .lesser:
; 	mov rax, -1
; 	ret

; .equal:
; 	xor rax, rax
; 	ret
