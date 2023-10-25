#include "forward_wrapper.h"
// parameter \omega - matrix (den har længden k_j+1 x states)
// parameter P - (kvadratisk) matrix (den har længden states x states)
// parameter pi - søjle vektor (den har længden states x 1)

double **forward(
    double **omega,
    double **P,
    double **pi,
    int n_states,
    int k_j)
{
    int number_of_rows = n_states;
    int number_of_columns = n_states;
    int number_of_row_variables = 0;
    int number_of_column_variables = 0;
    int _1 = 1;

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

    matrixToADD(
        pi,
        gbm,
        &_pi,
        &row_variables,
        &column_variables,
        &complemented_row_variables,
        &complemented_column_variables,
        &number_of_row_variables,
        &number_of_column_variables,
        &number_of_rows,
        &_1);

    // make omega into an ADD for each column
    for (int t = 0; t <= k_j; t++)
    {
        matrixToADD(
            &omega[t],
            gbm,
            &_omega[t],
            &row_variables,
            &column_variables,
            &complemented_row_variables,
            &complemented_column_variables,
            &number_of_column_variables,
            &number_of_row_variables,
            &_1,
            &number_of_rows);
    }

    DdNode **_alpha = _forwards(
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
    FILE *outfile; // output file pointer for .dot file
    char filename[40];
    sprintf(filename, "matrixpi.dot", 0);
    outfile = fopen(filename, "w");
    DdNode **ddnodearray = (DdNode **)malloc(sizeof(DdNode *)); // initialize the function array
    ddnodearray[0] = _pi;
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
    free(ddnodearray);
    fclose(outfile); // close the file */
    {
    FILE *outfile; // output file pointer for .dot file
    char filename[40];
    sprintf(filename, "matrixp.dot", 0);
    outfile = fopen(filename, "w");
    DdNode **ddnodearray = (DdNode **)malloc(sizeof(DdNode *)); // initialize the function array
    ddnodearray[0] = _P;
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
    free(ddnodearray);
    fclose(outfile); // close the file */
    }

    for (int t = 0; t <= k_j; t++)
    {
        FILE *outfile; // output file pointer for .dot file
        char filename[40];
        sprintf(filename, "matrix%d.dot", t);
        outfile = fopen(filename, "w");
        DdNode **ddnodearray = (DdNode **)malloc(sizeof(DdNode *)); // initialize the function array
        ddnodearray[0] = _omega[t];
        Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
        free(ddnodearray);
        fclose(outfile); // close the file */
    }

    assert(Cudd_DebugCheck(gbm) == 0);

    Cudd_Quit(gbm);
    return alpha;
}