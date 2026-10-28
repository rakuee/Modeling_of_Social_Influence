#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "montecarlo.h"

int read_config(Config *cfg) {
    char line[256];

    printf("Introduce 4 valores separados por espacio:\n");
    printf("  max_candidates,  Numero de escenarios,  personas por escenario,  candidatos activos\n");
    printf("  (C <= max_candidates <= %d)\n> ", HARD_CAP);
    fflush(stdout);

    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "Error: no se pudo leer entrada\n");
        return 1;
    }

    int max_c, R, M, C;
    int n = sscanf(line, "%d %d %d %d", &max_c, &R, &M, &C);
    if (n != 4) {
        fprintf(stderr, "Error: se esperaban 4 enteros, se leyeron %d\n", n);
        return 1;
    }

    if (max_c < 1 || max_c > HARD_CAP) {
        fprintf(stderr, "Error: max_candidates debe estar en [1, %d]\n", HARD_CAP);
        return 1;
    }
    if (R < 2) {
        fprintf(stderr, "Error: R debe ser >= 2 (se necesita para el SE)\n");
        return 1;
    }
    if (M < 1) {
        fprintf(stderr, "Error: M debe ser >= 1\n");
        return 1;
    }
    if (C < 1 || C > max_c) {
        fprintf(stderr, "Error: C debe estar en [1, max_candidates=%d]\n", max_c);
        return 1;
    }

    cfg->max_candidates = max_c;
    cfg->R              = R;
    cfg->M              = M;
    cfg->C              = C;
    cfg->seed           = SEED_DEFAULT;
    cfg->output_prefix  = "resultados";
    return 0;
}