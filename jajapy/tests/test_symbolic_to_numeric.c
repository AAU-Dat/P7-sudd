#include <check.h>
#include "../base/symbolic_to_numeric.h"
#include "cudd.h"
#include "helper.h"

START_TEST(test_matrix_2x2) {
    // Arrange
    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* row_var = Cudd_addIthVar(manager, 0);
    DdNode* col_var = Cudd_addIthVar(manager, 1);

    double matrix[2][2] = {
        {1, 2},
        {3, 4}
    };

    DdNode* _matrix = matrix_2x2(manager, matrix, row_var, col_var);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(_matrix, 2, 2);

    // Assert
    ck_assert_double_eq(matrix[0][0], actual[0][0]);
    ck_assert_double_eq(matrix[0][1], actual[0][1]);
    ck_assert_double_eq(matrix[1][0], actual[1][0]);
    ck_assert_double_eq(matrix[1][1], actual[1][1]);

    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

START_TEST(matrix_1x4) {
    // Arrange
    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* add_one = Cudd_addConst(manager, 1);
    Cudd_Ref(add_one);

    DdNode* add_two = Cudd_addConst(manager, 2);
    Cudd_Ref(add_two);

    DdNode* add_three = Cudd_addConst(manager, 3);
    Cudd_Ref(add_three);

    DdNode* add_four = Cudd_addConst(manager, 4);
    Cudd_Ref(add_four);

    DdNode* x0 = Cudd_addIthVar(manager, 0);
    DdNode* x1 = Cudd_addIthVar(manager, 1);

    DdNode* matrix = Cudd_addIte(manager, x0, Cudd_addIte(manager, x1, add_four, add_three), Cudd_addIte(manager, x1, add_two, add_one));
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
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* add_one = Cudd_addConst(manager, 1);
    Cudd_Ref(add_one);

    DdNode* add_two = Cudd_addConst(manager, 2);
    Cudd_Ref(add_two);

    DdNode* add_three = Cudd_addConst(manager, 3);
    Cudd_Ref(add_three);

    DdNode* add_four = Cudd_addConst(manager, 4);
    Cudd_Ref(add_four);

    DdNode* x0 = Cudd_addIthVar(manager, 0);
    DdNode* x1 = Cudd_addIthVar(manager, 1);

    DdNode* matrix = Cudd_addIte(manager, x0, Cudd_addIte(manager, x1, add_four, add_three), Cudd_addIte(manager, x1, add_two, add_one));
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

    // Cleanup
    Cudd_RecursiveDeref(manager, add_one);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(test_matrix_3x3) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* x1 = Cudd_addIthVar(manager, 0);
    Cudd_Ref(x1);
    DdNode* y1 = Cudd_addIthVar(manager, 1);
    Cudd_Ref(y1);
    DdNode* x2 = Cudd_addIthVar(manager, 2);
    Cudd_Ref(x2);
    DdNode* y2 = Cudd_addIthVar(manager, 3);
    Cudd_Ref(y2);

    DdNode* add1 = Cudd_addConst(manager, 1);
    Cudd_Ref(add1);
    DdNode* add2 = Cudd_addConst(manager, 2);
    Cudd_Ref(add2);
    DdNode* add3 = Cudd_addConst(manager, 3);
    Cudd_Ref(add3);
    DdNode* add4 = Cudd_addConst(manager, 4);
    Cudd_Ref(add4);
    DdNode* add5 = Cudd_addConst(manager, 5);
    Cudd_Ref(add5);
    DdNode* add6 = Cudd_addConst(manager, 6);
    Cudd_Ref(add6);
    DdNode* add7 = Cudd_addConst(manager, 7);
    Cudd_Ref(add7);
    DdNode* add8 = Cudd_addConst(manager, 8);
    Cudd_Ref(add8);
    DdNode* add9 = Cudd_addConst(manager, 9);
    Cudd_Ref(add9);
    DdNode* add0 = Cudd_addConst(manager, 0);
    Cudd_Ref(add0);

    DdNode* matrix = Cudd_addIte(
        manager,
        x1,
        Cudd_addIte(
            manager,
            y1,
            Cudd_addIte(
                manager,
                x2,
                Cudd_addIte(
                    manager,
                    y2,
                    add0,
                    add0
                ),
                Cudd_addIte(
                    manager,
                    y2,
                    add0,
                    add9
                )
            ),
            Cudd_addIte(
                manager,
                x2,
                Cudd_addIte(
                    manager,
                    y2,
                    add0,
                    add0

                ),
                Cudd_addIte(
                    manager,
                    y2,
                    add8,
                    add7
                )
            )
        ),CUDD_VALUE_TYPE
        Cudd_addIte(
            manager,
            y1,
            Cudd_addIte(
                manager,
                x2,
                Cudd_addIte(
                    manager,
                    y2,
                    add0,
                    add6
                ),
                Cudd_addIte(
                    manager,
                    y2,
                    add0,
                    add3
                )
            ),
            Cudd_addIte(
                manager,
                x2,
                Cudd_addIte(
                    manager,
                    y2,
                    add5,
                    add4
                ),
                Cudd_addIte(
                    manager,
                    y2,
                    add2,
                    add1
                )
            )
        )
    );
    Cudd_Ref(matrix);

    // Act
    CUDD_VALUE_TYPE** actual = symbolic_to_numeric(matrix, 3, 3);

    // Assert
    CUDD_VALUE_TYPE expected[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

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
    Cudd_RecursiveDeref(manager, add1);
    Cudd_RecursiveDeref(manager, add2);
    Cudd_RecursiveDeref(manager, add3);
    Cudd_RecursiveDeref(manager, add4);
    Cudd_RecursiveDeref(manager, add5);
    Cudd_RecursiveDeref(manager, add6);
    Cudd_RecursiveDeref(manager, add7);
    Cudd_RecursiveDeref(manager, add8);
    Cudd_RecursiveDeref(manager, add9);

    Cudd_RecursiveDeref(manager, x1);
    Cudd_RecursiveDeref(manager, y1);
    Cudd_RecursiveDeref(manager, x2);
    Cudd_RecursiveDeref(manager, y2);

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
