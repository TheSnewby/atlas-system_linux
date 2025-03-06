; strstr
; @rdi: haystack string
; @rsi: needle string
;
; return: rax with location of beginning of str match, or null if not found

section .text
	global asm_strstr

asm_strstr:
	; do check for first character match
	; do check for subsequent characters to match
	; if not, return to next first char match
	; while not at null byte
.outerLoop:
	mov al, byte [rdi]
	mov bl, byte [rsi]

	cmp al, 0
	je .notFound

	cmp al, bl
	je .matchingFirstChar

	inc rdi
	jmp .outerLoop


.matchingFirstChar:
	mov rbx, rdi
	mov rcx, rsi

.innerLoop:
	inc rbx
	inc rcx

	mov al, byte [rbx]
	mov bl, byte [rcx]

	cmp bl, 0
	je .found ; check if needle is at end

	cmp al, 0
	je .notFound

	cmp al, bl
	je .innerLoop

	inc rdi
	jmp .outerLoop

.notFound:
	xor rax, rax
	ret

.found:
	mov rax, rdi
	ret
