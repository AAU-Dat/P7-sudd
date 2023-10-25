#include "forward_wrapper.h"
// parameter \omega - matrix (den har længden k_j+1 x states)
// parameter P - (kvadratisk) matrix (den har længden states x states)
// parameter pi - søjle vektor (den har længden states x 1)

double **forward(
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