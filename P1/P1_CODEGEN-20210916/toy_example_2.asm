segment .data
	msg_error_division db "Error division por 0", 0
        ;more error messages here
 
segment .bss
	__esp resd 1
	_m resd 1

segment .text
	global main
	extern scan_int, print_int, scan_boolean, print_boolean
	extern print_endofline, print_blank, print_string

; -----------------------
; Main function
main:
	mov dword [__esp], esp
	push dword 100
	pop dword eax
	add [_m], 100
	mov dword [_m], eax
	push [_m]
	call print_int
	add esp, 4
ret
