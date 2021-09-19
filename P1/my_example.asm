segment .data
abyte db 0
aword dw 10
adoubleword dd 100
div_error_message db "Division by 0", 0


segment .bss
_d resd 1
segment .text

global main
extern print_int
main:
    mov eax, 2
    push eax
    call print_int
    add esp, 4
    mov eax, 0
 ret

