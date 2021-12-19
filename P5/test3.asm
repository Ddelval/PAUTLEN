;D:		main
segment .data
msg_error_division db "Error division by 0 ", 0
msg_error_rango db "Error out of range ", 0
segment .bss
__esp resd 1
;D:		{
;D:		array
;D:		int
;R10:	<tipo> ::= int
;D:		[
;D:		3
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;D:		]
;R15:	<clase_vector> ::= array <tipo> [ <constante_entera> ]
;R7:	<clase> ::= <clase_vector>
;D:		v
;R108:	<identificador> ::= TOK_IDENTIFICADOR
_v resd 3
;D:		;
;R18:	<identificadores> ::= <identificador>
;R4:	<declaracion> ::= <clase> <identificadores> ;
;D:		v
;R2:	<declaraciones> ::= <declaracion>
segment .text
global main
extern scan_int, print_int, scan_boolean, print_boolean
extern print_endofline, print_blank, print_string
;R21:	<funciones> ::=
main:
mov dword [__esp], esp
;D:		[
;D:		0
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 0
;D:		]
;R48:	<elemento_vector> ::= <identificador> [ <exp> ]
add esp, 4*1
;D:		=
;D:		1
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 1
;D:		;
;R44:	<asignacion> ::= <elemento_vector> = <exp>
push dword 0
pop dword eax
cmp eax, 0
jl fin_error_rango
cmp eax, 3
jge near fin_error_rango
mov dword edx, _v
lea eax, [edx + eax*4]
push dword eax
pop dword ebx
pop dword eax
mov dword [ebx], eax
;R34:	<sentencia_simple> ::= <asignacion>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		v
;D:		[
;D:		1
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 1
;D:		]
;R48:	<elemento_vector> ::= <identificador> [ <exp> ]
add esp, 4*1
;D:		=
;D:		2
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 2
;D:		;
;R44:	<asignacion> ::= <elemento_vector> = <exp>
push dword 1
pop dword eax
cmp eax, 0
jl fin_error_rango
cmp eax, 3
jge near fin_error_rango
mov dword edx, _v
lea eax, [edx + eax*4]
push dword eax
pop dword ebx
pop dword eax
mov dword [ebx], eax
;R34:	<sentencia_simple> ::= <asignacion>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		v
;D:		[
;D:		2
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 2
;D:		]
;R48:	<elemento_vector> ::= <identificador> [ <exp> ]
add esp, 4*1
;D:		=
;D:		3
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 3
;D:		;
;R44:	<asignacion> ::= <elemento_vector> = <exp>
push dword 2
pop dword eax
cmp eax, 0
jl fin_error_rango
cmp eax, 3
jge near fin_error_rango
mov dword edx, _v
lea eax, [edx + eax*4]
push dword eax
pop dword ebx
pop dword eax
mov dword [ebx], eax
;R34:	<sentencia_simple> ::= <asignacion>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		printf
;D:		4
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 4
;D:		+
;D:		(
;D:		-
;D:		2
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 2
;R76:	<exp> ::= - <exp>
pop dword eax
neg dword eax
push dword eax
;D:		)
;R82:	<exp> ::= ( <exp> )
;D:		;
;R72:	<exp> ::= <exp> + <exp>
pop dword ebx
pop dword eax
add eax, ebx
push dword eax
;R56:	<escritura> ::= printf <exp>
call print_int
add esp, 4
call print_endofline
;R36:	<sentencia_simple> ::= <escritura>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		printf
;D:		v
;D:		[
;D:		4
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 4
;D:		+
;D:		(
;D:		-
;D:		2
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 2
;R76:	<exp> ::= - <exp>
pop dword eax
neg dword eax
push dword eax
;D:		)
;R82:	<exp> ::= ( <exp> )
;D:		]
;R72:	<exp> ::= <exp> + <exp>
pop dword ebx
pop dword eax
add eax, ebx
push dword eax
;R48:	<elemento_vector> ::= <identificador> [ <exp> ]
add esp, 4*1
;R85:	<exp> ::= <elemento_vector>
push dword 2
pop dword eax
cmp eax, 0
jl fin_error_rango
cmp eax, 3
jge near fin_error_rango
mov dword edx, _v
lea eax, [edx + eax*4]
push dword eax
;D:		;
;R56:	<escritura> ::= printf <exp>
pop dword eax
mov dword eax, [eax]
push dword eax
call print_int
add esp, 4
call print_endofline
;R36:	<sentencia_simple> ::= <escritura>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		}
;R30:	<sentencias> ::= <sentencia>
;R31:	<sentencias> ::= <sentencia> <sentencias>
;R31:	<sentencias> ::= <sentencia> <sentencias>
;R31:	<sentencias> ::= <sentencia> <sentencias>
;R31:	<sentencias> ::= <sentencia> <sentencias>
;R1:	<programa> ::= main { <declaraciones> <funciones> <sentencias> }
mov eax, 0
jmp near fin
fin_error_division:
push dword msg_error_division
call print_string
add esp, 4
call print_endofline
jmp near fin
mov eax, 1
fin_error_rango:
push dword msg_error_rango
call print_string
add esp, 4
call print_endofline
jmp near fin
mov eax, 1
fin:
mov dword esp, [__esp]
ret
