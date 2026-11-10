/* main.c */

#include <stdio.h>
#include <math.h>
#include "montecarlo.h"

#include <stdio.h>
#include "montecarlo.h"

int main(void) {
    rng_seed(12345ULL);
    Theta th;
    int C = 4;

    for (int k = 0; k < 5; k++) {
        sample_theta(&th, C);
        printf("k=%d: mu=%.3f  sigma=%.3f  q=%.3f  urb=%.3f  cand=[",
               k, th.mu_ideo, th.sigma_ideo, th.turnout_prob, th.urban_prob);
        for (int c = 0; c < C; c++) printf("%.2f%s", th.cand_pos[c],
                                            c < C-1 ? ", " : "");
        printf("]\n");
    }
    return 0;
}