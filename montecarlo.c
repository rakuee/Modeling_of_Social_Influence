#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
/* ---------- RNG: xorshift64 + Box-Muller ---------- */

static unsigned long long rng_state = 88172645463325252ULL;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void rng_seed(unsigned long long s) {
    rng_state = s ? s : 88172645463325252ULL;   /* seed=0 produce ceros */
}

double rand_uniform(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 7;
    rng_state ^= rng_state << 17;
    /* 53 bits de mantisa → uniforme en [0,1) */
    return (rng_state >> 11) * (1.0 / 9007199254740992.0);
}

double rand_normal(double mu, double sd) {
    double u1 = rand_uniform();
    double u2 = rand_uniform();
    if (u1 < 1e-12) u1 = 1e-12;               /* evita log(0) */
    return mu + sd * sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

/* ---------- Posiciones nominales de los candidatos (escala 0-10) ---------- */
static const double CAND_BASE[HARD_CAP] = {
    2.0, 4.0, 6.0, 8.0,          /* primeros 4: equidistantes */
    5.0, 5.0, 5.0, 5.0,          /* relleno para C > 4        */
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0
};

void sample_theta(Theta *th, int C) {
    /* 1) Centro ideológico del electorado: N(5, 1²) en escala 0-10 */
    th->mu_ideo = rand_normal(5.0, 1.0);

    /* 2) Dispersión ideológica: U(1.0, 3.0) */
    th->sigma_ideo = 1.0 + 2.0 * rand_uniform();

    /* 3) Probabilidad base de votar: U(0.50, 0.80) */
    th->turnout_prob = 0.50 + 0.30 * rand_uniform();

    /* 4) Proporción urbana: U(0.55, 0.85) */
    th->urban_prob = 0.55 + 0.30 * rand_uniform();

    /* 5) Posición de cada candidato activo: N(base_c, 0.5²) */
    for (int c = 0; c < C; c++) {
        th->cand_pos[c] = rand_normal(CAND_BASE[c], 0.5);
    }

    /* 6) Relleno: los candidatos no usados quedan en el centro */
    for (int c = C; c < HARD_CAP; c++) {
        th->cand_pos[c] = 5.0;
    }
}