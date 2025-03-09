; strncasecmp
; @rdi mem address of string 1
; @rsi mem address of string 2
; @rdx number of characters
; return: rax with the value of rdi - rsi for caseless comparison

global asm_strncasecmp

section .text

asm_strncasecmp:
	; check for nulls
	; cast all letters to lower case (arbitrarily chosen, could be upper)
	; compare a character at a time
	; loop through until reaching an end of a string

	push rdi
	push rsi

	cmp rdx, 0
	jle .earlyexit

	mov rbx, 0				; counter for number number of chars compared

.loopStart:
	inc rbx
	movzx rax, byte [rdi]	; load 0-extended value into free register
	movzx rcx, byte [rsi]	; load 0-extended value into free register
	cmp rax, 65				; check if at least upper case
	jge .checkS1Upper		; jump if S1 character could be Uppercase
	jl  .checkS2			; jump to check S2 for Uppercase

.checkS1Upper:
	cmp rax, 90
	jle .castS1Lower
	jg  .checkS2

.castS1Lower:
	add rax, 32

.checkS2:
	cmp rcx, 65
	jge .checkS2Upper
	jl  .compChars

.checkS2Upper:
	cmp rcx, 90
	jle .castS2Lower
	jg .compChars

.castS2Lower:
	add rcx, 32

.compChars:
	sub rax, rcx			; subtract S2 char from S1 char
	cmp rax, 0
	jne .finne				; if not equal finish

	cmp byte [rdi], 0		; check if at end of S1
	je  .fin

	cmp byte [rsi], 0		; probably not needed
	je  .fin

	cmp rbx, rdx			; compares counter vs n-character limit
	je 	.fin

	inc rdi
	inc rsi
	jmp .loopStart

.earlyexit:
	pop rsi
	pop rdi
	xor rax, rax
	ret

.finne:						; fin not equal
	movzx rax, byte [rdi]	; load original values
	movzx rcx, byte [rsi]	; load original values
	sub rax, rcx			; compare actual values
	pop rsi
	pop rdi
	ret

.fin:						; fin
	xor rax, rax
	pop rsi
	pop rdi
	ret

