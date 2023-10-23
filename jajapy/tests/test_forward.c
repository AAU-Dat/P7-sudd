#include "test_forward.h"
#include "../base/symbolic_to_numeric.h"
#include "helper.h"
#include <check.h>
#include <cudd.h>
#include <unistd.h>

START_TEST(_forwards_2x2_1_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int k_j = 1;
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
    DdNode** _alpha = _forwards(manager, _omega, _P, _pi, &row_var, &col_var, n_vars, k_j);
    double** alpha0 = symbolic_to_numeric(_alpha[0], 2, 1);
    double** alpha1 = symbolic_to_numeric(_alpha[1], 2, 1);

    // Assert
    for (int s = 0; s < n_states; s++) {
        ck_assert_double_eq(pi[s] * omega0[s], alpha0[s][0]);
        ck_assert_double_eq(omega1[s] * (P[0][s] * alpha0[0][0] + P[1][s] * alpha0[1][0]), alpha1[s][0]);
    }
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Clean
    Cudd_Quit(manager);
}

START_TEST(_backwards_2x2_1_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int k_j = 1;
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
    DdNode** _beta = _backwards(manager, _omega, _P, _pi, &row_var, &col_var, n_vars, k_j);
    double** beta0 = symbolic_to_numeric(_beta[0], 2, 1);
    double** beta1 = symbolic_to_numeric(_beta[1], 2, 1);

    // Assert
    for (int s = 0; s < n_states; s++) {
        ck_assert_double_eq(P[s][0] * omega1[0] * beta1[0][0] + P[s][1] * omega1[1] * beta1[1][0], beta0[s][0]);
        ck_assert_double_eq(1, beta1[s][0]);
    }
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Clean
    Cudd_Quit(manager);
}

START_TEST(_forwards_3x3_2_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    const int k_j = 2;
    int n_vars = 2;
    int n_states = 3;

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
    DdNode** _alpha = _forwards(manager, _omega, _P, _pi, row_vars, col_vars, n_vars, k_j);
    double** alpha0 = symbolic_to_numeric(_alpha[0], n_states, 1);
    double** alpha1 = symbolic_to_numeric(_alpha[1], n_states, 1);
    double** alpha2 = symbolic_to_numeric(_alpha[2], n_states, 1);

    for (int s = 0; s < n_states; s++) {
        ck_assert_double_eq(pi[s] * omega0[s], alpha0[s][0]);
        ck_assert_double_eq(omega1[s] * (P[0][s] * alpha0[0][0] + P[1][s] * alpha0[1][0] + P[2][s] * alpha0[2][0]), alpha1[s][0]);
        ck_assert_double_eq(omega2[s] * (P[0][s] * alpha1[0][0] + P[1][s] * alpha1[1][0] + P[2][s] * alpha1[2][0]), alpha2[s][0]);
    }
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Clean
    Cudd_Quit(manager);
}

START_TEST(_backwards_3x3_2_k_j) {
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    const int k_j = 2;
    int n_vars = 2;
    int n_states = 3;

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
    DdNode** _beta = _backwards(manager, _omega, _P, _pi, row_vars, col_vars, n_vars, k_j);
    double** beta0 = symbolic_to_numeric(_beta[0], n_states, 1);
    double** beta1 = symbolic_to_numeric(_beta[1], n_states, 1);
    double** beta2 = symbolic_to_numeric(_beta[2], n_states, 1);

    for (int s = 0; s < n_states; s++) {
        ck_assert_double_eq(P[s][0] * omega1[0] * beta1[0][0] + P[s][1] * omega1[1] * beta1[1][0] + P[s][2] * omega1[2] * beta1[2][0], beta0[s][0]);
        ck_assert_double_eq(P[s][0] * omega2[0] * beta2[0][0] + P[s][1] * omega2[1] * beta2[1][0] + P[s][2] * omega2[2] * beta2[2][0], beta1[s][0]);
        ck_assert_double_eq(1, beta2[s][0]);
    }
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Clean
    Cudd_Quit(manager);
}

Suite* forward_suite(void) {
    Suite* s;
    TCase* tc_forward;

    s = suite_create("forward");

    tc_forward = tcase_create("forward");

    tcase_add_test(tc_forward, _forwards_2x2_1_k_j);
    tcase_add_test(tc_forward, _backwards_2x2_1_k_j);
    tcase_add_test(tc_forward, _forwards_3x3_2_k_j);
    tcase_add_test(tc_forward, _backwards_3x3_2_k_j);

    suite_add_tcase(s, tc_forward);
    return s;
}
