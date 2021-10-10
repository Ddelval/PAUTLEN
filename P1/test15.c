
#include "generacion.h"

int main(int argc, char **argv) {
    int etiqueta = 0;
    int getiqueta = 0;
    int etiquetas[MAX_ETIQUETAS];
    int cima_etiquetas = -1;
    FILE *fd_asm;

    fd_asm = fopen(argv[1], "w");
    escribir_subseccion_data(fd_asm);
    escribir_cabecera_bss(fd_asm);

    // int z;
    declarar_variable(fd_asm, "z", 1, 1);
    declarar_variable(fd_asm, "tmp", 1, 1);

    escribir_segmento_codigo(fd_asm);
    // Declaramos la funcion. Vamos a imprimir su etiqueta y decir que tiene una
    // variable local. function int doble(int arg)
    //{
    //        int auxArg;
    declararFuncion(fd_asm, "doble", 1);

    // auxArg = arg; Asignacion de parametro a variable local. Solo hay un
    // parametro.
    escribirParametro(fd_asm, 0, 1);
    escribirVariableLocal(fd_asm, 1);
    asignarDestinoEnPila(fd_asm, 1);

    // While. Gestion inicial de las etiquetas, guardado de etiqueta.
    getiqueta++;
    cima_etiquetas++;

    etiquetas[cima_etiquetas] = getiqueta;

    // Inicio del while. Impresion de la etiqueta.

    etiqueta = etiquetas[cima_etiquetas];
    while_inicio(fd_asm, etiqueta);
    escribirVariableLocal(fd_asm, 1);
    escribir_operando(fd_asm, "1", 0);
    mayor(fd_asm, 1, 0, etiqueta);

    while_exp_pila(fd_asm, 0, etiqueta);
    // 2*arg.

    escribirVariableLocal(fd_asm, 1);
    escribir(fd_asm, 1, ENTERO);

    escribir_operando(fd_asm, "1", 0);
    escribirVariableLocal(fd_asm, 1);
    y(fd_asm, 0, 1);
    no(fd_asm, 0, etiqueta);
    // 0 = impar

    ifthenelse_inicio(fd_asm, 0, etiqueta);

    cima_etiquetas++;
    etiquetas[cima_etiquetas] = ++getiqueta;
    etiqueta = etiquetas[cima_etiquetas];

    while_inicio(fd_asm, etiqueta);

    escribir_operando(fd_asm, "1", 0);
    escribirVariableLocal(fd_asm, 1);
    y(fd_asm, 0, 1);
    no(fd_asm, 0, etiqueta);
    asignar(fd_asm,"tmp",0);
    escribir_operando(fd_asm,"tmp",1);
    while_exp_pila(fd_asm, 1, etiqueta);

    escribirVariableLocal(fd_asm,1);
    escribir_operando(fd_asm,"2",0);
    dividir(fd_asm,1,0);
    escribirVariableLocal(fd_asm,1);
    asignarDestinoEnPila(fd_asm,0);

    while_fin(fd_asm,etiqueta);
    cima_etiquetas--;
    etiqueta=etiquetas[cima_etiquetas];

    ifthenelse_fin_then(fd_asm, etiqueta);

    escribirVariableLocal(fd_asm, 1);
    escribir_operando(fd_asm, "3", 0);
    multiplicar(fd_asm, 1, 0);
    escribir_operando(fd_asm, "1", 0);
    sumar(fd_asm, 0, 0);
    escribirVariableLocal(fd_asm,1);
    asignarDestinoEnPila(fd_asm,0);
    
    
    

    ifthenelse_fin(fd_asm, etiqueta);

    while_fin(fd_asm, etiqueta);

    escribirVariableLocal(fd_asm, 1);
    // Retornamos de la funcion con lo que esta encima de la pila.
    retornarFuncion(fd_asm, 1);

    escribir_inicio_main(fd_asm);
    // z=2
    escribir_operando(fd_asm, "101", 0);
    asignar(fd_asm, "z", 0);

    escribir_operando(fd_asm, "z", 1);

    // printf doble(z)

    // Llamamos a la funcion que tiene 1 argumento. Estamos dando un salto
    // a la
    // etiqueta.Primero apilamos el parametro.
    operandoEnPilaAArgumento(fd_asm, 1);
    llamarFuncion(fd_asm, "doble", 1);

    // Imprimimos el resultado de la funcion.
    escribir(fd_asm, 0, ENTERO);

    escribir_fin(fd_asm);
    fclose(fd_asm);
}
