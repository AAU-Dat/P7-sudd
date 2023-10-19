#include <check.h>
#include "../base/symbolic_to_numeric.h"
#include "../base/matrix_to_add.h"
#include "cudd.h"

START_TEST(test_matrix_to_add)
{

    // initialize variables
    DdManager *gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode *E; //**x, **y, **xn, **yn_;
    int nx, ny, m, n;
    m = 3;
    n = 3;
    int size_row = 3;
    int size_col = 3;
    nx = 0;
    ny = 0;
    DdNode *x[(int) ceil(log2(size_row))];
    DdNode *y[(int) ceil(log2(size_col))];
    DdNode *xn[(int) ceil(log2(size_row))];
    DdNode *yn_[(int) ceil(log2(size_col))];
    double matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    matrixToADD(gbm, (double **)matrix, &E, &x, &y, &xn, &yn_, &nx, &ny, &m, &n);

    symbolic_to_numeric(E, 3, 3);  

    // Assert
    // CUDD_VALUE_TYPE expected[3][3] = {
    //     {1, 2, 3},
    //     {4, 5, 6},
    //     {7, 8, 9}};

    Cudd_Quit(gbm);
}

Suite *matrix_to_add_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Matrix to ADD");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_matrix_to_add);
    suite_add_tcase(s, tc_core);

    return s;
}