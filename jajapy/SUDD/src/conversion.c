#include "conversion.h"
#include <cudd.h>

CUDD_VALUE_TYPE* add_to_vector(DdNode* add, int n, int var_index_offset, int var_index_multiplier) {
    int n_variables = (int) ceil(log2(n));
    bool variables[n_variables];

    memset(variables, 0, sizeof(variables));
    CUDD_VALUE_TYPE* result = new_array(n);

    for (int i = 0; i < n; i++) {
        result[i] = evaluate_vector_bdd(add, variables, var_index_offset, var_index_multiplier);
        increment_bit_array(variables, n_variables);
    }

    return result;
}

CUDD_VALUE_TYPE** add_to_matrix(DdNode* symbolic, int n_rows, int n_columns) {
    int n_row_variables = (int) ceil(log2(n_rows));
    int n_column_variables = (int) ceil(log2(n_columns));
    bool row_variables[n_row_variables];
    bool column_variables[n_column_variables];

    memset(row_variables, 0, sizeof(row_variables));
    memset(column_variables, 0, sizeof(column_variables));
    CUDD_VALUE_TYPE** result = create_2d_array(n_rows, n_columns);

    DdNode* root = symbolic;
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_columns; j++) {
            result[i][j] = evaluate_matrix_bdd(root, row_variables, column_variables);
            increment_bit_array(column_variables, n_column_variables);
        }
        increment_bit_array(row_variables, n_row_variables);
        memset(column_variables, 0, sizeof(column_variables));
    }

    return result;
}

void add_to_vector_in_place(
    DdNode* add,
    int n,
    int var_index_offset,
    int var_index_multiplier,
    CUDD_VALUE_TYPE* vector
) {
    int n_variables = (int) ceil(log2(n));
    bool variables[n_variables];

    memset(variables, 0, sizeof(variables));

    for (int i = 0; i < n; i++) {
        vector[i] = evaluate_vector_bdd(add, variables, var_index_offset, var_index_multiplier);
        increment_bit_array(variables, n_variables);
    }
}

CUDD_VALUE_TYPE evaluate_vector_bdd(DdNode* node, bool vars[], int var_index_offset, int var_index_multiplier) {
    if (Cudd_IsConstant(node)) {
        return Cudd_V(node);
    }
    unsigned int node_index = Cudd_NodeReadIndex(node);
    int var_index = (node_index - var_index_offset) / var_index_multiplier;
    if (vars[var_index]) {
        return evaluate_vector_bdd(Cudd_T(node), vars, var_index_offset, var_index_multiplier);
    } else {
        return evaluate_vector_bdd(Cudd_E(node), vars, var_index_offset, var_index_multiplier);
    }
}

CUDD_VALUE_TYPE evaluate_matrix_bdd(DdNode* node, bool row_bits[], bool col_bits[]) {
    if (Cudd_IsConstant(node)) {
        return Cudd_V(node);
    }
    
    // TODO STOP HARDCODING VARIABLE INDEXES
    // TODO this is probably not the best way to select between row vars and col vars
    unsigned int node_index = Cudd_NodeReadIndex(node);
    bool is_row_var = node_index % (ROW_VAR_INDEX_OFFSET + ROW_VAR_INDEX_MULTIPLIER) == 0;
    
    if (is_row_var) {
        int var_index = (node_index - ROW_VAR_INDEX_OFFSET) / ROW_VAR_INDEX_MULTIPLIER;
        if (row_bits[var_index]) {
            return evaluate_matrix_bdd(Cudd_T(node), row_bits, col_bits);
        } else {
            return evaluate_matrix_bdd(Cudd_E(node), row_bits, col_bits);
        }
    } else {
        int var_index = (node_index - COL_VAR_INDEX_OFFSET) / COL_VAR_INDEX_MULTIPLIER;
        if (col_bits[var_index]) {
            return evaluate_matrix_bdd(Cudd_T(node), row_bits, col_bits);
        } else {
            return evaluate_matrix_bdd(Cudd_E(node), row_bits, col_bits);
        }
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

CUDD_VALUE_TYPE* new_array(int size) {
    CUDD_VALUE_TYPE* array = (CUDD_VALUE_TYPE*) malloc(size * sizeof(CUDD_VALUE_TYPE));
    if (array == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return array;
}

CUDD_VALUE_TYPE** create_2d_array(int n_rows, int n_columns) {
    CUDD_VALUE_TYPE** array = (CUDD_VALUE_TYPE**)malloc(n_rows * sizeof(CUDD_VALUE_TYPE*));
    for (int i = 0; i < n_rows; i++) {
        array[i] = (CUDD_VALUE_TYPE*)malloc(n_columns * sizeof(CUDD_VALUE_TYPE));
    }
    return array;
}
