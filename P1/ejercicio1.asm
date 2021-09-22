segment .data
 
segment .bss

segment .text
	global main
	extern scan_int, print_int, scan_boolean, print_boolean
	extern print_endofline, print_blank, print_string

        ; -----------------------
        ; Main function
        main:
                mov dword eax, 100
                cdq
                mov dword ecx, 2

                idiv ecx

                push dword eax
                call print_int
                add esp, 4

                cmp eax, 0
                jle ending

                call print_endofline

                ending:
        ret
