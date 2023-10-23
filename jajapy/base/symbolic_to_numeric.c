#include "symbolic_to_numeric.h"
#include <stdio.h>

CUDD_VALUE_TYPE** symbolic_to_numeric(DdNode* symbolic, int n_rows, int n_columns) {
    int n_row_variables = (int) ceil(log2(n_rows));
    int n_column_variables = (int) ceil(log2(n_columns));
    bool row_variables[n_row_variables];
    bool column_variables[n_column_variables];
    bool bits[n_row_variables + n_column_variables];
    memset(row_variables, 0, sizeof(row_variables));
    memset(column_variables, 0, sizeof(column_variables));
    CUDD_VALUE_TYPE** result = create_2d_array(n_rows, n_columns);

    DdNode* root = symbolic;
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_columns; j++) {
            interleave(row_variables, n_row_variables, column_variables, n_column_variables, bits);
            result[i][j] = evaluate_matrix_BDD(root, bits, 0);
            increment_bit_array(column_variables, n_column_variables);
        }
        increment_bit_array(row_variables, n_row_variables);
        memset(column_variables, 0, sizeof(column_variables));
    }

    return result;
}

CUDD_VALUE_TYPE** create_2d_array(int n_rows, int n_columns) {
    CUDD_VALUE_TYPE** array = (CUDD_VALUE_TYPE**)malloc(n_rows * sizeof(CUDD_VALUE_TYPE*));
    for (int i = 0; i < n_rows; i++) {
        array[i] = (CUDD_VALUE_TYPE*)malloc(n_columns * sizeof(CUDD_VALUE_TYPE));
    }
    return array;
}

CUDD_VALUE_TYPE evaluate_matrix_BDD(DdNode* node, bool bits[], int index) {
    if (Cudd_IsConstant(node)) {
        return Cudd_V(node);
    }
    if (bits[index]) {
        return evaluate_matrix_BDD(Cudd_T(node), bits, index + 1);
    } else {
        return evaluate_matrix_BDD(Cudd_E(node), bits, index + 1);
    }
}

void interleave(bool A[], int sizeA, bool B[], int sizeB, bool result[]) {
    int a_index = 0, b_index = 0, r_index = 0;

    while (a_index < sizeA && b_index < sizeB) {
        result[r_index++] = A[a_index++];
        result[r_index++] = B[b_index++];
    }

    while (a_index < sizeA) {
        result[r_index++] = A[a_index++];
    }

    while (b_index < sizeB) {
        result[r_index++] = B[b_index++];
    }
}

void increment_bit_array(bool array[], int array_size) {
    for (int i = array_size - 1; i >= 0; --i) {
        if (array[i] == false) {
            array[i] = true;
            break;
        } else {
            array[i] = false;
        }
    }
}

