#include "test_forward.h"
#include "../base/symbolic_to_numeric.h"
#include <cudd.h>

START_TEST(_forwards_2x2_1_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int n_obs = 1;
    int n_vars = 1;

    DdNode* row_var = Cudd_addNewVar(manager);
    DdNode* column_var = Cudd_addNewVar(manager);

    DdNode* add1 = Cudd_addConst(manager, 1);
    DdNode* add2 = Cudd_addConst(manager, 2);
    DdNode* add3 = Cudd_addConst(manager, 3);
    DdNode* add4 = Cudd_addConst(manager, 4);

    DdNode* P = Cudd_addIte(
        manager,
        row_var,
        Cudd_addIte(
            manager,
            column_var,
            add4,
            add3
        ), 
        Cudd_addIte(
            manager,
            column_var,
            add2, 
            add1
        )
    );

    DdNode* omega0 = Cudd_addIte(
        manager,
        row_var,
        add2,
        add1
    );

    DdNode* omega1 = Cudd_addIte(
        manager,
        row_var,
        add4,
        add3
    );

    DdNode* omega[2] = {omega0, omega1};

    DdNode* pi = Cudd_addIte(
        manager,
        row_var,
        add2,
        add1
    );

    // Act
    DdNode** actual = _forwards(manager, omega, P, pi, &row_var, &column_var, n_vars, n_obs);
    // DdNode* actual = Cudd_addApply(manager, Cudd_addTimes, omega[0], pi);

    // Assert
    double expected[2][2] = {
        {1, 4},
        {39, 72}
    };
    ck_assert_double_eq(expected[0][0], symbolic_to_numeric(actual[0], 2, 1)[0][0]);
    ck_assert_double_eq(expected[0][1], symbolic_to_numeric(actual[0], 2, 1)[1][0]);
    ck_assert_double_eq(expected[1][0], symbolic_to_numeric(actual[1], 2, 1)[0][0]);
    ck_assert_double_eq(expected[1][1], symbolic_to_numeric(actual[1], 2, 1)[1][0]);

    Cudd_Quit(manager);
}

START_TEST(_backwards_2x2_1_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int k_j = 1;
    int n_vars = 1;

    DdNode* row_var = Cudd_addNewVar(manager);
    DdNode* column_var = Cudd_addNewVar(manager);

    DdNode* add1 = Cudd_addConst(manager, 1);
    DdNode* add2 = Cudd_addConst(manager, 2);
    DdNode* add3 = Cudd_addConst(manager, 3);
    DdNode* add4 = Cudd_addConst(manager, 4);

    DdNode* P = Cudd_addIte(
        manager,
        row_var,
        Cudd_addIte(
            manager,
            column_var,
            add4,
            add3
        ), 
        Cudd_addIte(
            manager,
            column_var,
            add2, 
            add1
        )
    );

    DdNode* omega0 = Cudd_addIte(
        manager,
        row_var,
        add2,
        add1
    );

    DdNode* omega1 = Cudd_addIte(
        manager,
        row_var,
        add4,
        add3
    );

    DdNode* omega[2] = {omega0, omega1};

    DdNode* pi = Cudd_addIte(
        manager,
        row_var,
        add2,
        add1
    );

    // Act
    DdNode** actual = _backwards(manager, omega, P, pi, &row_var, &column_var, n_vars, k_j);

    // Assert
    double expected[2][2] = {
        {11, 25},
        {1, 1}
    };
    ck_assert_double_eq(expected[0][0], symbolic_to_numeric(actual[0], 2, 1)[0][0]);
    ck_assert_double_eq(expected[0][1], symbolic_to_numeric(actual[0], 2, 1)[1][0]);
    ck_assert_double_eq(expected[1][0], symbolic_to_numeric(actual[1], 2, 1)[0][0]);
    ck_assert_double_eq(expected[1][1], symbolic_to_numeric(actual[1], 2, 1)[1][0]);

    Cudd_Quit(manager);
}

START_TEST(_backwards_3x3_2_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int k_j = 2;
    int n_vars = 2;

    DdNode* row_var_0 = Cudd_addNewVar(manager);
    DdNode* row_var_1 = Cudd_addNewVar(manager);
    DdNode* row_vars[2] = {row_var_0, row_var_1};

    DdNode* col_var_0 = Cudd_addNewVar(manager);
    DdNode* col_var_1 = Cudd_addNewVar(manager);
    DdNode* col_vars[2] = {col_var_0, col_var_1};

    DdNode* add1 = Cudd_addConst(manager, 1);
    DdNode* add2 = Cudd_addConst(manager, 2);
    DdNode* add3 = Cudd_addConst(manager, 3);
    DdNode* add4 = Cudd_addConst(manager, 4);
    DdNode* add5 = Cudd_addConst(manager, 5);
    DdNode* add6 = Cudd_addConst(manager, 6);
    DdNode* add7 = Cudd_addConst(manager, 7);
    DdNode* add8 = Cudd_addConst(manager, 8);
    DdNode* add9 = Cudd_addConst(manager, 9);
    DdNode* add0 = Cudd_addConst(manager, 0);
    
    DdNode* P = Cudd_addIte(
        manager,
        row_var_0,
        Cudd_addIte(
            manager,
            col_var_0,
            Cudd_addIte(
                manager,
                row_var_1,
                Cudd_addIte(
                    manager,
                    col_var_1,
                    add0,
                    add0
                ),
                Cudd_addIte(
                    manager,
                    col_var_1,
                    add0,
                    add9
                )
            ),
            Cudd_addIte(
                manager,
                row_var_1,
                Cudd_addIte(
                    manager,
                    col_var_1,
                    add0,
                    add0

                ),
                Cudd_addIte(
                    manager,
                    col_var_1,
                    add8,
                    add7
                )
            )
        ),
        Cudd_addIte(
            manager,
            col_var_0,
            Cudd_addIte(
                manager,
                row_var_1,
                Cudd_addIte(
                    manager,
                    col_var_1,
                    add0,
                    add6
                ),
                Cudd_addIte(
                    manager,
                    col_var_1,
                    add0,
                    add3
                )
            ),
            Cudd_addIte(
                manager,
                row_var_1,
                Cudd_addIte(
                    manager,
                    col_var_1,
                    add5,
                    add4
                ),
                Cudd_addIte(
                    manager,
                    col_var_1,
                    add2,
                    add1
                )
            )
        )
    );
    
    DdNode* pi = Cudd_addIte(
        manager,
        row_var_0,
        Cudd_addIte(
            manager,
            row_var_1,
            add0,
            add3
        ),
        Cudd_addIte(
            manager,
            row_var_1,
            add2,
            add1
        )
    );

    DdNode* omega0 = Cudd_addIte(
        manager,
        row_var_0,
        Cudd_addIte(
            manager,
            row_var_1,
            add0,
            add3
        ),
        Cudd_addIte(
            manager,
            row_var_1,
            add2,
            add1
        )
    );
    

    DdNode* omega1 = Cudd_addIte(
        manager,
        row_var_0,
        Cudd_addIte(
            manager,
            row_var_1,
            add0,
            add6
        ),
        Cudd_addIte(
            manager,
            row_var_1,
            add5,
            add4
        )
    );

    DdNode* omega2 = Cudd_addIte(
        manager,
        row_var_0,
        Cudd_addIte(
            manager,
            row_var_1,
            add0,
            add9
        ),
        Cudd_addIte(
            manager,
            row_var_1,
            add8,
            add7
        )
    );

    DdNode* omega[3] = {omega0, omega1, omega2};
    
    // Act
    DdNode** actual = _forwards(manager, omega, P, pi, row_vars, col_vars, n_vars, k_j);

    // Assert
    double expected[3][3] = {
        {    1,     4,     9},
        {  320,   470,   648},
        {47152, 65392, 86508}
    };

    ck_assert_double_eq(expected[0][0], symbolic_to_numeric(actual[0], 3, 1)[0][0]);
    ck_assert_double_eq(expected[0][1], symbolic_to_numeric(actual[0], 3, 1)[1][0]);
    ck_assert_double_eq(expected[0][2], symbolic_to_numeric(actual[0], 3, 1)[2][0]);

    ck_assert_double_eq(expected[1][0], symbolic_to_numeric(actual[1], 3, 1)[0][0]);
    ck_assert_double_eq(expected[1][1], symbolic_to_numeric(actual[1], 3, 1)[1][0]);
    ck_assert_double_eq(expected[1][2], symbolic_to_numeric(actual[1], 3, 1)[2][0]);
    
    ck_assert_double_eq(expected[2][0], symbolic_to_numeric(actual[2], 3, 1)[0][0]);
    ck_assert_double_eq(expected[2][1], symbolic_to_numeric(actual[2], 3, 1)[1][0]);
    ck_assert_double_eq(expected[2][2], symbolic_to_numeric(actual[2], 3, 1)[2][0]);
    
}

Suite* forward_suite(void) {
    Suite* s;
    TCase* tc_forward;

    s = suite_create("forward");

    tc_forward = tcase_create("forward");

    tcase_add_test(tc_forward, _forwards_2x2_1_k_j);
    tcase_add_test(tc_forward, _backwards_2x2_1_k_j);
    tcase_add_test(tc_forward, _backwards_3x3_2_k_j);

    suite_add_tcase(s, tc_forward);
    return s;
}
