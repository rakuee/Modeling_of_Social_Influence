/* montecarlo.h */

#ifndef MONTECARLO_H
#define MONTECARLO_H


/* ---------- Topes físicos del programa ---------- */
#define HARD_CAP        64      /* tamaño fijo de arrays de candidatos */
#define SEED_DEFAULT 12345ULL


/* ---------- Configuración leída desde la terminal ---------- */
typedef struct {
    int    max_candidates;      /* entrada #1: tope lógico, <= HARD_CAP */
    int    R;                   /* entrada #2: número de escenarios     */
    int    M;                   /* entrada #3: personas por escenario   */
    int    C;                   /* entrada #4: candidatos activos       */
    unsigned long long seed;    /* fija por ahora                        */
    const char *output_prefix;
} Config;


/* ---------- Parámetros de un escenario: theta^(r) ---------- */

typedef struct {
    double mean_ideo;                    /* media ideológica       */
    double sigma_ideo;                 /* dispersión ideológica  */
    double turnout_prob;               /* prob. de votar         */
    double urban_prob;                 /* prob. de ser urbano    */
    double cand_pos[HARD_CAP];   /* posición por candidato */
} Theta;



/* ---------- Persona sintética ---------- */
typedef struct {
    double ideo;
    double age;
    double income;
    int    sex;          /* 0 = M, 1 = F */
    int    educ;         /* 0..3         */
    int    urban;        /* 0/1          */
    double pol_interest; /* [0,1]        */
    double trust;        /* [0,1]        */
} Person;



/* ---------- Resultado agregado por candidato ---------- */
typedef struct {
    double f_hat;
    double se;
    double ci_low;
    double ci_high;
} Estimate;

int read_config(Config *cfg);

#endif /* MONTECARLO_H */
