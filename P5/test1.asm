;D:		main
segment .data
msg_error_division db "Error division by 0 ", 0
msg_error_rango db "Error out of range ", 0
segment .bss
__esp resd 1
;D:		{
;D:		int
;R10:	<tipo> ::= int
;R9:	<clase_escalar> ::= <tipo>
;R5:	<clase> ::= <clase_escalar>
;D:		x
;R108:	<identificador> ::= TOK_IDENTIFICADOR
_x resd 1
;D:		;
;R18:	<identificadores> ::= <identificador>
;R4:	<declaracion> ::= <clase> <identificadores> ;
;D:		function
;R2:	<declaraciones> ::= <declaracion>
segment .text
global main
extern scan_int, print_int, scan_boolean, print_boolean
extern print_endofline, print_blank, print_string
;D:		int
;R10:	<tipo> ::= int
;D:		abc
;D:		(
;D:		int
;R10:	<tipo> ::= int
;D:		a
;R27:	<parametro_funcion> ::= <tipo> <identificador>
;D:		)
;R26:	<resto_parametros_funcion> ::=
;R23:	<parametros_funcion> ::= <parametro_funcion> <resto_parametros_funcion>
_abc:
push dword ebp
mov dword ebp, esp
sub esp, 4*0
;D:		{
;D:		int
;R10:	<tipo> ::= int
;R9:	<clase_escalar> ::= <tipo>
;R5:	<clase> ::= <clase_escalar>
;D:		y
;R108:	<identificador> ::= TOK_IDENTIFICADOR
;D:		;
;R18:	<identificadores> ::= <identificador>
;R4:	<declaracion> ::= <clase> <identificadores> ;
;D:		y
;R2:	<declaraciones> ::= <declaracion>
;R28:	<declaraciones_funcion> ::= <declaraciones>
;D:		=
;D:		15
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 15
;D:		;
;R43:	<asignacion> ::= <identificador> = <exp>
;local: 0
lea eax, [ebp - 0]
push dword eax
pop dword ebx
pop dword eax
mov dword [ebx], eax
;R34:	<sentencia_simple> ::= <asignacion>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		printf
;D:		a
;D:		*
;R80:	<exp> ::= <identificador>
lea eax, [ebp + 8]
push dword eax
;D:		y
;D:		;
;R80:	<exp> ::= <identificador>
;loc:0
lea eax, [ebp - 0]
push dword eax
;R75:	<exp> ::= <exp> * <exp>
pop dword ebx
mov dword ebx, [ebx]
pop dword eax
mov dword eax, [eax]
cdq
imul ebx
push dword eax
;R56:	<escritura> ::= printf <exp>
call print_int
add esp, 4
call print_endofline
;R36:	<sentencia_simple> ::= <escritura>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		return
;D:		y
;D:		-
;R80:	<exp> ::= <identificador>
;loc:0
lea eax, [ebp - 0]
push dword eax
;D:		a
;D:		;
;R80:	<exp> ::= <identificador>
lea eax, [ebp + 8]
push dword eax
;R73:	<exp> ::= <exp> - <exp>
pop dword ebx
mov dword ebx, [ebx]
pop dword eax
mov dword eax, [eax]
sub eax, ebx
push dword eax
;R61:	<retorno_funcion> ::= return <exp>
pop dword eax
mov dword esp, ebp
pop dword ebp
ret
;R38:	<sentencia_simple> ::= <retorno_funcion>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		}
;R30:	<sentencias> ::= <sentencia>
;R31:	<sentencias> ::= <sentencia> <sentencias>
;R31:	<sentencias> ::= <sentencia> <sentencias>
;R22:	<funcion> ::= function <tipo> <identificador> ( <parametros_funcion> ) { <declaraciones_funcion> <sentencias> }
;D:		x
;R21:	<funciones> ::=
;R20:	<funciones> ::= <funcion> <funciones>
main:
mov dword [__esp], esp
;D:		=
;D:		abc
;D:		(
;D:		5
;R104:	<constante_entera> ::= TOK_CONSTANTE_ENTERA
;R100:	<constante> ::= <constante_entera>
;R81:	<exp> ::= <constante>
push dword 5
;D:		)
;R92:	<resto_lista_expresiones> ::=
;R89:	<lista_expresiones> ::= <exp> <resto_lista_expresiones>
;R88:	<exp> ::= <identificador> ( <lista_expresiones> )
call _abc
add esp, 4*1
push dword eax
;D:		;
;R43:	<asignacion> ::= <identificador> = <exp>
;local: -1
pop dword eax
mov dword [_x], eax
;R34:	<sentencia_simple> ::= <asignacion>
;R32:	<sentencia> ::= <sentencia_simple> ;
;D:		printf
;D:		x
;D:		;
;R80:	<exp> ::= <identificador>
;loc:-1
push dword _x
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
