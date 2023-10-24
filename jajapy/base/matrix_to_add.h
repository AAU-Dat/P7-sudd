#ifndef MATRIX_TO_ADD_H
#define MATRIX_TO_ADD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "cudd.h"

int matrixToADD(double **matrix, DdManager *gbm, DdNode **E, DdNode ***x, DdNode ***y, DdNode ***xn, DdNode ***yn, int *nx, int *ny, int *m, int *n);
int writeMatrixToFile(double **matrix, int *m, int *n);


#endif