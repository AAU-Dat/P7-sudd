#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cudd.h"

DdNode* matrix_2x2(DdManager* manager, double matrix[2][2], DdNode* row_var, DdNode* col_var);
DdNode* vector_2x1(DdManager* manager, double vector[2], DdNode* row_var);

#endif
