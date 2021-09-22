segment .data
	msg_error_division db "Error division por 0", 0
 
segment .bss
	_v1 resd 1
        _v2 resd 1

segment .text
	global main
	extern scan_int, print_int, scan_boolean, print_boolean
	extern print_endofline, print_blank, print_string

        ; -----------------------
        ; Main function
        main:
                push dword _v1
                call scan_int
                add esp, 4
                cmp eax, 0
                jl badscan

                push dword _v2
                call scan_int
                add esp, 4
                cmp eax, 0
                jl badscan
                cmp dword [_v2], 0
                je divzero

                mov dword eax, [_v1]
                cdq
                idiv dword [_v2]

                push dword eax
                call print_int
                add esp, 4
                cmp eax, 0
                jle badscan

                call print_endofline
                jmp ending

                divzero:
                        push dword msg_error_division
                        call print_string
                        add esp, 4

                        cmp eax, 0
                        jle badscan

                        call print_endofline

                badscan:
                ending:
        ret
