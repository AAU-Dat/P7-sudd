#ifndef FORWARD_H
#define FORWARD_H

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cudd.h"

double** forward(double** gamma, double** P, double* pi);
DdNode* add_square_transpose(DdManager* manager, DdNode* matrix, int n_rows);
DdNode** _forward( DdManager* manager,
    DdNode** gamma, 
    DdNode* P, 
    int n_states,
    DdNode* pi, 
    DdNode** z,
    int k_j
);

#endif
