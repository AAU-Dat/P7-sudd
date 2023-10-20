#ifndef FORWARD_H
#define FORWARD_H

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cudd.h"

double** forward(
    double** omega,
    double** P, double* pi,
    int n_states,
    int k_j
);
DdNode* add_square_transpose(DdManager* manager, DdNode* matrix, int n_rows);
DdNode** _forwards(
    DdManager* manager,
    DdNode** omega, 
    DdNode* P, 
    DdNode* pi, 
    DdNode** row_vars,
    DdNode** column_vars,
    int n_vars,
    int k_j
);
DdNode** _backwards(
    DdManager* manager,
    DdNode** omega, 
    DdNode* P, 
    DdNode* pi, 
    DdNode** row_vars,
    DdNode** column_vars,
    int n_vars,
    int k_j
);

#endif
