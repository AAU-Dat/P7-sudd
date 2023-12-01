#include "forwards_backwards.h"

int forwards(
    CUDD_VALUE_TYPE* omega,
    CUDD_VALUE_TYPE* P,
    CUDD_VALUE_TYPE* pi,
    ssize_t n_states,
    ssize_t n_obs,
    CUDD_VALUE_TYPE* alpha
) {
    return fb(_forwards, omega, P, pi, n_states, n_obs, alpha);
}

int backwards(
    CUDD_VALUE_TYPE* omega,
    CUDD_VALUE_TYPE* P,
    CUDD_VALUE_TYPE* pi,
    ssize_t n_states,
    ssize_t n_obs,
    CUDD_VALUE_TYPE*beta
) {
    return fb(_backwards, omega, P, pi, n_states, n_obs, beta);
}

int log_forwards(
    CUDD_VALUE_TYPE* omega,
    CUDD_VALUE_TYPE* P,
    CUDD_VALUE_TYPE* pi,
    ssize_t n_states,
    ssize_t n_obs,
    CUDD_VALUE_TYPE* alpha
) {
    return fb(_log_forwards, omega, P, pi, n_states, n_obs, alpha);
}


int log_backwards(
    CUDD_VALUE_TYPE* omega,
    CUDD_VALUE_TYPE* P,
    CUDD_VALUE_TYPE* pi,
    ssize_t n_states,
    ssize_t n_obs,
    CUDD_VALUE_TYPE* beta
) {
    return fb(_log_backwards, omega, P, pi, n_states, n_obs, alpha);
}

int fb(
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
    CUDD_VALUE_TYPE* omega,
    CUDD_VALUE_TYPE* P,
    CUDD_VALUE_TYPE* pi,
    ssize_t n_states,
    ssize_t n_obs,
    CUDD_VALUE_TYPE* ab // output variable
) {
    DdManager* dd = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    Cudd_SetEpsilon(dd, 0);

    int dump_n_rows = n_states;
    int dump_n_cols = n_states;
    int n_row_vars = 0;
    int n_col_vars = 0;

    int n_vars = ceil(log2(n_states));
    DdNode** row_vars = safe_malloc(sizeof(DdNode*), n_vars);
    DdNode** col_vars = safe_malloc(sizeof(DdNode*), n_vars);
    DdNode** comp_row_vars = safe_malloc(sizeof(DdNode*), n_vars);
    DdNode** comp_col_vars = safe_malloc(sizeof(DdNode*), n_vars);

    DdNode* _P;
    DdNode* _pi;
    DdNode** _omega = safe_malloc(sizeof(DdNode*), n_obs);

    Sudd_addRead(
        P,
        n_states,
        n_states,
        dd,
        &_P,
        &row_vars,
        &col_vars,
        &comp_row_vars,
        &comp_col_vars,
        &n_row_vars,
        &n_col_vars,
        &dump_n_rows,
        &dump_n_cols,
        ROW_VAR_INDEX_OFFSET,
        ROW_VAR_INDEX_MULTIPLIER,
        COL_VAR_INDEX_OFFSET,
        COL_VAR_INDEX_MULTIPLIER
    );

    Sudd_addRead(
        pi,
        n_states,
        1,
        dd,
        &_pi,
        &row_vars,
        &col_vars,
        &comp_row_vars,
        &comp_col_vars,
        &n_row_vars,
        &n_col_vars,
        &dump_n_rows,
        &dump_n_cols,
        ROW_VAR_INDEX_OFFSET,
        ROW_VAR_INDEX_MULTIPLIER,
        COL_VAR_INDEX_OFFSET,
        COL_VAR_INDEX_MULTIPLIER
    );

    for (ssize_t t = 0; t < n_obs; t++) {
        Sudd_addRead(
            &omega[t * n_states],
            n_states,
            1,
            dd,
            &_omega[t],
            &row_vars,
            &col_vars,
            &comp_row_vars,
            &comp_col_vars,
            &n_row_vars,
            &n_col_vars,
            &dump_n_rows,
            &dump_n_cols,
            ROW_VAR_INDEX_OFFSET,
            ROW_VAR_INDEX_MULTIPLIER,
            COL_VAR_INDEX_OFFSET,
            COL_VAR_INDEX_MULTIPLIER
        );
    }

    DdNode** _ab = _fb(
        dd,
        _omega,
        _P,
        _pi,
        row_vars,
        col_vars,
        n_row_vars,
        n_obs
    );

    for (size_t t = 0; t <= n_obs; t++) {
        add_to_vector_in_place(
            _ab[t],
            n_states,
            ROW_VAR_INDEX_OFFSET,
            ROW_VAR_INDEX_MULTIPLIER,
            &ab[t * n_states]
        );
    }

    Cudd_Quit(dd);
    return 0;
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
    DdNode** alpha = safe_malloc(sizeof(DdNode*), n_obs + 1);
    alpha[0] = pi;

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

    DdNode** beta = safe_malloc(sizeof(DdNode*), n_obs + 1);
    beta[n_obs] = Cudd_ReadOne(manager);

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

// Calculates alpha numerically but using numpy arrays
int forwards_numeric_c(double* omega, 
                         double* P, 
                         double* pi,
                         int n_states,
                         int n_obs,
                         double* alpha
) {

    // Base case: t = 0
    for (int s = 0; s < n_states; s++)
    {
        alpha[0 * n_states + s] = pi[s];
    }
    
    // Case: 0 < t <= k_j  
    for (int t = 0; t < n_obs; t++)
    {
        for (int s = 0; s < n_states; s++)
        {
            double temp = 0;
            for (int ss = 0; ss < n_states; ss++)
            {
                temp += P[ss * n_states + s] * omega[t * n_states + ss] * alpha[t * n_states + ss];
                
            }
            alpha[(t+1) * n_states + s] = temp;            
        }
    }

    return 0;
}

// Calculates beta numerically but using numpy arrays
int backwards_numeric_c(double* omega, 
                         double* P, 
                         double* pi,
                         int n_states,
                         int n_obs,
                         double* beta
) { 
    // Base case: t = |o|
    for (int s = 0; s < n_states; s++)
    {
        beta[n_obs * n_states + s] = 1;
    }

    // Case: 0 <= t < k_j
    for (int t = n_obs; 0 < t; t--)
    {
        for (int s = 0; s < n_states; s++)
        {
            double temp = 0;
            for (int ss = 0; ss < n_states; ss++)
            {
                temp += beta[t * n_states + ss] * P[s * n_states + ss];
            }
            beta[(t-1) * n_states + s] = omega[(t-1) * n_states + s] * temp;
        }
    }

    return 0;
}
DdNode** _log_forwards(
    DdManager* dd,
    DdNode** omega,
    DdNode* P,
    DdNode* pi,
    DdNode** row_vars,
    DdNode** column_vars,
    int n_vars,
    int n_obs
) {
    DdNode* log_P = Cudd_addMonadicApply(dd, Cudd_addLog, P);
    Cudd_Ref(log_P);
    DdNode* log_pi = Cudd_addMonadicApply(dd, Cudd_addLog, pi);
    Cudd_Ref(log_pi);
    DdNode** log_omega = safe_malloc(sizeof(DdNode*), n_obs);
    for (size_t t = 0; t < n_obs; t++) {
        log_omega[t] = Cudd_addMonadicApply(dd, Cudd_addLog, omega[t]);
        Cudd_Ref(log_omega[t]);
    }

    DdNode** log_alpha = safe_malloc(sizeof(DdNode*), n_obs + 1);
    log_alpha[0] = log_pi;
    for (size_t t = 1; t < n_obs + 1; t++) {
        DdNode* log_alpha_temp_0 = Cudd_addApply(dd, Cudd_addPlus, log_omega[t - 1], log_alpha[t - 1]);
        Cudd_Ref(log_alpha_temp_0);
        DdNode* log_alpha_temp_1 = Cudd_addLogMatrixMultiply(dd, log_P, log_alpha_temp_0, row_vars, n_vars);
        Cudd_Ref(log_alpha_temp_1);
        log_alpha[t] = Cudd_addSwapVariables(dd, log_alpha_temp_1, column_vars, row_vars, n_vars);
        Cudd_Ref(log_alpha[t]);
        Cudd_RecursiveDeref(dd, log_alpha_temp_0);
        Cudd_RecursiveDeref(dd, log_alpha_temp_1);
    }

    DdNode** alpha = safe_malloc(sizeof(DdNode*), n_obs + 1);
    for (size_t t = 0; t < n_obs + 1; t++) {
        alpha[t] = Cudd_addMonadicApply(dd, Cudd_addExp, log_alpha[t]);
        Cudd_Ref(alpha[t]);
        Cudd_RecursiveDeref(dd, log_alpha[t]);
    }
    free(log_alpha);

    Cudd_RecursiveDeref(dd, log_P);
    Cudd_RecursiveDeref(dd, log_pi);
    for (size_t t = 0; t < n_obs; t++) {
        Cudd_RecursiveDeref(dd, log_omega[t]);
    }
    free(log_omega);

    return alpha;
}

DdNode** _log_backwards(
    DdManager* dd,
    DdNode** omega,
    DdNode* P,
    DdNode* pi,
    DdNode** row_vars,
    DdNode** column_vars,
    int n_vars,
    int n_obs
) {
    DdNode* _P = Cudd_addSwapVariables(dd, P, column_vars, row_vars, n_vars);
    Cudd_Ref(_P);
    DdNode* log_P = Cudd_addMonadicApply(dd, Cudd_addLog, _P);
    Cudd_Ref(log_P);
    Cudd_RecursiveDeref(dd, _P);
    DdNode** log_omega = safe_malloc(sizeof(DdNode*), n_obs);
    for (size_t t = 0; t < n_obs; t++) {
        log_omega[t] = Cudd_addMonadicApply(dd, Cudd_addLog, omega[t]);
        Cudd_Ref(log_omega[t]);
    }

    DdNode** log_beta = safe_malloc(sizeof(DdNode*), n_obs + 1);
    log_beta[n_obs] = Cudd_ReadZero(dd);
    for (ssize_t t = n_obs - 1; 0 <= t; t--) {
        DdNode* log_beta_temp_0 = Cudd_addLogMatrixMultiply(dd, log_P, log_beta[t + 1], row_vars, n_vars);
        Cudd_Ref(log_beta_temp_0);
        DdNode* log_beta_temp_1 = Cudd_addSwapVariables(dd, log_beta_temp_0, column_vars, row_vars, n_vars);
        Cudd_Ref(log_beta_temp_1);
        log_beta[t] = Cudd_addApply(dd, Cudd_addPlus, log_omega[t], log_beta_temp_1);
        Cudd_Ref(log_beta[t]);
        Cudd_RecursiveDeref(dd, log_beta_temp_0);
        Cudd_RecursiveDeref(dd, log_beta_temp_1);
    }

    DdNode** beta = safe_malloc(sizeof(DdNode*), n_obs + 1);
    for (size_t t = 0; t < n_obs + 1; t++) {
        beta[t] = Cudd_addMonadicApply(dd, Cudd_addExp, log_beta[t]);
        Cudd_Ref(beta[t]);
        Cudd_RecursiveDeref(dd, log_beta[t]);
    }
    free(log_beta);

    Cudd_RecursiveDeref(dd, log_P);
    for (size_t t = 0; t < n_obs; t++) {
        Cudd_RecursiveDeref(dd, log_omega[t]);
    }
    free(log_omega);

    return beta;
}

void *safe_malloc(size_t type_size, size_t amount) {
    void *ptr = malloc(type_size * amount);
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
