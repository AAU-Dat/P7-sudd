#include "forward.h"
#include "symbolic_to_numeric.h"
#include <cudd.h>
#include <stdlib.h>
// #include "matrix_to_add.h"

// Convert omega, P and pi to ADDs, and call _forward on those.
// double** forward(
//     double** omega,
//     double** P, 
//     double* pi,
//     int n_states,
//     int k_j
// ) {
//     DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
//
//     int n_state_variables = ceil(log2(n_states));
//
//     DdNode* x[n_state_variables];
//     DdNode** y;
//     DdNode* z[n_state_variables];
//     DdNode* xn[n_state_variables];
//     DdNode** yn;
//     DdNode* zn[n_state_variables];
//
//     int nx = 0;
//     int ny = 0;
//     int nz = 0;
//
//     int m = 0;
//     int n = 0;
//
//     //    y
//     // z []
//     DdNode* omega_add[k_j + 1];
//     for (int t = 0; t <= k_j; t++) {
//         matrixToADD(manager, omega[t], omega_add[t], z, y, zn, yn, &nz, &ny, &m, &n);
//     } 
//
//     //    y
//     // z []
//     DdNode* pi_add; // TODO: cache this if possible
//     matrixToADD(manager, pi, pi, z, y, zn, yn, &nz, &ny, &m, &n);
//
//     //    x
//     // z []
//     DdNode* P_add;
//     matrixToADD(manager, P, P, z, x, zn, xn, &nz, &nx, &m, &n);
//
//     //        y
//     // x | z []
//     DdNode** alpha_add = _forward(manager, omega_add, P_add, pi_add, z, nz, k_j);
//
//     double** alpha = malloc(sizeof(double) * (1 + k_j) * n_states);
//     for (int t = 0 ; t <= k_j; t++) {
//         alpha[t] = (double*) symbolic_to_numeric(alpha_add[t], n_states, 0);
//     }
//
//     return alpha;
// }

DdNode** _forwards(
    DdManager* manager,
    DdNode** omega, 
    DdNode* P, 
    DdNode* pi, 
    DdNode** row_vars,
    DdNode** column_vars,
    int n_vars,
    int k_j
) {
    DdNode** alpha = (DdNode**) malloc(sizeof(DdNode*) * (k_j + 1));
    alpha[0] = Cudd_addApply(manager, Cudd_addTimes, omega[0], pi);

    for (int t = 1; t <= k_j; t++) {
        DdNode* PT_alpha_t = Cudd_addMatrixMultiply(manager, P, alpha[t - 1], row_vars, n_vars);
        PT_alpha_t = Cudd_addSwapVariables(manager, PT_alpha_t, column_vars, row_vars, n_vars);
        alpha[t] = Cudd_addApply( manager, Cudd_addTimes, omega[t], PT_alpha_t);
    }

    return alpha;
}

DdNode** _backwards(
    DdManager* manager,
    DdNode** omega, 
    DdNode* P, 
    DdNode* pi, 
    DdNode** row_vars,
    DdNode** column_vars,
    int n_vars,
    int k_j
) {
    DdNode* _P = Cudd_addSwapVariables(manager, P, column_vars, row_vars, n_vars);

    DdNode** beta = (DdNode**) malloc(sizeof(DdNode*) * (k_j + 1));
    beta[k_j] = Cudd_addConst(manager, 1);

    for (int t = k_j - 1; 0 <= t; t--) {
        DdNode* beta_hadamard_omega_t 
            = Cudd_addApply(manager, Cudd_addTimes, beta[t + 1], omega[t + 1]);
        DdNode* beta_t 
            = Cudd_addMatrixMultiply(manager, _P, beta_hadamard_omega_t, row_vars, n_vars);
        beta[t] = Cudd_addSwapVariables(manager, beta_t, column_vars, row_vars, n_vars);
    }

    return beta;
}
