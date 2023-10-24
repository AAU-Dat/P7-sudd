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
    Cudd_Ref(alpha[0]);

    for (int t = 1; t <= k_j; t++) {
        DdNode* PT_alpha_t = Cudd_addMatrixMultiply(manager, P, alpha[t - 1], row_vars, n_vars);
        Cudd_Ref(PT_alpha_t);
        PT_alpha_t = Cudd_addSwapVariables(manager, PT_alpha_t, column_vars, row_vars, n_vars);
        Cudd_Ref(PT_alpha_t);
        alpha[t] = Cudd_addApply(manager, Cudd_addTimes, omega[t], PT_alpha_t);
        Cudd_Ref(alpha[t]);
        Cudd_RecursiveDeref(manager, PT_alpha_t);
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
    Cudd_Ref(_P);

    DdNode** beta = (DdNode**) malloc(sizeof(DdNode*) * (k_j + 1));
    beta[k_j] = Cudd_addConst(manager, 1);
    Cudd_Ref(beta[k_j]);

    for (int t = k_j - 1; 0 <= t; t--) {
        DdNode* beta_hadamard_omega_t 
            = Cudd_addApply(manager, Cudd_addTimes, beta[t + 1], omega[t + 1]);
        Cudd_Ref(beta_hadamard_omega_t);
        DdNode* beta_t 
            = Cudd_addMatrixMultiply(manager, _P, beta_hadamard_omega_t, row_vars, n_vars);
        Cudd_Ref(beta_t);
        beta[t] = Cudd_addSwapVariables(manager, beta_t, column_vars, row_vars, n_vars);
        Cudd_Ref(beta[t]);
        Cudd_RecursiveDeref(manager, beta_hadamard_omega_t);
        Cudd_RecursiveDeref(manager, beta_t);
    }

    Cudd_RecursiveDeref(manager, _P);

    return beta;
}
