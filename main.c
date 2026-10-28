/* main.c */
#include <stdio.h>
#include "montecarlo.h"

int main(void) {
    Config cfg;

    if (read_config(&cfg) != 0) {
        return 1;
    }

    printf("\nConfiguración leída:\n");
    printf("  max_candidates = %d\n", cfg.max_candidates);
    printf("  R              = %d\n", cfg.R);
    printf("  M              = %d\n", cfg.M);
    printf("  C              = %d\n", cfg.C);
    printf("  seed           = %llu\n", cfg.seed);
    printf("  output_prefix  = %s\n", cfg.output_prefix);

    return 0;
}
