#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cudd.h"

DdNode* matrix_2x2(DdManager* manager, double matrix[2][2], DdNode* row_var, DdNode* col_var);
DdNode* matrix_3x3(DdManager* manager, double matrix[3][3], DdNode* row_vars[2], DdNode* col_vars[2]);
DdNode* vector_2x1(DdManager* manager, double vector[2], DdNode* row_var);
DdNode* vector_3x1(DdManager* manager, double vector[3], DdNode* row_vars[2]);
double** forwards_numeric(double** omega, double** P, double* pi, int n_states, int n_obs);
double** backwards_numeric(double** omega, double** P, double* pi, int n_states, int n_obs);
#endif
