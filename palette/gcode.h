#ifndef __GCODE_H__
#define __GCODE_H__

#include "bb.h"

#define MAX_RUNS        100000
#define N_DRIVES 4

typedef struct {
    bb_t   bb;
    double z;
    double e;
    int    t;
    int    pre_transition, post_transition;
} run_t;

typedef struct {
    int drive;
    double mm;
} splice_t;

typedef struct {
    double mm;
} ping_t;

extern run_t runs[MAX_RUNS];
extern int n_runs;
extern int used_tool[N_DRIVES];

extern splice_t splices[MAX_RUNS];
extern int n_splices;
extern ping_t pings[MAX_RUNS];
extern int n_pings;

extern int extrusions;
extern int gcode_trace;
extern int validate_only;

void gcode_to_runs(const char *fname);
void gcode_to_msf_gcode(const char *output_fname);

#endif