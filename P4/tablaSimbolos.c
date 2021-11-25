#include "ht.h"
#include "syTable.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *in, *out;
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    char line[255];
    syTable *st = syTable_create();
    if (!st) {
        return -1;
    }

    while (fgets(line, 255, in)) {

        char id[255] = {0};
        int arg = 0;
        int n_filled = sscanf(line, "%s\t%d", id, &arg);

        if (n_filled == 2 && arg == -999) {
            // Close scope
            if (syTable_close_scope(st)) {
                fprintf(out, "cierre\n");
            } else {
                fprintf(out, "-1\tcierre\n");
            }
        } else if (n_filled == 2 && arg < -1) {
            // Create scope
            if (syTable_create_scope(st, id, arg)) {
                fprintf(out, "%s\n", id);
            } else {
                fprintf(out, "-1\t%s\n", id);
            }

        } else if (n_filled == 2 && arg >= 0) {
            // Try to insert the identifier with the value
            if (syTable_insert(st, id, arg)) {
                fprintf(out, "%s\n", id);
            } else {
                fprintf(out, "-1\t%s\n", id);
            }

        } else if (n_filled == 1) {
            // Search the identifier
            const int *val = syTable_search(st, id);
            fprintf(out, "%s\t%d\n", id, val ? *val : -1);
        } else {
            fprintf(stderr, "%d\t%s\t%d\n", n_filled, id, arg);
        }
    }
    return 0;
}