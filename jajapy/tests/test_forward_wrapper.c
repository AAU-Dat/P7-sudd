#include "test_forward_wrapper.h"
#include "../base/symbolic_to_numeric.h"
#include "../base/forward.h"
#include <cudd.h>
#include <unistd.h>
#include <check.h>


START_TEST(forwards_wrapper_2x2_1_k_j)
{
    int states = 3, k_j = 2;
    // make matrix for omega that has the size k_j+1 x states
    double **omega = (double **)malloc((k_j + 1) * sizeof(double *));
    for (int i = 0; i < k_j + 1; i++)
    {
        omega[i] = (double *)malloc(states * sizeof(double));
    }
    omega[0][0] = 1;
    omega[0][1] = 2;
    omega[0][2] = 3;
    omega[1][0] = 4;
    omega[1][1] = 5;
    omega[1][2] = 6;
    omega[2][0] = 7;
    omega[2][1] = 8;
    omega[2][2] = 9;

    // make matrix for P that has the size states x states
    double **P = (double **)malloc(states * sizeof(double *));
    double **pi = (double **)malloc(states * sizeof(double *));
    for (int i = 0; i < states; i++)
    {
        P[i] = (double *)malloc(states * sizeof(double));
        pi[i] = (double *)malloc(states * sizeof(double));
    }
    P[0][0] = 1;
    P[0][1] = 2;
    P[0][2] = 3;
    P[1][0] = 4;
    P[1][1] = 5;
    P[1][2] = 6;
    P[2][0] = 7;
    P[2][1] = 8;
    P[2][2] = 9;

    pi[0][0] = 1;
    pi[0][1] = 2;
    pi[0][2] = 3;

    CUDD_VALUE_TYPE** alpha = forward(omega, P, pi, states, k_j);

    for (int s = 0; s < states; s++)
    {
        //ck_assert_double_eq(pi[0][s] * omega[0][s], alpha[0][s]);
        //ck_assert_double_eq(omega[1][s] * (P[0][s] * alpha[0][0] + P[1][s] * alpha[0][1] + P[2][s] * alpha[0][2]), alpha[1][s]);
        //ck_assert_double_eq(omega[2][s] * (P[0][s] * alpha[1][0] + P[1][s] * alpha[1][1] + P[2][s] * alpha[1][2]), alpha[2][s]);
    }

    // clean up
    for (int i = 0; i <= k_j; i++)
    {
        free(omega[i]);
    }
    for (int i = 0; i < states; i++)
    {
        free(P[i]);
        free(pi[i]);
    }
    free(omega);
    free(P);
    free(pi);
}

Suite *forward_wrapper_suite(void)
{
    Suite *s;
    TCase *tc_forward_wrapper;

    s = suite_create("forward_wrapper");

    tc_forward_wrapper = tcase_create("forward_wrapper");

    tcase_add_test(tc_forward_wrapper, forwards_wrapper_2x2_1_k_j);

    suite_add_tcase(s, tc_forward_wrapper);
    return s;
}