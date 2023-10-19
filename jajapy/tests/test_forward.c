#include "test_forward.h"

START_TEST(transpose_matrix_3x3) {
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
    
    // 1 2 3
    // 4 5 6
    // 7 8 9     
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
        ),
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
    double** actual = symbolic_to_numeric(add_square_transpose(manager, matrix, 3), 3, 3);

    // Assert
    double expected[3][3] = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
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

Suite* forward_suite(void) {
    Suite* s;
    TCase* tc_forward;

    s = suite_create("forward");

    tc_forward = tcase_create("forward");

    tcase_add_test(tc_forward, transpose_matrix_3x3);

    suite_add_tcase(s, tc_forward);
    return s;
}
