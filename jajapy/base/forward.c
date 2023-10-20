#include "forward.h"
#include "symbolic_to_numeric.h"
#include <cudd.h>
#include <stdlib.h>

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
