#include "forwards_backwards.h"
#include "conversion.h"
#include <cudd.h>
#include <stdlib.h>

double **fb(
    DdNode **(*_fb)(
        DdManager *manager,
        DdNode **omega,
        DdNode *P,
        DdNode *pi,
        DdNode **row_vars,
        DdNode **column_vars,
        int n_vars,
        int k_j),
    double **omega,
    double **P,
    double *pi,
    int n_states,
    int k_j)
{
    int number_of_rows = n_states;
    int number_of_columns = n_states;
    int number_of_row_variables = 0;
    int number_of_column_variables = 0;

    DdManager *gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode **row_variables = (DdNode **)malloc(ceil(log2(n_states)) * sizeof(DdNode *));
    DdNode **column_variables = (DdNode **)malloc(ceil(log2(n_states)) * sizeof(DdNode *));

    DdNode **complemented_row_variables = (DdNode **)malloc(ceil(log2(n_states)) * sizeof(DdNode *));
    DdNode **complemented_column_variables = (DdNode **)malloc(ceil(log2(n_states)) * sizeof(DdNode *));

    DdNode *_P;
    DdNode *_pi;
    DdNode **_omega = (DdNode **)malloc((k_j + 1) * sizeof(DdNode *));

    matrixToADD(
        P,
        gbm,
        &_P,
        &row_variables,
        &column_variables,
        &complemented_row_variables,
        &complemented_column_variables,
        &number_of_row_variables,
        &number_of_column_variables,
        &number_of_rows,
        &number_of_columns);

    vectorToADD(
        pi,
        gbm,
        &_pi,
        &row_variables,
        &column_variables,
        &complemented_row_variables,
        &complemented_column_variables,
        &number_of_row_variables,
        &number_of_column_variables,
        &number_of_rows);

    // make omega into an ADD for each column
    for (int t = 0; t <= k_j; t++)
    {
        vectorToADD(
            omega[t],
            gbm,
            &_omega[t],
            &row_variables,
            &column_variables,
            &complemented_row_variables,
            &complemented_column_variables,
            &number_of_column_variables,
            &number_of_row_variables,
            &number_of_rows);
    }

    DdNode **_alpha = _fb(
        gbm,
        _omega,
        _P,
        _pi,
        row_variables,
        column_variables,
        number_of_column_variables,
        k_j);

    CUDD_VALUE_TYPE **alpha = (CUDD_VALUE_TYPE **)malloc(sizeof(CUDD_VALUE_TYPE *) * (k_j + 1));
    for (int t = 0; t <= k_j; t++)
    {
        alpha[t] = symbolic_to_numeric(_alpha[t], 1, number_of_rows)[0];
    }

    assert(Cudd_DebugCheck(gbm) == 0);

    Cudd_Quit(gbm);
    return alpha;
}

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

