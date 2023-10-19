#ifndef MATRIX_TO_ADD_H
#define MATRIX_TO_ADD_H

#include <stdio.h>
#include "cudd.h"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

void matrixToADD(DdManager *gbm, double **matrix, DdNode *E, DdNode **x, DdNode **y, DdNode **xn, DdNode **yn_, int *nx, int *ny, int *m, int *n);
void writeMatrixToFile(double **matrix, int *m, int *n);

#endif