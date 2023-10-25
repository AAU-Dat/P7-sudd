#ifndef FORWARD_WRAPPER_H
#define FORWARD_WRAPPER_H

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "cudd.h"
#include "matrix_to_add.h"
#include "symbolic_to_numeric.h"
#include "forward.h"

double** forward(
    double **omega, 
    double **P, 
    double **pi,
    int n_states, 
    int k_j
);

#endif
