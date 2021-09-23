#include "generacion.h"

int main() {
    FILE *f = stdout;
    escribir_subseccion_data(f);
    declarar_variable(f, "var1", ENTERO, 1);
    return 0;
}