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

CUDD_VALUE_TYPE** add_to_matrix(DdNode* symbolic, int n_rows, int n_columns);
CUDD_VALUE_TYPE** create_2d_array(int n_rows, int n_columns);
void interleave(bool A[], int size_a, bool B[], int size_b, bool result[]);
CUDD_VALUE_TYPE evaluate_matrix_bdd(DdNode* node, bool bits[], int index);
void increment_bit_array(bool array[], int array_size);
void printMatrix(CUDD_VALUE_TYPE** matrix);
int vector_to_add(
    double* vector, 
    DdManager* manager, 
    DdNode** E, 
    DdNode*** x, 
    DdNode*** y, 
    DdNode*** xn, 
    DdNode*** yn, 
    int* nx, 
    int* ny, 
    int* m);
int write_vector_to_file(double* vector, int* m);
int matrix_to_add(
    double **matrix, 
    DdManager* manager, 
    DdNode** E, 
    DdNode*** x,
    DdNode*** y, 
    DdNode*** xn, 
    DdNode*** yn,
    int* nx, 
    int* ny, 
    int* m, 
    int* n);
int write_matrix_to_file(double **matrix, int *m, int *n);


#endif