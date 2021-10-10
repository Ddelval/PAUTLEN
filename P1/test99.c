#include "generacion.h"
#include <stdio.h>

int test1(int argc, char **argv) {
    FILE *salida;

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    salida = fopen(argv[1], "w");

    escribir_subseccion_data(salida);
    escribir_cabecera_bss(salida);
    declarar_variable(salida, "x", ENTERO, 1);
    declarar_variable(salida, "y", ENTERO, 1);
    declarar_variable(salida, "z", ENTERO, 1);

    escribir_segmento_codigo(salida);
    escribir_inicio_main(salida);

    /* x=8; */
    escribir_operando(salida, "8", 0);
    asignar(salida, "x", 0);

    /* scanf(&y); */
    leer(salida, "y", ENTERO);

    /* z = x + y */
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "y", 1);
    sumar(salida, 1, 1);
    asignar(salida, "z", 0);

    /* printf(z); */
    escribir_operando(salida, "z", 1);
    escribir(salida, 1, ENTERO);

    /* z = 7 + y */
    escribir_operando(salida, "7", 0);
    escribir_operando(salida, "y", 1);
    sumar(salida, 0, 1);
    asignar(salida, "z", 0);

    /* printf(z); */
    escribir_operando(salida, "z", 1);
    escribir(salida, 1, ENTERO);

    escribir_fin(salida);

    fclose(salida);
    return 0;
}

int test2(int argc, char **argv) {
    FILE *salida;
    int cuantos_no = 0;

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    salida = fopen(argv[1], "w");

    escribir_subseccion_data(salida);
    escribir_cabecera_bss(salida);
    declarar_variable(salida, "b1", BOOLEANO, 1);

    escribir_segmento_codigo(salida);
    escribir_inicio_main(salida);

    /* scanf b1; */
    leer(salida, "b1", BOOLEANO);

    /* printf !b1; */
    escribir_operando(salida, "b1", 1);
    no(salida, 1, cuantos_no++);
    escribir(salida, 0, BOOLEANO);

    /* printf !!b1; */
    escribir_operando(salida, "b1", 1);
    no(salida, 1, cuantos_no++);
    no(salida, 0, cuantos_no++);
    escribir(salida, 0, BOOLEANO);

    escribir_fin(salida);

    fclose(salida);
    return 0;
}
int test3(int argc, char **argv) {
    FILE *salida;

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    salida = fopen(argv[1], "w");

    escribir_subseccion_data(salida);
    escribir_cabecera_bss(salida);
    declarar_variable(salida, "x", ENTERO, 1);
    declarar_variable(salida, "y", ENTERO, 1);
    declarar_variable(salida, "z", ENTERO, 1);
    declarar_variable(salida, "j", ENTERO, 1);

    escribir_segmento_codigo(salida);
    escribir_inicio_main(salida);

    /* scanf x; */
    leer(salida, "x", ENTERO);

    /* scanf z; */
    leer(salida, "z", ENTERO);

    /* j = - x; */
    escribir_operando(salida, "x", 1);
    cambiar_signo(salida, 1);
    asignar(salida, "j", 0);

    /* printf j; */
    escribir_operando(salida, "j", 1);
    escribir(salida, 1, ENTERO);

    /* printf x-z; */
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "z", 1);

    restar(salida, 1, 1);
    escribir(salida, 0, ENTERO);

    /* y=x/2;*/
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "2", 0);
    dividir(salida, 1, 0);
    asignar(salida, "y", 0);

    /* printf y; */
    escribir_operando(salida, "y", 1);
    escribir(salida, 1, ENTERO);

    /*printf x*y;*/
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "y", 1);
    multiplicar(salida, 1, 1);
    escribir(salida, 0, ENTERO);

    escribir_fin(salida);

    fclose(salida);
    return 0;
}


int test4(int argc, char **argv) {
    FILE *salida;
    int etiqueta = 0;

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    salida = fopen(argv[1], "w");

    escribir_subseccion_data(salida);
    escribir_cabecera_bss(salida);
    declarar_variable(salida, "b1", BOOLEANO, 1);
    declarar_variable(salida, "x", ENTERO, 1);

    escribir_segmento_codigo(salida);
    escribir_inicio_main(salida);

    /* scanf b1; */
    leer(salida, "b1", BOOLEANO);
    /* scanf x; */
    leer(salida, "x", ENTERO);

    /* printf (x > 3); */
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "3", 0);
    mayor(salida, 1, 0, etiqueta++);
    escribir(salida, 0, BOOLEANO);

    /* printf (x >= 3); */
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "3", 0);
    mayor_igual(salida, 1, 0, etiqueta++);
    escribir(salida, 0, BOOLEANO);

    /* printf (x < 3); */
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "3", 0);
    menor(salida, 1, 0, etiqueta++);
    escribir(salida, 0, BOOLEANO);

    /* printf (x <= 3); */
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "3", 0);
    menor_igual(salida, 1, 0, etiqueta++);
    escribir(salida, 0, BOOLEANO);

    /* printf (x == 3); */
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "3", 0);
    igual(salida, 1, 0, etiqueta++);
    escribir(salida, 0, BOOLEANO);

    /* printf (x != 3); */
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "3", 0);
    distinto(salida, 1, 0, etiqueta++);
    escribir(salida, 0, BOOLEANO);

    /* printf b1&&false; */
    escribir_operando(salida, "b1", 1);
    escribir_operando(salida, "0", 0);
    y(salida, 1, 0);
    escribir(salida, 0, BOOLEANO);

    /* printf b1||true; */
    escribir_operando(salida, "b1", 1);
    escribir_operando(salida, "1", 0);
    o(salida, 1, 0);
    escribir(salida, 0, BOOLEANO);

    escribir_fin(salida);

    fclose(salida);
    return 0;
}

int test5 (int argc, char **argv) {

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    int etiqueta = 0;
    int getiqueta = 0;
    int etiquetas[MAX_ETIQUETAS];
    int cima_etiquetas = -1;
    FILE *fd_asm;

    fd_asm = fopen(argv[1], "w");
    escribir_subseccion_data(fd_asm);
    escribir_cabecera_bss(fd_asm);

    // int m;
    declarar_variable(fd_asm, "m", 1, 1);

    // int [4] v;
    declarar_variable(fd_asm, "v", 1, 4);
    escribir_segmento_codigo(fd_asm);
    escribir_inicio_main(fd_asm);

    // m=0;
    escribir_operando(fd_asm, "0", 0);
    asignar(fd_asm, "m", 0);

    // While. Gestion inicial de las etiquetas, guardado de etiqueta.
    getiqueta++;
    cima_etiquetas++;

    etiquetas[cima_etiquetas] = getiqueta;

    // Inicio del while. Impresion de la etiqueta.
    etiqueta = etiquetas[cima_etiquetas];
    while_inicio(fd_asm, etiqueta);

    // Condicion del bucle while.
    escribir_operando(fd_asm, "m", 1);
    escribir_operando(fd_asm, "4", 0);
    menor_igual(fd_asm, 1, 0, etiqueta);

    // Recuperamos la etiqueta para imprimir la comparacion del while.
    etiqueta = etiquetas[cima_etiquetas];
    while_exp_pila(fd_asm, 0, etiqueta);

    // printf m
    escribir_operando(fd_asm, "m", 1);
    escribir(fd_asm, 1, ENTERO);

    // v[m] = m*10;
    escribir_operando(fd_asm, "m", 1);
    escribir_operando(fd_asm, "10", 0);
    multiplicar(fd_asm, 1, 0);
    escribir_operando(fd_asm, "m", 1);
    escribir_elemento_vector(fd_asm, "v", 4, 1);
    asignarDestinoEnPila(fd_asm, 0);

    // m = m + 1
    escribir_operando(fd_asm, "m", 1);
    escribir_operando(fd_asm, "1", 0);
    sumar(fd_asm, 1, 0);
    asignar(fd_asm, "m", 0);

    // Recuperamos la etiqueta para imprimir el fin de etiqueta del while.
    etiqueta = etiquetas[cima_etiquetas];
    while_fin(fd_asm, etiqueta);

    // Al cerrar el ámbito, decrementamos el contador de cima de etiquetas.
    cima_etiquetas--;

    escribir_fin(fd_asm);

    fclose(fd_asm);
}



int test6(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    int etiqueta = 0;
    int getiqueta = 0;
    int etiquetas[MAX_ETIQUETAS];
    int cima_etiquetas = -1;
    FILE *fd_asm;
    fd_asm = fopen(argv[1], "w");
    escribir_subseccion_data(fd_asm);
    escribir_cabecera_bss(fd_asm);

    // int m;
    declarar_variable(fd_asm, "m", 1, 1);

    escribir_segmento_codigo(fd_asm);
    escribir_inicio_main(fd_asm);

    // m=0;
    escribir_operando(fd_asm, "0", 0);
    asignar(fd_asm, "m", 0);

    // Gestion de etiquetas para abrir el ambito del if. Esta sera la etiqueta
    // que tenga.
    getiqueta++;
    cima_etiquetas++;
    etiquetas[cima_etiquetas] = getiqueta;
    etiqueta = getiqueta;

    // Condición del if. Salto a fin si, si la condicion se da.
    escribir_operando(fd_asm, "m", 1);
    escribir_operando(fd_asm, "5", 0);
    mayor(fd_asm, 1, 0, etiqueta);
    ifthenelse_inicio(fd_asm, 0, etiqueta);

    // printf 2
    escribir_operando(fd_asm, "2", 0);
    escribir(fd_asm, 0, ENTERO);

    // Salto a fin sino al terminar el if, impresion de la etiqueta fin_si.
    etiqueta = etiquetas[cima_etiquetas];
    ifthenelse_fin_then(fd_asm, etiqueta);

    // printf 3
    escribir_operando(fd_asm, "3", 0);
    escribir(fd_asm, 0, ENTERO);

    // Fin del condicional if. Imprimimos la etiqueta de ambito del fin del
    etiqueta = etiquetas[cima_etiquetas];
    ifthenelse_fin(fd_asm, etiqueta);
    cima_etiquetas--;

    escribir_fin(fd_asm);

    fclose(fd_asm);
}


int test7(int argc, char **argv) {
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

    // 2*arg.
    escribir_operando(fd_asm, "2", 0);
    escribirParametro(fd_asm, 0, 1);
    multiplicar(fd_asm, 0, 1);

    // Retornamos de la funcion con lo que esta encima de la pila.
    retornarFuncion(fd_asm, 0);

    escribir_inicio_main(fd_asm);
    // z=2
    escribir_operando(fd_asm, "2", 0);
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

int test10 (int argc, char **argv) {
    FILE *salida;

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    salida = fopen(argv[1], "w");

    escribir_subseccion_data(salida);
    escribir_cabecera_bss(salida);
    declarar_variable(salida, "x", ENTERO, 1);
    declarar_variable(salida, "y", ENTERO, 1);
    declarar_variable(salida, "z", ENTERO, 1);
    declarar_variable(salida, "j", ENTERO, 1);

    escribir_segmento_codigo(salida);
    escribir_inicio_main(salida);

    /* scanf x; */
    leer(salida, "x", ENTERO);


    /* y=x/0;*/
    escribir_operando(salida, "x", 1);
    escribir_operando(salida, "0", 0);
    dividir(salida, 1, 0);
    asignar(salida, "y", 0);

    /* printf y; */
    escribir_operando(salida, "y", 1);
    escribir(salida, 1, ENTERO);

    escribir_fin(salida);

    fclose(salida);
    return 0;
}

int test11(int argc, char **argv) {
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
    while_exp_pila(fd_asm, 0, etiqueta);

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


int test12(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    int etiqueta = 0;
    int getiqueta = 0;
    int etiquetas[MAX_ETIQUETAS];
    int cima_etiquetas = -1;
    FILE *fd_asm;
    fd_asm = fopen(argv[1], "w");
    escribir_subseccion_data(fd_asm);
    escribir_cabecera_bss(fd_asm);

    // int m;
    declarar_variable(fd_asm, "m", 1, 1);

    escribir_segmento_codigo(fd_asm);
    escribir_inicio_main(fd_asm);

    // m=0;
    escribir_operando(fd_asm, "1", 0);
    asignar(fd_asm, "m", 0);

    // Gestion de etiquetas para abrir el ambito del if. Esta sera la etiqueta
    // que tenga.
    getiqueta++;
    cima_etiquetas++;
    etiquetas[cima_etiquetas] = getiqueta;
    etiqueta = getiqueta;

    escribir_operando(fd_asm,"m",1);
    ifthen_inicio(fd_asm, 1, etiqueta);

    // printf 2
    escribir_operando(fd_asm, "2", 0);
    escribir(fd_asm, 0, ENTERO);

    // printf 3
    escribir_operando(fd_asm, "3", 0);
    escribir(fd_asm, 0, ENTERO);

    ifthen_fin(fd_asm, etiqueta);

    escribir_operando(fd_asm,"m",1);
    ifthenelse_inicio(fd_asm, 1, etiqueta);

    // printf 2
    escribir_operando(fd_asm, "2", 0);
    escribir(fd_asm, 0, ENTERO);
    ifthenelse_fin_then(fd_asm,etiqueta);

    // printf 3
    escribir_operando(fd_asm, "3", 0);
    escribir(fd_asm, 0, ENTERO);

    ifthenelse_fin(fd_asm, etiqueta);

    escribir_fin(fd_asm);

    fclose(fd_asm);
}


int test14(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stdout, "ERROR POCOS ARGUMENTOS\n");
        return -1;
    }

    int etiqueta = 0;
    int getiqueta = 0;
    int etiquetas[MAX_ETIQUETAS];
    int cima_etiquetas = -1;
    FILE *fd_asm;
    fd_asm = fopen(argv[1], "w");
    escribir_subseccion_data(fd_asm);
    escribir_cabecera_bss(fd_asm);

    // int m;
    declarar_variable(fd_asm, "m", 1, 1);
    declarar_variable(fd_asm, "m2", 1, 1);

    escribir_segmento_codigo(fd_asm);
    escribir_inicio_main(fd_asm);

    // m=0;
    escribir_operando(fd_asm, "1", 0);
    asignar(fd_asm, "m2", 0);
    escribir_operando(fd_asm, "m2",1);
    asignar(fd_asm, "m", 1);

    // Gestion de etiquetas para abrir el ambito del if. Esta sera la etiqueta
    // que tenga.
    getiqueta++;
    cima_etiquetas++;
    etiquetas[cima_etiquetas] = getiqueta;
    etiqueta = getiqueta;

    escribir_operando(fd_asm,"m",1);
    ifthen_inicio(fd_asm, 1, etiqueta);

    // printf 2
    escribir_operando(fd_asm, "2", 0);
    escribir(fd_asm, 0, ENTERO);

    // printf 3
    escribir_operando(fd_asm, "3", 0);
    escribir(fd_asm, 0, ENTERO);

    ifthen_fin(fd_asm, etiqueta);

    escribir_operando(fd_asm,"m",1);
    ifthenelse_inicio(fd_asm, 1, etiqueta);

    // printf 2
    escribir_operando(fd_asm, "2", 0);
    escribir(fd_asm, 0, ENTERO);
    ifthenelse_fin_then(fd_asm,etiqueta);

    // printf 3
    escribir_operando(fd_asm, "3", 0);
    escribir(fd_asm, 0, ENTERO);

    ifthenelse_fin(fd_asm, etiqueta);

    escribir_fin(fd_asm);

    fclose(fd_asm);
}

int test15(int argc, char **argv) {
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


int main(int argc, char ** argv){
    test1(argc,argv);
    test2(argc,argv);
    test3(argc,argv);
    test4(argc,argv);
    test5(argc,argv);
    test6(argc,argv);
    test7(argc,argv);
    test10(argc,argv);
    test11(argc,argv);
    test12(argc,argv);
    test14(argc,argv);
    test15(argc,argv);
    return 0;
}