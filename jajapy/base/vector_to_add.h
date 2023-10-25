#ifndef VECTOR_TO_ADD_H
#define VECTOR_TO_ADD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "cudd.h"

int vectorToADD(double *vector, DdManager *gbm, DdNode **E, DdNode ***x, DdNode ***y, DdNode ***xn, DdNode ***yn, int *nx, int *ny, int *m);
int writeVectorToFile(double *vector, int *m);


#endif
