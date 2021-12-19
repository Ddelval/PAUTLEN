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
;D:		)
;R24:	<parametros_funcion> ::=
