#include <check.h>
#include "../base/symbolic_to_numeric.h"
#include "cudd.h"

START_TEST(matrix_2x2) {
    // Arrange
    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode *add_one = Cudd_addConst(manager, 1);
    Cudd_Ref(add_one);

    DdNode *add_two = Cudd_addConst(manager, 2);
    Cudd_Ref(add_two);

    DdNode *add_three = Cudd_addConst(manager, 3);
    Cudd_Ref(add_three);

    DdNode *add_four = Cudd_addConst(manager, 4);
    Cudd_Ref(add_four);

    DdNode *x0 = Cudd_addIthVar(manager, 0);
    DdNode *x1 = Cudd_addIthVar(manager, 1);

    DdNode *matrix = Cudd_addIte(manager, x0, Cudd_addIte(manager, x1, add_four, add_three), Cudd_addIte(manager, x1, add_two, add_one));
    Cudd_Ref(matrix);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(matrix, 2, 2);

    // Assert
    CUDD_VALUE_TYPE expected[2][2] = {
        {1, 2},
        {3, 4}
    };
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[1][1], actual[1][1]);

    // Cleanup
    Cudd_RecursiveDeref(manager, add_one);
    Cudd_RecursiveDeref(manager, add_two);
    Cudd_RecursiveDeref(manager, add_three);
    Cudd_RecursiveDeref(manager, add_four);
    Cudd_RecursiveDeref(manager, matrix);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(matrix_1x4) {
    // Arrange
    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode *add_one = Cudd_addConst(manager, 1);
    Cudd_Ref(add_one);

    DdNode *add_two = Cudd_addConst(manager, 2);
    Cudd_Ref(add_two);

    DdNode *add_three = Cudd_addConst(manager, 3);
    Cudd_Ref(add_three);

    DdNode *add_four = Cudd_addConst(manager, 4);
    Cudd_Ref(add_four);

    DdNode *x0 = Cudd_addIthVar(manager, 0);
    DdNode *x1 = Cudd_addIthVar(manager, 1);

    DdNode *matrix = Cudd_addIte(manager, x0, Cudd_addIte(manager, x1, add_four, add_three), Cudd_addIte(manager, x1, add_two, add_one));
    Cudd_Ref(matrix);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(matrix, 1, 4);

    // Assert
    CUDD_VALUE_TYPE expected[1][4] = {
        {1, 2, 3, 4}
    };
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[0][2], actual[0][2]);
    ck_assert_double_eq(expected[0][3], actual[0][3]);

    // Cleanup
    Cudd_RecursiveDeref(manager, add_one);
    Cudd_RecursiveDeref(manager, add_two);
    Cudd_RecursiveDeref(manager, add_three);
    Cudd_RecursiveDeref(manager, add_four);
    Cudd_RecursiveDeref(manager, matrix);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(matrix_4x1) {
    // Arrange
    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode *add_one = Cudd_addConst(manager, 1);
    Cudd_Ref(add_one);

    DdNode *add_two = Cudd_addConst(manager, 2);
    Cudd_Ref(add_two);

    DdNode *add_three = Cudd_addConst(manager, 3);
    Cudd_Ref(add_three);

    DdNode *add_four = Cudd_addConst(manager, 4);
    Cudd_Ref(add_four);

    DdNode *x0 = Cudd_addIthVar(manager, 0);
    DdNode *x1 = Cudd_addIthVar(manager, 1);

    DdNode *matrix = Cudd_addIte(manager, x0, Cudd_addIte(manager, x1, add_four, add_three), Cudd_addIte(manager, x1, add_two, add_one));
    Cudd_Ref(matrix);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(matrix, 4, 1);

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

    // Cleanup
    Cudd_RecursiveDeref(manager, add_one);
    Cudd_RecursiveDeref(manager, add_two);
    Cudd_RecursiveDeref(manager, add_three);
    Cudd_RecursiveDeref(manager, add_four);
    Cudd_RecursiveDeref(manager, matrix);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(matrix_1x1) { // TODO fix this
    // Arrange
    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode *add_one = Cudd_addConst(manager, 1);
    Cudd_Ref(add_one);

    DdNode *add_two = Cudd_addConst(manager, 2);
    Cudd_Ref(add_two);

    DdNode *add_three = Cudd_addConst(manager, 3);
    Cudd_Ref(add_three);

    DdNode *add_four = Cudd_addConst(manager, 4);
    Cudd_Ref(add_four);

    DdNode *x0 = Cudd_addIthVar(manager, 0);
    DdNode *x1 = Cudd_addIthVar(manager, 1);

    DdNode *matrix = Cudd_addIte(manager, x0, Cudd_addIte(manager, x1, add_four, add_three), Cudd_addIte(manager, x1, add_two, add_one));
    Cudd_Ref(matrix);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(matrix, 1, 1);

    // Assert
    CUDD_VALUE_TYPE expected[1][1] = {{1}};
    ck_assert_double_eq(expected[0][0], actual[0][0]);

    // Cleanup
    Cudd_RecursiveDeref(manager, add_one);
    Cudd_RecursiveDeref(manager, add_two);
    Cudd_RecursiveDeref(manager, add_three);
    Cudd_RecursiveDeref(manager, add_four);
    Cudd_RecursiveDeref(manager, matrix);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(matrix_3x3) {
}
END_TEST

Suite* symbolic_to_numeric_suite(void) {
    Suite* s;
    TCase* tc_symbolic_to_numeric;

    s = suite_create("Symbolic to numeric conversion");

    tc_symbolic_to_numeric = tcase_create("symbolic_to_numeric");

    tcase_add_test(tc_symbolic_to_numeric, matrix_2x2);
    tcase_add_test(tc_symbolic_to_numeric, matrix_1x4);
    tcase_add_test(tc_symbolic_to_numeric, matrix_4x1);
    tcase_add_test(tc_symbolic_to_numeric, matrix_1x1);
    tcase_add_test(tc_symbolic_to_numeric, matrix_3x3);

    suite_add_tcase(s, tc_symbolic_to_numeric);
    return s;
}
