#include "generacion.h"
#include <stdio.h>

int main(int argc, char **argv) {
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
