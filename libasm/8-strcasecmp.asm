; strcasecmp
; @rdi mem address of string 1
; @rsi mem address of string 2
;
; return: rax with the value of rdi - rsi for caseless comparison

global asm_strcasecmp

section .text

asm_strcasecmp:
	; check for nulls
	; cast all letters to lower case (arbitrarily chosen, could be upper)
	; compare a character at a time
	; loop through until reaching an end of a string

	push rdi
	push rsi

.loopStart:
	movzx rax, byte [rdi]	; load 0-extended value into free register
	movzx rcx, byte [rsi]	; load 0-extended value into free register
	cmp rax, 0				; check if empty
	je  .fin
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
	jne .fin				; if not equal finish

	inc rdi
	inc rsi
	jmp .loopStart

.fin:
	pop rsi
	pop rdi
	ret
