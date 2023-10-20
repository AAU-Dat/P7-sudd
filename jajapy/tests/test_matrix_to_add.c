#include <stdio.h>
#include <check.h>
#include "../base/symbolic_to_numeric.h"
#include "../base/matrix_to_add.h"
#include "cudd.h"

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

    matrixToADD(matrix, gbm, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);

    CUDD_VALUE_TYPE **actual = symbolic_to_numeric(E, m, n);

    // Assert
    CUDD_VALUE_TYPE expected[2][2] = {
        {1, 2},
        {3, 4}};
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[1][1], actual[1][1]);

    // free memory of matrix
    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(gbm);
    free(matrix);
}

START_TEST(matrix_1x4)
{
    int m = 1; // number of rows
    int n = 4; // number of columns

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
    matrix[0][3] = 4;

    matrixToADD(matrix, gbm, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);
    FILE *outfile; // output file pointer for .dot file

    CUDD_VALUE_TYPE **actual = symbolic_to_numeric(E, 1, 4);
    CUDD_VALUE_TYPE expected[1][4] = {
        {1, 2, 3, 4}};

    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[0][2], actual[0][2]);
    ck_assert_double_eq(expected[0][3], actual[0][3]);
}

START_TEST(matrix_4x1)
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

    double **matrix = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double *)malloc(n * sizeof(double));
    }

    matrix[0][0] = 1;
    matrix[1][0] = 2;
    matrix[2][0] = 3;
    matrix[3][0] = 4;

    matrixToADD(matrix, gbm, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);
    FILE *outfile; // output file pointer for .dot file

    CUDD_VALUE_TYPE **actual = symbolic_to_numeric(E, 4, 1);
    CUDD_VALUE_TYPE expected[4][1] = {
        {1},
        {2},
        {3},
        {4}
    };

    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[2][0], actual[2][0]);
    ck_assert_double_eq(expected[3][0], actual[3][0]);
}
Suite *matrix_to_add_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Matrix to ADD");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, matrix_2x2);
    tcase_add_test(tc_core, matrix_1x4);
    tcase_add_test(tc_core, matrix_4x1);
    suite_add_tcase(s, tc_core);

    return s;
}