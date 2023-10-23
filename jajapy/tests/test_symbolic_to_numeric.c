#include <check.h>
#include <unistd.h>
#include "../base/symbolic_to_numeric.h"
#include "cudd.h"
#include "helper.h"

START_TEST(test_matrix_2x2) {
    // Arrange
    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    double matrix[2][2] = {
        {1, 2},
        {3, 4}
    };
    DdNode* _matrix = matrix_2x2(manager, matrix, row_var, col_var);
    Cudd_Ref(_matrix);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(_matrix, 2, 2);

    // Assert
    ck_assert_double_eq(matrix[0][0], actual[0][0]);
    ck_assert_double_eq(matrix[0][1], actual[0][1]);
    ck_assert_double_eq(matrix[1][0], actual[1][0]);
    ck_assert_double_eq(matrix[1][1], actual[1][1]);
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

START_TEST(matrix_1x4) {
    // Arrange
    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    double matrix[2][2] = {
        {1, 2},
        {3, 4}
    };
    DdNode* _matrix = matrix_2x2(manager, matrix, row_var, col_var);
    Cudd_Ref(_matrix);
    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(_matrix, 1, 4);

    // Assert
    CUDD_VALUE_TYPE expected[1][4] = {
        {1, 2, 3, 4}
    };
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[0][2], actual[0][2]);
    ck_assert_double_eq(expected[0][3], actual[0][3]);
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

START_TEST(matrix_4x1) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    double matrix[2][2] = {
        {1, 2},
        {3, 4}
    };
    DdNode* _matrix = matrix_2x2(manager, matrix, row_var, col_var);
    Cudd_Ref(_matrix);
    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(_matrix, 4, 1);

    // Assert
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
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

START_TEST(matrix_1x1) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* add_one = Cudd_addConst(manager, 1);
    Cudd_Ref(add_one);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(add_one, 1, 1);

    // Assert
    CUDD_VALUE_TYPE expected[1][1] = {{1}};
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Cleanup
    Cudd_RecursiveDeref(manager, add_one);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(test_matrix_3x3) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* row_var_0 = Cudd_addNewVar(manager);
    Cudd_Ref(row_var_0);
    DdNode* row_var_1 = Cudd_addNewVar(manager);
    Cudd_Ref(row_var_1);
    DdNode* row_vars[2] = {row_var_0, row_var_1};

    DdNode* col_var_0 = Cudd_addNewVar(manager);
    Cudd_Ref(col_var_0);
    DdNode* col_var_1 = Cudd_addNewVar(manager);
    Cudd_Ref(col_var_1);
    DdNode* col_vars[2] = {col_var_0, col_var_1};

    double matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    DdNode* _matrix = matrix_3x3(manager, matrix, row_vars, col_vars);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(_matrix, 3, 3);

    // Assert
    CUDD_VALUE_TYPE expected[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[0][2], actual[0][2]);

    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[1][1], actual[1][1]);
    ck_assert_double_eq(expected[1][2], actual[1][2]);

    ck_assert_double_eq(expected[2][0], actual[2][0]);
    ck_assert_double_eq(expected[2][1], actual[2][1]);
    ck_assert_double_eq(expected[2][2], actual[2][2]);


    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

Suite* symbolic_to_numeric_suite(void) {
    Suite* s;
    TCase* tc_symbolic_to_numeric;

    s = suite_create("Symbolic to numeric conversion");

    tc_symbolic_to_numeric = tcase_create("symbolic_to_numeric");

    tcase_add_test(tc_symbolic_to_numeric, test_matrix_2x2);
    tcase_add_test(tc_symbolic_to_numeric, matrix_1x4);
    tcase_add_test(tc_symbolic_to_numeric, matrix_4x1);
    tcase_add_test(tc_symbolic_to_numeric, matrix_1x1);
    tcase_add_test(tc_symbolic_to_numeric, test_matrix_3x3);

    suite_add_tcase(s, tc_symbolic_to_numeric);
    return s;
}

