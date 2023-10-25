#ifndef SYMBOLIC_TO_NUMERIC_H
#define SYMBOLIC_TO_NUMERIC_H

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cudd.h"

CUDD_VALUE_TYPE** symbolic_to_numeric(DdNode* symbolic, int n_rows, int n_columns);
CUDD_VALUE_TYPE** create_2d_array(int n_rows, int n_columns);
void interleave(bool A[], int sizeA, bool B[], int sizeB, bool result[]);
CUDD_VALUE_TYPE evaluate_matrix_BDD(DdNode* node, bool bits[], int index);
void increment_bit_array(bool array[], int array_size);
void printMatrix(CUDD_VALUE_TYPE** matrix);

#endif
