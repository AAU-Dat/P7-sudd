#include "forward.h"
#include <cudd.h>
#include <stdlib.h>

// Convert gamma, P and pi to ADDs, and call _forward on those.
double** forward(double** gamma, double** P, double* pi) {
}

DdNode* add_square_transpose(DdManager* manager, DdNode* matrix, int n_rows) {
    int n_variables = 2 * (int) ceil(log2(n_rows));

    int* permutation = (int*) malloc(sizeof(int) * (n_variables));
    for (int i = 0; i < n_variables; i++) {
        permutation[i] = i;
    }

    for (int i = 0; i < n_variables; i += 2) {
        int temp = permutation[i];
        permutation[i] = permutation[i + 1];
        permutation[i + 1] = temp;
    }

    DdNode* matrix_T =  Cudd_addPermute(manager, matrix, permutation);
    free(permutation);
    return matrix_T;
}

DdNode** _forward(
    DdManager* manager,
    DdNode** gamma, 
    DdNode* P, 
    int n_states,
    DdNode* pi, 
    DdNode** z,
    int k_j
) {
    DdNode** alpha = (DdNode**) malloc(sizeof(DdNode*) * k_j);

    alpha[0] = Cudd_addApply(manager, Cudd_addTimes, gamma[0], pi);

    DdNode* P_T = add_square_transpose(manager, P, n_states);
    for (int t = 1; t <= k_j; t++) {
        alpha[t] = Cudd_addApply(
            manager, 
            Cudd_addTimes, 
            gamma[t], 
            Cudd_addMatrixMultiply(manager, P_T, alpha[t - 1], z, n_states)
        );
    }

    return alpha;
}

