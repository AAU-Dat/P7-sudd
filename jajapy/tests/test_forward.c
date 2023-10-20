#include "test_forward.h"
#include "../base/symbolic_to_numeric.h"
#include <cudd.h>

START_TEST(_forwards_2x2) {
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

    DdNode* omega[3] = {omega0, omega1};

    DdNode* pi = Cudd_addIte(
        manager,
        row_var,
        add2,
        add1
    );

    // Act
    DdNode** actual = _forward(manager, omega, P, pi, &row_var, &column_var, n_vars, n_obs);
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

START_TEST(_backwards_2x2) {
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

    DdNode* omega[3] = {omega0, omega1};

    DdNode* pi = Cudd_addIte(
        manager,
        row_var,
        add2,
        add1
    );

    // Act
    DdNode** actual = _backwards(manager, omega, P, pi, &row_var, &column_var, n_vars, n_obs);

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

Suite* forward_suite(void) {
    Suite* s;
    TCase* tc_forward;

    s = suite_create("forward");

    tc_forward = tcase_create("forward");

    tcase_add_test(tc_forward, _forwards_2x2);
    tcase_add_test(tc_forward, _backwards_2x2);

    suite_add_tcase(s, tc_forward);
    return s;
}
