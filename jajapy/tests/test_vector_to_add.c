#include <stdio.h>
#include <check.h>
#include "../base/symbolic_to_numeric.h"
#include "../base/vector_to_add.h"
#include "cudd.h"
#include <unistd.h>

START_TEST(vector_4x1)
{
    int m = 4; // number of rows
    int n = 1; // number of columns

    DdManager *gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode *E;
    DdNode **x = (DdNode **)malloc(ceil(log2(m)) * sizeof(DdNode *));
    DdNode **y = (DdNode **)malloc(ceil(log2(n)) * sizeof(DdNode *));
    DdNode **xn = (DdNode **)malloc(ceil(log2(m)) * sizeof(DdNode *));
    DdNode **yn = (DdNode **)malloc(ceil(log2(n)) * sizeof(DdNode *));
    int nx = 0, ny = 0;

    double * vector = (double *)malloc(m * sizeof(double));

    vector[0] = 1;
    vector[1] = 2;
    vector[2] = 3;
    vector[3] = 4;

    vectorToADD(vector, gbm, &E, &x, &y, &xn, &yn, &nx, &ny, &m);

    CUDD_VALUE_TYPE **actual = symbolic_to_numeric(E, 4, 1);
    CUDD_VALUE_TYPE expected[4] = { 1, 2, 3, 4};

    ck_assert_double_eq(expected[0], actual[0][0]);
    ck_assert_double_eq(expected[1], actual[1][0]);
    ck_assert_double_eq(expected[2], actual[2][0]);
    ck_assert_double_eq(expected[3], actual[3][0]);

    ck_assert_int_eq(Cudd_DebugCheck(gbm), 0);

    Cudd_Quit(gbm);
    free(vector);
}

START_TEST(vector_1x1)
{
    int m = 1; // number of rows
    int n = 1; // number of columns

    DdManager *gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode *E;
    DdNode **x = (DdNode **)malloc(ceil(log2(m)) * sizeof(DdNode *));
    DdNode **y = (DdNode **)malloc(ceil(log2(n)) * sizeof(DdNode *));
    DdNode **xn = (DdNode **)malloc(ceil(log2(m)) * sizeof(DdNode *));
    DdNode **yn = (DdNode **)malloc(ceil(log2(n)) * sizeof(DdNode *));
    int nx = 0, ny = 0;

    double *vector = (double *)malloc(m * sizeof(double ));

    vector[0] = 1;

    vectorToADD(vector, gbm, &E, &x, &y, &xn, &yn, &nx, &ny, &m);

    CUDD_VALUE_TYPE **actual = symbolic_to_numeric(E, 1, 1);

    // Assert
    CUDD_VALUE_TYPE expected[1] = {1};
    ck_assert_double_eq(expected[0], actual[0][0]);

    ck_assert_int_eq(Cudd_DebugCheck(gbm), 0);

    Cudd_Quit(gbm);
    free(vector);
}

// This test checks that the function vectorToADD fails when called on a 2x2 matrix
START_TEST(matrix_2x2)
{
    int n = 2; // number of columns
    int m = 2; // number of rows

    DdManager *gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode *E;
    DdNode **x = (DdNode **)malloc(ceil(log2(m)) * sizeof(DdNode *));
    DdNode **y = (DdNode **)malloc(ceil(log2(n)) * sizeof(DdNode *));
    DdNode **xn = (DdNode **)malloc(ceil(log2(m)) * sizeof(DdNode *));
    DdNode **yn = (DdNode **)malloc(ceil(log2(n)) * sizeof(DdNode *));
    int nx = 0, ny = 0;

    double **matrix = (double **)malloc(m * sizeof(double *));
    matrix[0] = (double *)malloc(n * sizeof(double *));
    matrix[1] = (double *)malloc(n * sizeof(double *));
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;

    printf("\nThe following print should be an error coming from test (vector_2x2) in test_vector_to_add.c: \n");

    // Gets the result from the function. Expected value is 1, as we wrongly called the function on a 2D-matrix
    int result = vectorToADD(matrix, gbm, &E, &x, &y, &xn, &yn, &nx, &ny, &m);

    // Make sure that the function failed    
    ck_assert_int_ne(result, 0);

    ck_assert_int_eq(Cudd_DebugCheck(gbm), 0);

    // free memory of matrix
    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(gbm);
    free(matrix);
}

// This test checks that the function vectorToADD fails when called on a 3x3 matrix
START_TEST(matrix_3x3)
{
    int m = 3; // number of rows
    int n = 3; // number of columns

    DdManager *gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode *E;
    DdNode **x = (DdNode **)malloc(ceil(log2(m)) * sizeof(DdNode *));
    DdNode **y = (DdNode **)malloc(ceil(log2(n)) * sizeof(DdNode *));
    DdNode **xn = (DdNode **)malloc(ceil(log2(m)) * sizeof(DdNode *));
    DdNode **yn = (DdNode **)malloc(ceil(log2(n)) * sizeof(DdNode *));
    int nx = 0, ny = 0;

    double **matrix = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double *)malloc(n * sizeof(double));
    }

    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    matrix[1][2] = 6;
    matrix[2][0] = 7;
    matrix[2][1] = 8;
    matrix[2][2] = 9;

    printf("\nThe following print should be an error coming from test (vector_3x3) in test_vector_to_add.c: \n");


    // Gets the result from the function. Expected value is 1, as we wrongly called the function on a 2D-matrix
    int result3x3 = vectorToADD(matrix, gbm, &E, &x, &y, &xn, &yn, &nx, &ny, &m);

    // Make sure that the function failed
    ck_assert_int_ne(result3x3, 0);

     ck_assert_int_eq(Cudd_DebugCheck(gbm), 0);

    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(gbm);
    free(matrix);
}

Suite *vector_to_add_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Vector to ADD");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, vector_4x1);
    tcase_add_test(tc_core, vector_1x1);
    // tcase_add_test(tc_core, matrix_2x2);
    // tcase_add_test(tc_core, matrix_3x3);
    suite_add_tcase(s, tc_core);

    return s;
}
