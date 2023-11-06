#include "forwards_backwards.h"
#include "conversion.h"
#include <cudd.h>
#include <stdio.h>
#include <stdlib.h>

double** forwards(
    double** omega,
    double** P,
    double* pi,
    int n_states,
    int n_obs
) {
    return fb(_forwards, omega, P, pi, n_states, n_obs);
}

double** backwards(
    double** omega,
    double** P,
    double* pi,
    int n_states,
    int n_obs
) {
    return fb(_backwards, omega, P, pi, n_states, n_obs);
}

double** file_forwards(
    char **omega,
    char *P,
    char *pi,
    int n_states,
    int n_obs
) {
    return file_fb(_forwards, omega, P, pi, n_states, n_obs);
}

double** file_backwards(
    char **omega,
    char *P,
    char *pi,
    int n_states,
    int n_obs
) {
    return file_fb(_backwards, omega, P, pi, n_states, n_obs);
}

double** fb(
    DdNode** (*_fb)(
        DdManager* manager,
        DdNode** omega,
        DdNode* P,
        DdNode* pi,
        DdNode** row_vars,
        DdNode** column_vars,
        int n_vars,
        int n_obs
    ),
    double** omega,
    double** P,
    double* pi,
    int n_states,
    int n_obs
) {
    int number_of_rows = n_states;
    int number_of_columns = n_states;
    int number_of_row_variables = 0;
    int number_of_column_variables = 0;

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode** row_variables = (DdNode**)malloc(ceil(log2(n_states)) * sizeof(DdNode*));
    DdNode** column_variables = (DdNode**)malloc(ceil(log2(n_states)) * sizeof(DdNode*));

    DdNode** complemented_row_variables = (DdNode**)malloc(ceil(log2(n_states)) * sizeof(DdNode*));
    DdNode** complemented_column_variables = (DdNode**)malloc(ceil(log2(n_states)) * sizeof(DdNode*));

    DdNode* _P;
    DdNode* _pi;
    DdNode** _omega = (DdNode**)malloc((n_obs) * sizeof(DdNode*));

    matrix_to_add(
        P,
        manager,
        &_P,
        &row_variables,
        &column_variables,
        &complemented_row_variables,
        &complemented_column_variables,
        &number_of_row_variables,
        &number_of_column_variables,
        &number_of_rows,
        &number_of_columns
    );

    vector_to_add(
        pi,
        manager,
        &_pi,
        &row_variables,
        &column_variables,
        &complemented_row_variables,
        &complemented_column_variables,
        &number_of_row_variables,
        &number_of_column_variables,
        &number_of_rows
    );

    // make omega into an ADD for each column
    for (int t = 0; t <= n_obs - 1; t++) {
        vector_to_add(
            omega[t],
            manager,
            &_omega[t],
            &row_variables,
            &column_variables,
            &complemented_row_variables,
            &complemented_column_variables,
            &number_of_column_variables,
            &number_of_row_variables,
            &number_of_rows
        );
    }

    DdNode** _alpha = _fb(
        manager,
        _omega,
        _P,
        _pi,
        row_variables,
        column_variables,
        number_of_column_variables,
        n_obs
    );

    CUDD_VALUE_TYPE** alpha = (CUDD_VALUE_TYPE**)malloc(sizeof(CUDD_VALUE_TYPE*) * (n_obs + 1));
    for (int t = 0; t <= n_obs; t++) {
        alpha[t] = add_to_vector(_alpha[t], number_of_rows, ROW_VAR_INDEX_OFFSET, ROW_VAR_INDEX_MULTIPLIER);
    }

    assert(Cudd_DebugCheck(manager) == 0);

    Cudd_Quit(manager);
    return alpha;
}

double** file_fb(
    DdNode** (*_fb)(
        DdManager* manager,
        DdNode** omega,
        DdNode* P,
        DdNode* pi,
        DdNode** row_vars,
        DdNode** column_vars,
        int n_vars,
        int n_obs
    ),
    char **omega,
    char *P,
    char *pi,
    int n_states,
    int n_obs
) {
    int number_of_rows = n_states;
    int number_of_columns = n_states;
    int number_of_row_variables = 0;
    int number_of_column_variables = 0;

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode** row_variables = (DdNode**)malloc(ceil(log2(n_states)) * sizeof(DdNode*));
    DdNode** column_variables = (DdNode**)malloc(ceil(log2(n_states)) * sizeof(DdNode*));

    DdNode** complemented_row_variables = (DdNode**)malloc(ceil(log2(n_states)) * sizeof(DdNode*));
    DdNode** complemented_column_variables = (DdNode**)malloc(ceil(log2(n_states)) * sizeof(DdNode*));

    DdNode* _P;
    DdNode* _pi;
    DdNode** _omega = (DdNode**)malloc((n_obs + 1) * sizeof(DdNode*));

    FILE* P_fp = fopen(P, "r");
    Cudd_addRead(
        P_fp,
        manager,
        &_P,
        &row_variables,
        &column_variables,
        &complemented_row_variables,
        &complemented_column_variables,
        &number_of_row_variables,
        &number_of_column_variables,
        &number_of_rows,
        &number_of_columns,
        ROW_VAR_INDEX_OFFSET,
        ROW_VAR_INDEX_MULTIPLIER,
        COL_VAR_INDEX_OFFSET,
        COL_VAR_INDEX_MULTIPLIER
    );
    fclose(P_fp);

    FILE* pi_fp = fopen(pi, "r");
    Cudd_addRead(
        pi_fp,
        manager,
        &_pi,
        &row_variables,
        &column_variables,
        &complemented_row_variables,
        &complemented_column_variables,
        &number_of_row_variables,
        &number_of_column_variables,
        &number_of_rows,
        &number_of_columns,
        ROW_VAR_INDEX_OFFSET,
        ROW_VAR_INDEX_MULTIPLIER,
        COL_VAR_INDEX_OFFSET,
        COL_VAR_INDEX_MULTIPLIER
    );
    fclose(pi_fp);

    FILE** omega_fps = (FILE**) malloc((n_obs) * sizeof(FILE*));
    for (int t = 0; t <= n_obs - 1; t++) {
        omega_fps[t] = fopen(omega[t], "r");
        Cudd_addRead(
            omega_fps[t],
            manager,
            &_omega[t],
            &row_variables,
            &column_variables,
            &complemented_row_variables,
            &complemented_column_variables,
            &number_of_row_variables,
            &number_of_column_variables,
            &number_of_rows,
            &number_of_columns,
            ROW_VAR_INDEX_OFFSET,
            ROW_VAR_INDEX_MULTIPLIER,
            COL_VAR_INDEX_OFFSET,
            COL_VAR_INDEX_MULTIPLIER
        );
        fclose(omega_fps[t]);
    }
    free(omega_fps);

    DdNode** _alpha = _fb(
        manager,
        _omega,
        _P,
        _pi,
        row_variables,
        column_variables,
        number_of_row_variables,
        n_obs
    );

    CUDD_VALUE_TYPE** alpha = (CUDD_VALUE_TYPE**)malloc(sizeof(CUDD_VALUE_TYPE*) * (n_obs + 1));
    for (int t = 0; t <= n_obs; t++) {
        alpha[t] = add_to_vector(_alpha[t], number_of_rows, ROW_VAR_INDEX_OFFSET, ROW_VAR_INDEX_MULTIPLIER);
    }

    assert(Cudd_DebugCheck(manager) == 0);

    Cudd_Quit(manager);
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
    int n_obs
) {
    DdNode** alpha = (DdNode**) malloc(sizeof(DdNode*) * (n_obs + 1));
    alpha[0] = Cudd_addApply(manager, Cudd_addTimes, omega[0], pi);
    Cudd_Ref(alpha[0]);

    for (int t = 1; t <= n_obs; t++) {
        DdNode* alpha_temp_0 = Cudd_addApply(manager, Cudd_addTimes, omega[t - 1], alpha[t - 1]);
        Cudd_Ref(alpha_temp_0);
        DdNode* alpha_temp_1 = Cudd_addMatrixMultiply(manager, P, alpha_temp_0, row_vars, n_vars);
        Cudd_Ref(alpha_temp_1);
        alpha[t] = Cudd_addSwapVariables(manager, alpha_temp_1, column_vars, row_vars, n_vars);
        Cudd_Ref(alpha[t]);
        Cudd_RecursiveDeref(manager, alpha_temp_0);
        Cudd_RecursiveDeref(manager, alpha_temp_1);
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
    int n_obs
) {
    DdNode* _P = Cudd_addSwapVariables(manager, P, column_vars, row_vars, n_vars);
    Cudd_Ref(_P);

    DdNode** beta = (DdNode**) malloc(sizeof(DdNode*) * (n_obs + 1));
    beta[n_obs] = Cudd_addConst(manager, 1);
    Cudd_Ref(beta[n_obs]);

    for (int t = n_obs - 1; 0 <= t; t--) {
        DdNode* beta_temp_0 = Cudd_addMatrixMultiply(manager, _P, beta[t + 1], row_vars, n_vars);
        Cudd_Ref(beta_temp_0);
        DdNode* beta_temp_1 = Cudd_addSwapVariables(manager, beta_temp_0, column_vars, row_vars, n_vars);
        Cudd_Ref(beta_temp_1);
        beta[t] = Cudd_addApply(manager, Cudd_addTimes, omega[t], beta_temp_1);
        Cudd_Ref(beta[t]);
        Cudd_RecursiveDeref(manager, beta_temp_0);
        Cudd_RecursiveDeref(manager, beta_temp_1);
    }

    Cudd_RecursiveDeref(manager, _P);

    return beta;
}

