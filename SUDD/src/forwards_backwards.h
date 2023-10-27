#ifndef FORWARDS_BACKWARDS_H
#define FORWARDS_BACKWARDS_H

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "cudd.h"
#include "conversion.h"

double** forwards(
    double** omega,
    double** P,
    double* pi,
    int n_states,
    int k_j
);
double** backwards(
    double** omega,
    double** P,
    double* pi,
    int n_states,
    int k_j
);
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
double **fb(
    DdNode** (*_fb)(
        DdManager* manager,
        DdNode** omega,
        DdNode* P,
        DdNode* pi,
        DdNode** row_vars,
        DdNode** column_vars,
        int n_vars,
        int k_j),
    double** omega,
    double** P,
    double* pi,
    int n_states,
    int k_j);

#endif
