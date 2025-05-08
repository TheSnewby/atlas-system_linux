; addware, per Daniel's Request - prints the sum of two ints
; rdi: 1st int
; rsi: 2nd int
;
; return: void

section .text
	global addware

addware:
	add rdi, rsi
	push rbp
    mov rbp, rsp
    sub rsp, 24             ; Buffer for digits

    mov rax, rdi            ; Number to RAX
    mov r8, 10              ; Divisor
    lea rsi, [rsp+23]       ; Point to end of buffer
    mov byte [rsi], 0       ; Null terminator

.negative:					; Handles printing negative sign
    test rax, rax           ; Check if negative by setting the sign flag nz
    jns .positive
    neg rax					; turns rax positive
    push rax				; stores value of rax in the stack
    mov rdi, '-'			; loads variable for printing negative sign
    call print_char
    pop rax					; pops the stack into rax

.positive:
    mov rcx, rax
    test rcx, rcx			; check if zero
    jnz .convert_to_ASCII_loop	; Jumps if not zero
    mov byte [rsi-1], '0'   ; Handle zero case
    dec rsi
    jmp .print_buffer

.convert_to_ASCII_loop:
    xor rdx, rdx            ; Clear RDX for division
    div r8                  ; Divide by 10
    add dl, '0'             ; Convert to ASCII
    dec rsi                 ; Move buffer pointer
    mov [rsi], dl           ; Store digit
    test rax, rax           ; Check if done
    jnz .convert_to_ASCII_loop

.print_buffer:
    mov rdi, 1              ; stdout
    mov rdx, rsp
    add rdx, 23
    sub rdx, rsi            ; Calculate length
    mov rax, 1              ; sys_write (syscall #1)
    syscall					; executes command stored in rax

    mov rsp, rbp
    pop rbp
    ret

; Input: character in RDI
print_char:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    mov [rsp], dil
    mov rax, 1              ; sys_write
    mov rdi, 1              ; stdout
    mov rsi, rsp            ; Buffer
    mov rdx, 1              ; Length
    syscall					; executes command stored in rax
    mov rsp, rbp
    pop rbp
    ret						; returns to caller
