#include "test_forward.h"
#include "../base/symbolic_to_numeric.h"
#include "helper.h"
#include <check.h>
#include <cudd.h>

START_TEST(_forwards_2x2_1_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int n_obs = 1;
    int n_vars = 1;
    int n_states = 2;

    DdNode* row_var = Cudd_addNewVar(manager);
    DdNode* col_var = Cudd_addNewVar(manager);

    double P[2][2] = {
        {1, 2},
        {3, 4}
    };
    DdNode* _P = matrix_2x2(manager, P, row_var, col_var);

    double omega0[2] = {1, 2};
    DdNode* _omega0 = vector_2x1(manager, omega0, row_var);

    double omega1[2] = {3, 4};
    DdNode* _omega1 = vector_2x1(manager, omega1, row_var);

    DdNode* _omega[2] = {_omega0, _omega1};

    double pi[2] = {1, 2};
    DdNode* _pi = vector_2x1(manager, pi, row_var);

    // Act
    DdNode** _alpha = _forwards(manager, _omega, _P, _pi, &row_var, &col_var, n_vars, n_obs);
    double** alpha0 = symbolic_to_numeric(_alpha[0], 2, 1);
    double** alpha1 = symbolic_to_numeric(_alpha[1], 2, 1);

    // Assert
    for (int s = 0; s < n_states; s++) {
        ck_assert_double_eq(pi[s] * omega0[s], alpha0[s][0]);
        ck_assert_double_eq(omega1[s] * (P[0][s] * alpha0[0][0] + P[1][s] * alpha0[1][0]), alpha1[s][0]);
    }

    Cudd_Quit(manager);
}

START_TEST(_backwards_2x2_1_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int n_obs = 1;
    int n_vars = 1;
    int n_states = 2;

    DdNode* row_var = Cudd_addNewVar(manager);
    DdNode* col_var = Cudd_addNewVar(manager);

    double P[2][2] = {
        {1, 2},
        {3, 4}
    };
    DdNode* _P = matrix_2x2(manager, P, row_var, col_var);

    double omega0[2] = {1, 2};
    DdNode* _omega0 = vector_2x1(manager, omega0, row_var);

    double omega1[2] = {3, 4};
    DdNode* _omega1 = vector_2x1(manager, omega1, row_var);

    DdNode* _omega[2] = {_omega0, _omega1};

    double pi[2] = {1, 2};
    DdNode* _pi = vector_2x1(manager, pi, row_var);

    // Act
    DdNode** _beta = _backwards(manager, _omega, _P, _pi, &row_var, &col_var, n_vars, n_obs);
    double** beta0 = symbolic_to_numeric(_beta[0], 2, 1);
    double** beta1 = symbolic_to_numeric(_beta[1], 2, 1);

    // Assert
    for (int s = 0; s < n_states; s++) {
        ck_assert_double_eq(P[s][0] * omega1[0] * beta1[0][0] + P[s][1] * omega1[1] * beta1[1][0], beta0[s][0]);
        ck_assert_double_eq(1, beta1[s][0]);
    }

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
    
    double P[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    DdNode* _P = matrix_3x3(manager, P, row_vars, col_vars);
    
    double pi[3] = {1, 2, 3};
    DdNode* _pi = vector_3x1(manager, pi, row_vars);

    double omega0[3] = {1, 2, 3};
    DdNode* _omega0 = vector_3x1(manager, omega0, row_vars);
    
    double omega1[3] = {4, 5, 6};
    DdNode* _omega1 = vector_3x1(manager, omega1, row_vars);

    double omega2[3] = {7, 8, 9};
    DdNode* _omega2 = vector_3x1(manager, omega2, row_vars);

    DdNode* _omega[3] = {_omega0, _omega1, _omega2};
    
    // Act
    DdNode** actual = _forwards(manager, _omega, _P, _pi, row_vars, col_vars, n_vars, k_j);

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
