#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_RUNS	100000

typedef struct {
    enum {
	MOVE,
	START_TOWER,
	END_TOWER,
	SET_E,
	TOOL,
	PING,
	START,
	DONE
    } t;
    union {
	struct {
	    double z, e;
	} move;
	struct {
	    int num, step;
	} ping;
	double e;
	int tool;
    } x;
    long pos;
} token_t;

typedef struct {
    double z;
    double e;
    int	   t;
} run_t;

static FILE *f;

static char buf[1024*1024];

static int validate_only = 0;
static int summary = 0;

static double last_z = 0, start_e = 0, last_e = 0, last_e_z = 0, acc_e = 0, last_reported_z = -1;
static int tool = 0;
static int seen_tool = 0;
static double tower_z = -1;
static int in_tower = 0;
static int seen_ping = 0;
static int started = 0;

static run_t runs[MAX_RUNS];
static int n_runs = 0;

static int
find_arg(const char *buf, char arg, double *val)
{
    size_t i;

    for (i = 0; buf[i]; i++) {
	if (buf[i] == ' ' && buf[i+1] == arg) {
	     if (sscanf(&buf[i+2], "%lf", val) > 0) {
		return 1;
	     }
	     return 0;
	}
    }
}

#define STRNCMP(a, b) strncmp(a, b, strlen(b))

static token_t
get_next_token()
{
    token_t t;

    while (fgets(buf, sizeof(buf), f) != NULL) {
	t.pos = ftell(f);
	if (STRNCMP(buf, "G1 ") == 0) {
	    t.t = MOVE;
	    if (! find_arg(buf, 'Z', &t.x.move.z)) t.x.move.z = last_z;
	    if (! find_arg(buf, 'E', &t.x.move.e)) t.x.move.e = last_e;
	    return t;
	}
	if (STRNCMP(buf, "; finishing tower layer") == 0 ||
	    STRNCMP(buf, "; finishing sparse tower layer") == 0) {
	    t.t = START_TOWER;
	    return t;
	}
	if (STRNCMP(buf, "; leaving transition tower") == 0) {
	    t.t = END_TOWER;
	    return t;
	}
	if (STRNCMP(buf, "; ping ") == 0) {
	    t.t = PING;
	    sscanf(buf, "; ping %d pause %d", &t.x.ping.num, &t.x.ping.step);
	    return t;
	}
	if (STRNCMP(buf, "G92 ") == 0) {
	    t.t = SET_E;
	    if (find_arg(buf, 'E', &t.x.e)) return t;
	}
	if (STRNCMP(buf, "; *** Main G-code ***") == 0 ||
	    STRNCMP(buf, "; layer 1, ") == 0) {
	    t.t = START;
	    return t;
	}
	if (buf[0] == 'T' && isdigit(buf[1])) {
	    t.t = TOOL;
	    t.x.tool = atoi(&buf[1]);
	    return t;
	}
    }

    t.t = DONE;
    return t;
}

static void
accumulate()
{
    acc_e += (last_e - start_e);
}

static void
reset_state()
{
    start_e = last_e;
    acc_e = 0;
    seen_ping = 0;
}

static void
show_extrusion(char chr, int force)
{
    int bad = in_tower && tower_z != last_e_z && acc_e > 0;

    last_reported_z = last_e_z;
    if (bad || acc_e != 0 || force) {
	if (validate_only && ! bad) return;

	printf("%c", chr);
	if (seen_tool) printf(" T%d", tool);
	printf(" Z %.02f", last_e_z);
	if (acc_e != 0) printf(" E %7.02f", acc_e);
	if (bad) printf(" *********** z delta = %.02f", last_e_z - tower_z);
	if (seen_ping) printf(" [ping]");
	printf("\n");
    }
}

static void
add_run()
{
    if (acc_e != 0) {
	runs[n_runs].z = last_e_z;
	runs[n_runs].e = acc_e;
	runs[n_runs].t = tool;
	n_runs++;
    }
}

static void process(const char *fname)
{
    if ((f = fopen(fname, "r")) == NULL) {
	perror(fname);
	return;
    }

    while (1) {
	token_t t = get_next_token();
	switch(t.t) {
	case MOVE:
	    if (t.x.move.e != last_e && t.x.move.z != last_e_z) {
		accumulate();
		if (started) {
		    add_run();
		    show_extrusion('+', 0);
		}
		last_e_z = t.x.move.z;
		reset_state();
	    }
	    last_e = t.x.move.e;
	    last_z = t.x.move.z;
	    break;
	case START_TOWER:
	    accumulate();
	    show_extrusion('+', 0);
	    reset_state();
	    tower_z = last_e_z;
	    in_tower = 1;
	    show_extrusion('>', 1);
	    break;
	case END_TOWER:
	    accumulate();
	    show_extrusion('<', 1);
	    in_tower = 0;
	    reset_state();
	    break;
	case SET_E:
	    accumulate();
	    start_e = t.x.e;
	    last_e = t.x.e;
	    break;
	case TOOL:
	    if (tool != t.x.tool) {
		if (! started) {
		    fprintf(stderr, "** ERROR *** Tool change before in prefix gcode\n");
		    exit(1);
		}
		accumulate();
		add_run();
		if (validate_only && acc_e == 0) printf("Z %.02f ******* Tool change with no extrusion, chroma may screw this up\n", last_z);
		show_extrusion('+', 0);
		reset_state();
		tool = t.x.tool;
		seen_tool = 1;
	    }
	    break;
	case PING:
	    seen_ping = 1;
	    break;
	case START:
	    started = 1;
	    accumulate();
	    reset_state();
	    break;
	case DONE:
	    goto done;
	}
    }

done:
    fclose(f);
    if (summary) {
	int i, j;

	for (i = 0; i < n_runs; i = j) {
	    printf("%6.02f", runs[i].z);
	    for (j = i; j < n_runs && runs[i].z == runs[j].z; j++) {
		printf(" %10.2f [%d]", runs[j].e, runs[j].t);
	    }
	    printf("\n");
	}
    }
}

int main(int argc, char **argv)
{
    int i;

    while (argc > 1) {
	    if (strcmp(argv[1], "--validate") == 0) validate_only = 1;
	    else if (strcmp(argv[1], "--summary") == 0) summary = 1;
	    else break;
	    argc--;
	    argv++;
    }

    for (i = 1; i < argc; i++) {
	process(argv[i]);
    }
}
