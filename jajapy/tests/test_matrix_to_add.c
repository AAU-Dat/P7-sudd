#include <check.h>
#include "../base/symbolic_to_numeric.h"
#include "../base/matrix_to_add.h"
#include "cudd.h"

START_TEST(test_matrix_to_add)
{

    // initialize variables
    // DdManager *gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    // DdNode *E, **x, **y, **xn, **yn_;
    // int nx, ny, m, n;
    // int size_row = 2;
    // int size_col = 3;
    // nx = 0;
    // ny = 0;
    // x = (DdNode **)malloc(sizeof(DdNode *) * ceil(log(size_row)));
    // y = (DdNode **)malloc(sizeof(DdNode *) * ceil(log(size_col)));
    // xn = (DdNode **)malloc(sizeof(DdNode *) * ceil(log(size_row)));
    // yn_ = (DdNode **)malloc(sizeof(DdNode *) * ceil(log(size_col)));

    // // matrixToADD(gbm, matrix, E, x, y, xn, yn_, nx, ny, m, n);

    // symbolic_to_numeric(E, 3, 3);

    // // Assert
    // // CUDD_VALUE_TYPE expected[3][3] = {
    // //     {1, 2, 3},
    // //     {4, 5, 6},
    // //     {7, 8, 9}};

    // Cudd_Quit(gbm);
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