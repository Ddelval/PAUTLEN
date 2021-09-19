segment .data
 
segment .bss
	_a resd 1

segment .text
	global main
	extern scan_int, print_int, scan_boolean, print_boolean
	extern print_endofline, print_blank, print_string

; -----------------------
; Main function
main:
 	mov eax, 100
	mov [_a], eax
 	mov edx, [_a]
 	add eax, edx
 	push eax
 	call print_int
 	add esp, 4
 ret


