#include "generacion.h"

int main() {
    FILE *f = stdout;
    escribir_subseccion_data(f);

    escribir_cabecera_bss(f);
    declarar_variable(f, "a", ENTERO, 1);

    escribir_segmento_codigo(f);
    escribir_inicio_main(f);
    escribir_operando(f, "a", 1);
    escribir_operando(f, "32", 0);
    asignar(f, "a", 0);
    escribir_fin(f);
    sumar(f, 0, 0);

    return 0;
}