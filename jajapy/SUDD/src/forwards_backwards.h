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
double** file_forwards(
    char **omega,
    char *P,
    char *pi,
    int n_states,
    int k_j
);
double** file_backwards(
    char **omega,
    char *P,
    char *pi,
    int n_states,
    int k_j
);
double** file_fb(
    DdNode** (*_fb)(
        DdManager* manager,
        DdNode** omega,
        DdNode* P,
        DdNode* pi,
        DdNode** row_vars,
        DdNode** column_vars,
        int n_vars,
        int k_j),
    char **omega,
    char *P,
    char *pi,
    int n_states,
    int k_j);

#endif
