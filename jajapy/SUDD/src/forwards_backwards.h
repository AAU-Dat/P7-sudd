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

int forwards(
    CUDD_VALUE_TYPE* omega,
    CUDD_VALUE_TYPE* P,
    CUDD_VALUE_TYPE* pi,
    ssize_t n_states,
    ssize_t n_obs,
    CUDD_VALUE_TYPE* alpha
);
int backwards(
    CUDD_VALUE_TYPE* omega,
    CUDD_VALUE_TYPE* P,
    CUDD_VALUE_TYPE* pi,
    ssize_t n_states,
    ssize_t n_obs,
    CUDD_VALUE_TYPE* beta
);
int fb(
    DdNode** (*_fb)(
        DdManager* manager,
        DdNode** omega,
        DdNode* P,
        DdNode* pi,
        DdNode** row_vars,
        DdNode** column_vars,
        int n_vars,
        int n_obs
    ),
    CUDD_VALUE_TYPE* omega,
    CUDD_VALUE_TYPE* P,
    CUDD_VALUE_TYPE* pi,
    ssize_t n_states,
    ssize_t n_obs,
    CUDD_VALUE_TYPE* alpha // output variable
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
int backwards_numeric_numpy(double* omega, 
                         double* P, 
                         double* pi,
                         int n_states,
                         int n_obs,
                         double* beta
);

int forwards_numeric_numpy(double* omega, 
                         double* P, 
                         double* pi,
                         int n_states,
                         int n_obs,
                         double* alpha
);
#endif
