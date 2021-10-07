
#include "generacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

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

    // Fin del condicional if. Imprimimos la etiqueta de ambito del fin del
    etiqueta = etiquetas[cima_etiquetas];
    ifthen_fin(fd_asm, etiqueta);
    cima_etiquetas--;

    escribir_fin(fd_asm);

    fclose(fd_asm);
}