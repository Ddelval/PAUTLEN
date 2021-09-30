#include "generacion.h"

/** PRIVATE PROTOTYPES **/
void append_string(char **dest, const char *line);

/* OBSERVACIÓN GENERAL A TODAS LAS FUNCIONES:
Todas ellas escriben el código NASM a un FILE* proporcionado como primer
argumento.
*/

void escribir_cabecera_bss(FILE *fpasm) {
    const char *assembler_string = "segment .bss\n"
                                   "__esp resd 1\n";

    fprintf(fpasm, "%s", assembler_string);
}
/*
Código para el principio de la sección .bss.
Con seguridad sabes que deberás reservar una variable entera para guardar el
puntero de pila extendido (esp). Se te sugiere el nombre __esp para esta
variable.
*/

void escribir_subseccion_data(FILE *fpasm) {
    const char *assembler_string =
        "segment .data\n"
        "msg_error_division db \"Error division por 0 \", 0\n"
        "msg_error_rango db \"Error out of range \", 0\n";

    fprintf(fpasm, "%s", assembler_string);
}
/*
Declaración (con directiva db) de las variables que contienen el texto de los
mensajes para la identificación de errores en tiempo de ejecución.
En este punto, al menos, debes ser capaz de detectar la división por 0.
*/

void declarar_variable(FILE *fpasm, char *nombre, int tipo, int tamano) {
    const char *assembler_string = "_%s resd %d\n";
    fprintf(fpasm, assembler_string, nombre, tamano);
}
/*
Para ser invocada en la sección .bss cada vez que se quiera declarar una
variable:
- El argumento nombre es el de la variable.
- tipo puede ser ENTERO o BOOLEANO (observa la declaración de las constantes
del principio del fichero).
- Esta misma función se invocará cuando en el compilador se declaren
vectores, por eso se adjunta un argumento final (tamano) que para esta
primera práctica siempre recibirá el valor 1.
*/

void escribir_segmento_codigo(FILE *fpasm) {
    const char *assembler_string =
        "segment .text\n"
        "global main\n"
        "extern scan_int, print_int, scan_boolean, print_boolean\n"
        "extern print_endofline, print_blank, print_string\n";
    fprintf(fpasm, "%s", assembler_string);
}
/*
Para escribir el comienzo del segmento .text, básicamente se indica que se
exporta la etiqueta main y que se usarán las funciones declaradas en la librería
alfalib.o
*/

void escribir_inicio_main(FILE *fpasm) {
    const char *assembler_string = "main:\n"
                                   "mov [__esp], esp\n";
    fprintf(fpasm, "%s", assembler_string);
}
/*
En este punto se debe escribir, al menos, la etiqueta main y la sentencia que
guarda el puntero de pila en su variable (se recomienda usar __esp).
*/

void escribir_fin(FILE *fpasm) {
    const char *assembler_string = "mov  esp, [__esp]\n"
                                   "ret\n";
    fprintf(fpasm, "%s\n", "jmp near fin");

    // Error division
    fprintf(fpasm, "%s\n", "fin_error_division:");
    fprintf(fpasm, "%s\n", "push dword msg_error_division");
    fprintf(fpasm, "%s\n", "call print_string");
    fprintf(fpasm, "%s\n", "add esp, 4");
    fprintf(fpasm, "%s\n", "call print_endofline");
    fprintf(fpasm, "%s\n", "jmp near fin");

    // Error range
    fprintf(fpasm, "%s\n", "fin_error_rango:");
    fprintf(fpasm, "%s\n", "push dword msg_error_rango");
    fprintf(fpasm, "%s\n", "call print_string");
    fprintf(fpasm, "%s\n", "add esp, 4");
    fprintf(fpasm, "%s\n", "call print_endofline");
    fprintf(fpasm, "%s\n", "jmp near fin");

    // fin
    fprintf(fpasm, "%s\n", "fin:");
    fprintf(fpasm, "%s", assembler_string);
}
/*
Al final del programa se escribe:
- El código NASM para salir de manera controlada cuando se detecta un error
en tiempo de ejecución (cada error saltará a una etiqueta situada en esta
zona en la que se imprimirá el correspondiente mensaje y se saltará a la
zona de finalización del programa).
- En el final del programa se debe:
·Restaurar el valor del puntero de pila (a partir de su variable __esp)
·Salir del programa (ret).
*/

void escribir_operando(FILE *fpasm, char *nombre, int es_variable) {
    char *assembler_string = NULL;
    if (es_variable) {
        assembler_string = "push dword _%s\n";
    } else {
        assembler_string = "push dword %s\n";
    }
    fprintf(fpasm, assembler_string, nombre);
}
/*
Función que debe ser invocada cuando se sabe un operando de una operación
aritmético-lógica y se necesita introducirlo en la pila.
- nombre es la cadena de caracteres del operando tal y como debería aparecer
en el fuente NASM
- es_variable indica si este operando es una variable (como por ejemplo b1)
con un 1 u otra cosa (como por ejemplo 34) con un 0. Recuerda que en el
primer caso internamente se representará como _b1 y, sin embargo, en el
segundo se representará tal y como esté en el argumento (34).
*/

void asignar(FILE *fpasm, char *nombre, int es_variable) {
    fprintf(fpasm, "%s\n", "pop eax");
    if (es_variable) {
        fprintf(fpasm, "%s\n", "mov eax, [eax]");
    }
    fprintf(fpasm, "mov [_%s], eax\n", nombre);
}
/*
- Genera el código para asignar valor a la variable de nombre nombre.
- Se toma el valor de la cima de la pila.
- El último argumento es el que indica si lo que hay en la cima de la pila es
una referencia (1) o ya un valor explícito (0).
*/

/**
 * Generate the code to pop the operands from the stack
 * the integer arguments indicate whether the variables are
 * stored in the stack as pointers or as values
 */
void pop_values_for_operation(FILE *fpasm, int es_variable_1,
                              int es_variable_2) {
    // Note that variables shall be popped from the stack in
    // the reverse order
    fprintf(fpasm, "%s\n", "pop dword ebx");
    if (es_variable_2) {
        fprintf(fpasm, "%s\n", "mov dword ebx, [ebx]");
    }

    fprintf(fpasm, "%s\n", "pop dword eax");
    if (es_variable_1) {
        fprintf(fpasm, "%s\n", "mov dword eax, [eax]");
    }
}

/**
 * Performs a generic binary operation.
 * This function can only be used for operations that can be implemented
 * exactly like an addition. For instance, this function cannot handle
 * a multiplication
 */
void simple_operation(FILE *fpasm, const char *instruction, int es_variable_1,
                      int es_variable_2) {
    pop_values_for_operation(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "%s eax, ebx\n", instruction);
    fprintf(fpasm, "%s\n", "push dword eax");
}

/* FUNCIONES ARITMÉTICO-LÓGICAS BINARIAS */
/*
En todas ellas se realiza la operación como se ha resumido anteriormente:
- Se extrae de la pila los operandos
- Se realiza la operación
- Se guarda el resultado en la pila
Los dos últimos argumentos indican respectivamente si lo que hay en la pila
es una referencia a un valor o un valor explícito.

Deben tenerse en cuenta las peculiaridades de cada operación. En este
sentido sí hay que mencionar explícitamente que, en el caso de la división,
se debe controlar si el divisor es “0” y en ese caso se debe saltar a la
rutina de error controlado (restaurando el puntero de pila en ese caso y
comprobando en el retorno que no se produce “Segmentation Fault”)
*/
void sumar(FILE *fpasm, int es_variable_1, int es_variable_2) {
    simple_operation(fpasm, "add", es_variable_1, es_variable_2);
}

void restar(FILE *fpasm, int es_variable_1, int es_variable_2) {
    simple_operation(fpasm, "sub", es_variable_1, es_variable_2);
}

void multiplicar(FILE *fpasm, int es_variable_1, int es_variable_2) {
    pop_values_for_operation(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "%s\n", "cdq");
    fprintf(fpasm, "%s\n", "imul ebx");
    fprintf(fpasm, "%s\n", "push dword eax");
}

void dividir(FILE *fpasm, int es_variable_1, int es_variable_2) {
    pop_values_for_operation(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "%s\n", "cmp ebx, 0");
    fprintf(fpasm, "%s\n", "je fin_error_division");
    fprintf(fpasm, "%s\n", "cdq");
    fprintf(fpasm, "%s\n", "idiv ebx");
    fprintf(fpasm, "%s\n", "push dword eax");
}

void o(FILE *fpasm, int es_variable_1, int es_variable_2) {
    simple_operation(fpasm, "or", es_variable_1, es_variable_2);
}

void y(FILE *fpasm, int es_variable_1, int es_variable_2) {
    simple_operation(fpasm, "and", es_variable_1, es_variable_2);
}

void cambiar_signo(FILE *fpasm, int es_variable) {
    fprintf(fpasm, "%s\n", "pop dword eax");
    if (es_variable) {
        fprintf(fpasm, "%s\n", "mov eax, [eax]");
    }
    fprintf(fpasm, "%s\n", "neg dword eax");
    fprintf(fpasm, "%s\n", "push dword eax");
}
/*
Función aritmética de cambio de signo.
Es análoga a las binarias, excepto que sólo requiere de un acceso a la pila ya
que sólo usa un operando.
*/
void no(FILE *fpasm, int es_variable, int cuantos_no) {
    fprintf(fpasm, "%s\n", "pop dword eax");
    if (es_variable) {
        fprintf(fpasm, "%s\n", "mov dword eax, [eax]");
    }
    fprintf(fpasm, "%s\n", "cmp eax, 0");
    fprintf(fpasm, "jne match_no_%d\n", cuantos_no);
    fprintf(fpasm, "%s\n", "push dword 1");
    fprintf(fpasm, "jmp end_no_%d\n", cuantos_no);
    fprintf(fpasm, "match_no_%d:\n", cuantos_no);
    fprintf(fpasm, "%s\n", "push dword 0");
    fprintf(fpasm, "end_no_%d:\n", cuantos_no);
}
/*
Función monádica lógica de negación. No hay un código de operación de la ALU
que realice esta operación por lo que se debe codificar un algoritmo que, si
encuentra en la cima de la pila un 0 deja en la cima un 1 y al contrario.
El último argumento es el valor de etiqueta que corresponde (sin lugar a dudas,
la implementación del algoritmo requerirá etiquetas). Véase en los ejemplos de
programa principal como puede gestionarse el número de etiquetas cuantos_no.
*/

/* FUNCIONES COMPARATIVAS */
/*
Todas estas funciones reciben como argumento si los elementos a comparar son o
no variables. El resultado de las operaciones, que siempre será un booleano (“1”
si se cumple la comparación y “0” si no se cumple), se deja en la pila como en
el resto de operaciones. Se deben usar etiquetas para poder gestionar los saltos
necesarios para implementar las comparaciones.
*/

/**
 * Performs a comparision between two values. The scenario in which the
 * value of 1 is pushed to the stack is defined by the jump_instruction
 * parameter.
 * The other parameters encode the same values as in all comparison
 * functions
 */
void generic_comparison(FILE *fpasm, const char *jump_instruction,
                        int es_variable1, int es_variable2, int etiqueta) {
    pop_values_for_operation(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "%s\n", "cmp eax, ebx");
    fprintf(fpasm, "%s near match_%d\n", jump_instruction, etiqueta);
    fprintf(fpasm, "%s\n", "push dword 0");
    fprintf(fpasm, "jmp near end_%d\n", etiqueta);
    fprintf(fpasm, "match_%d:\n", etiqueta);
    fprintf(fpasm, "%s\n", "push dword 1");
    fprintf(fpasm, "end_%d:\n", etiqueta);
}

void igual(FILE *fpasm, int es_variable1, int es_variable2, int etiqueta) {
    generic_comparison(fpasm, "je", es_variable1, es_variable2, etiqueta);
}

void distinto(FILE *fpasm, int es_variable1, int es_variable2, int etiqueta) {
    generic_comparison(fpasm, "jne", es_variable1, es_variable2, etiqueta);
}

void menor_igual(FILE *fpasm, int es_variable1, int es_variable2,
                 int etiqueta) {
    generic_comparison(fpasm, "jle", es_variable1, es_variable2, etiqueta);
}

void mayor_igual(FILE *fpasm, int es_variable1, int es_variable2,
                 int etiqueta) {
    generic_comparison(fpasm, "jge", es_variable1, es_variable2, etiqueta);
}

void menor(FILE *fpasm, int es_variable1, int es_variable2, int etiqueta) {
    generic_comparison(fpasm, "jl", es_variable1, es_variable2, etiqueta);
}

void mayor(FILE *fpasm, int es_variable1, int es_variable2, int etiqueta) {
    generic_comparison(fpasm, "jg", es_variable1, es_variable2, etiqueta);
}

/* FUNCIONES DE ESCRITURA Y LECTURA */
/*
Se necesita saber el tipo de datos que se va a procesar (ENTERO o BOOLEANO) ya
que hay diferentes funciones de librería para la lectura (idem. escritura) de
cada tipo. Se deben insertar en la pila los argumentos necesarios, realizar la
llamada (call) a la función de librería correspondiente y limpiar la pila.
*/
void leer(FILE *fpasm, char *nombre, int tipo) {
    fprintf(fpasm, "push dword _%s\n", nombre);
    if (tipo == ENTERO) {
        fprintf(fpasm, "%s\n", "call scan_int");
    } else { // for our own sanity let's suppose this can only be BOOLEAN now
        fprintf(fpasm, "%s\n", "call scan_boolean");
    }
    fprintf(fpasm, "%s\n", "add esp, 4"); // TODO: error handling?
}

void escribir(FILE *fpasm, int es_variable, int tipo) {
    if (es_variable) {
        fprintf(fpasm, "%s\n", "pop dword eax");
        fprintf(fpasm, "%s\n", "mov eax, [eax]");
        fprintf(fpasm, "%s\n", "push dword eax");
    }

    if (tipo == ENTERO) {
        fprintf(fpasm, "%s\n", "call print_int");
    } else { // for our own sanity let's suppose this can only be BOOLEAN now
        fprintf(fpasm, "%s\n", "call print_boolean");
    } // TODO: error handling?

    fprintf(fpasm, "%s\n", "add esp, 4");
    fprintf(fpasm, "%s\n", "call print_endofline"); // TODO: error handling?
}

void ifthenelse_inicio(FILE *fpasm, int exp_es_variable, int etiqueta) {
    fprintf(fpasm, "%s\n", "pop dword eax");
    if (exp_es_variable) {
        fprintf(fprintf, "%s\n", "mov eax, [eax]");
    }
    fprintf(fpasm, "%s\n", "cmp eax, 0");
    fprintf(fpasm, "je near else%d\n", etiqueta);
}
/*
Generación de código para el inicio de una estructura if-then-else
Como es el inicio de uno bloque de control de flujo de programa que requiere de
una nueva etiqueta deben ejecutarse antes las tareas correspondientes a esta
situación exp_es_variable Es 1 si la expresión de la condición es algo
asimilable a una variable (identificador, elemento de vector) Es 0 en caso
contrario (constante u otro tipo de expresión)
*/

void ifthen_inicio(FILE *fpasm, int exp_es_variable, int etiqueta) {
    fprintf(fpasm, "%s\n", "pop dword eax");
    if (exp_es_variable) {
        fprintf(fprintf, "%s\n", "mov dword eax, [eax]");
    }
    fprintf(fpasm, "%s\n", "cmp eax, dword 0");
    fprintf(fpasm, "je near end%d\n", etiqueta);
}
/*
Generación de código para el inicio de una estructura if-then
Como es el inicio de uno bloque de control de flujo de programa que requiere de
una nueva etiqueta deben ejecutarse antes las tareas correspondientes a esta
situación exp_es_variable Es 1 si la expresión de la condición es algo
asimilable a una variable (identificador, elemento de vector) Es 0 en caso
contrario (constante u otro tipo de expresión)
*/

void ifthen_fin(FILE *fpasm, int etiqueta) {
    fprintf(fpasm, "end%d\n", etiqueta);
}
/*
Generación de código para el fin de una estructura if-then
Como es el fin de uno bloque de control de flujo de programa que hace uso de la
etiqueta del mismo se requiere que antes de su invocación tome el valor de la
etiqueta que le toca según se ha explicado Y tras ser invocada debe realizar el
proceso para ajustar la información de las etiquetas puesto que se ha liberado
la última de ellas.
*/

void ifthenelse_fin_then(FILE *fpasm, int etiqueta) {
    fprintf(fpasm, "jmp near end%d\n", etiqueta);
    fprintf(fpasm, "else%d:\n", etiqueta);
}
/*
Generación de código para el fin de la rama then de una estructura if-then-else
Sólo necesita usar la etiqueta adecuada, aunque es el final de una rama, luego
debe venir otra (la rama else) antes de que se termine la estructura y se tenga
que ajustar las etiquetas por lo que sólo se necesita que se utilice la etiqueta
que corresponde al momento actual.
*/

void ifthenelse_fin(FILE *fpasm, int etiqueta) {
    fprintf(fpasm, "end%d:\n", etiqueta);
}
/*
Generación de código para el fin de una estructura if-then-else
Como es el fin de uno bloque de control de flujo de programa que hace uso de la
etiqueta del mismo se requiere que antes de su invocación tome el valor de la
etiqueta que le toca según se ha explicado Y tras ser invocada debe realizar el
proceso para ajustar la información de las etiquetas puesto que se ha liberado
la última de ellas.
*/

void while_inicio(FILE *fpasm, int etiqueta) {
    fprintf(fpasm, "start_while%d:\n", etiqueta);
}
/*
Generación de código para el inicio de una estructura while
Como es el inicio de uno bloque de control de flujo de programa que requiere de
una nueva etiqueta deben ejecutarse antes las tareas correspondientes a esta
situación exp_es_variable Es 1 si la expresión de la condición es algo
asimilable a una variable (identificador, elemento de vector) Es 0 en caso
contrario (constante u otro tipo de expresión)
*/

void while_exp_pila(FILE *fpasm, int exp_es_variable, int etiqueta) {
    fprintf(fpasm, "%s\n", "pop dword eax");
    if (exp_es_variable) {
        fprintf(fpasm, "%s\n", "mov eax, [eax]");
    }
    fprintf(fpasm, "%s\n", "cmp eax, 0");
    fprintf(fpasm, "je near end_while%d\n", etiqueta);
}
/*
Generación de código para el momento en el que se ha generado el código de la
expresión de control del bucle Sólo necesita usar la etiqueta adecuada, por lo
que sólo se necesita que se recupere el valor de la etiqueta que corresponde al
momento actual. exp_es_variable Es 1 si la expresión de la condición es algo
asimilable a una variable (identificador, o elemento de vector) Es 0 en caso
contrario (constante u otro tipo de expresión)
*/

void while_fin(FILE *fpasm, int etiqueta) {
    fprintf(fpasm, "jmp near start_while%d\n", etiqueta);
    fprintf(fpasm, "end_while%d:\n", etiqueta);
}
/*
Generación de código para el final de una estructura while
Como es el fin de uno bloque de control de flujo de programa que hace uso de la
etiqueta del mismo se requiere que antes de su invocación tome el valor de la
etiqueta que le toca según se ha explicado Y tras ser invocada debe realizar el
proceso para ajustar la información de las etiquetas puesto que se ha liberado
la última de ellas.
*/

void escribir_elemento_vector(FILE *fpasm, char *nombre_vector, int tam_max,
                              int exp_es_direccion) {
    fprintf(fpasm, "%s\n", "pop dword eax");

    if (exp_es_direccion) {
        fprintf(fpasm, "%s\n", "mov eax, [eax]");
    }

    fprintf(fpasm, "%s\n", "cmp eax, 0");
    fprintf(fpasm, "%s\n", "jl fin_error_rango");
    fprintf(fpasm, "cmp eax, %d\n", tam_max);
    fprintf(fpasm, "%s\n", "jge fin_error_rango");

    fprintf(fpasm, "mov dword edx, _%s\n", nombre_vector);
    fprintf(fpasm, "%s\n", "lea eax, [edx + eax*4]");
    fprintf(fpasm, "%s\n", "push dword eax");
}
/*
Generación de código para indexar un vector
Cuyo nombre es nombre_vector
Declarado con un tamaño tam_max
La expresión que lo indexa está en la cima de la pila
Puede ser una variable (o algo equivalente) en cuyo caso exp_es_direccion vale 1
Puede ser un valor concreto (en ese caso exp_es_direccion vale 0)
Según se especifica en el material, es suficiente con utilizar dos registros
para realizar esta tarea.
*/

void declararFuncion(FILE *fd_asm, char *nombre_funcion, int num_var_loc) {
    fprintf(fd_asm, "_%s:\n", nombre_funcion);
    fprintf(fd_asm, "%s\n%s\n", "push dword ebp", "mov ebp, esp");
    fprintf(fd_asm, "sub esp, 4*%d\n", num_var_loc);
}
/*
Generación de código para iniciar la declaración de una función.
Es necesario proporcionar
Su nombre
Su número de variables locales
*/

void retornarFuncion(FILE *fd_asm, int es_variable) {
    fprintf(fd_asm, "%s\n", "pop dword eax");

    if (es_variable) {
        fprintf(fd_asm, "%s\n", "mov dword eax, [eax]");
    }

    fprintf(fd_asm, "%s\n%s\n%s\n", "mov esp, ebp", "pop dword ebp", "ret");
}
/*
Generación de código para el retorno de una función.
La expresión que se retorna está en la cima de la pila.
Puede ser una variable (o algo equivalente) en cuyo caso exp_es_direccion vale 1
Puede ser un valor concreto (en ese caso exp_es_direccion vale 0)
*/

void escribirParametro(FILE *fpasm, int pos_parametro,
                       int num_total_parametros) {
    int d_ebp = 4 * (1 + num_total_parametros - pos_parametro);
    fprintf(fpasm, "lea eax, [ebp + %d]\n", d_ebp);
    fprintf(fpasm, "%s\n", "push dword eax");
}
/*
Función para dejar en la cima de la pila la dirección efectiva del parámetro que
ocupa la posición pos_parametro (recuerda que los parámetros se ordenan con
origen 0) de un total de num_total_parametros
*/

void escribirVariableLocal(FILE *fpasm, int posicion_variable_local) {
    int d_ebp = 4 * posicion_variable_local;

    fprintf(fpasm, "lea eax, [ebp - %d]\n", d_ebp);
    fprintf(fpasm, "%s\n", "push dword eax");
}
/*
Función para dejar en la cima de la pila la dirección efectiva de la variable
local que ocupa la posición posicion_variable_local (recuerda que ordenadas con
origen 1)
*/

void asignarDestinoEnPila(FILE *fpasm, int es_variable) {
    fprintf(fpasm, "%s\n", "pop dword ebx");
    fprintf(fpasm, "%s\n", "pop dword eax");

    if (es_variable) {
        fprintf(fpasm, "%s\n", "mov eax, [eax]");
    }

    fprintf(fpasm, "%s\n", "mov dword [ebx], eax");
}
/*
Función para poder asignar a un destino que no es una variable “global” (tipo
_x) por ejemplo parámetros o variables locales (ya que en ese caso su nombre
real de alto nivel, no se tiene en cuenta pues es realmente un desplazamiento a
partir de ebp: ebp+4 o ebp-8 por ejemplo). Se debe asumir que en la pila estará
Primero (en la cima) lo que hay que asignar
Debajo (se ha introducido en la pila antes) la dirección donde hay que asignar
es_variable
Es 1 si la expresión que se va a asignar es algo asimilable a una variable
(identificador, o elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)
*/

void operandoEnPilaAArgumento(FILE *fd_asm, int es_variable) {
    if (es_variable) {
        fprintf(fd_asm, "%s\n", "mov dword eax, [esp]");
        fprintf(fd_asm, "%s\n", "mov dword eax, [eax]");
        fprintf(fd_asm, "%s\n", "mov dword [esp], eax");
    }
}
/*
Como habrás visto en el material, nuestro convenio de llamadas a las funciones
asume que los argumentos se pasan por valor, esto significa que siempre se dejan
en la pila “valores” y no “variables” Esta función realiza la tarea de dado un
operando escrito en la pila y sabiendo si es variable o no (es_variable) se deja
en la pila el valor correspondiente
*/

void llamarFuncion(FILE *fd_asm, char *nombre_funcion, int num_argumentos) {
    fprintf(fd_asm, "call _%s\n", nombre_funcion);
    limpiarPila(fd_asm, num_argumentos);
}
/*
Esta función genera código para llamar a la función nombre_funcion asumiendo que
los argumentos están en la pila en el orden fijado en el material de la
asignatura. Debe dejar en la cima de la pila el retorno de la función tras
haberla limpiado de sus argumentos Para limpiar la pila puede utilizar la
función de nombre limpiarPila
*/

void limpiarPila(FILE *fd_asm, int num_argumentos) {
    fprintf(fd_asm, "add esp, 4*%d\n", num_argumentos);
    fprintf(fd_asm, "%s\n", "push dword eax");
}
/*
Genera código para limpiar la pila tras invocar una función
Esta función es necesaria para completar la llamada a métodos, su gestión
dificulta el conocimiento por parte de la función de llamada del número de
argumentos que hay en la pila
*/
