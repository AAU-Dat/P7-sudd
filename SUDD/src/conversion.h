#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "cudd.h"

CUDD_VALUE_TYPE** symbolic_to_numeric(DdNode* symbolic, int n_rows, int n_columns);
CUDD_VALUE_TYPE** create_2d_array(int n_rows, int n_columns);
void interleave(bool A[], int sizeA, bool B[], int sizeB, bool result[]);
CUDD_VALUE_TYPE evaluate_matrix_BDD(DdNode* node, bool bits[], int index);
void increment_bit_array(bool array[], int array_size);
void printMatrix(CUDD_VALUE_TYPE** matrix);
int vectorToADD(double *vector, DdManager *gbm, DdNode **E, DdNode ***x, DdNode ***y, DdNode ***xn, DdNode ***yn, int *nx, int *ny, int *m);
int writeVectorToFile(double *vector, int *m);
int matrixToADD(double **matrix, DdManager *gbm, DdNode **E, DdNode ***x, DdNode ***y, DdNode ***xn, DdNode ***yn, int *nx, int *ny, int *m, int *n);
int writeMatrixToFile(double **matrix, int *m, int *n);


#endif