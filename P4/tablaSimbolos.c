#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *in, *out;
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    char line[255];

    while (fgets(line, 255, in)) {
        char id[255];
        int arg;
        int n_filled = sscanf("%s\t%d", id, arg);

        if (n_filled == 2 && arg < -1) {
        }
    }
}