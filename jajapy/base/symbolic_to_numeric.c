#include "symbolic_to_numeric.h"
// #define SET_BIT(array, pos)   (array[(pos) / 8] |= (1 << ((pos) % 8)))
// #define CLEAR_BIT(array, pos) (array[(pos) / 8] &= ~(1 << ((pos) % 8)))
// #define GET_BIT(array, pos)   ((array[(pos) / 8] & (1 << ((pos) % 8))) != 0)

CUDD_VALUE_TYPE** symbolic_to_numeric(DdNode* symbolic, int n_rows, int n_columns) {
    unsigned int n_row_variables = (int) ceil(log2(n_rows));
    unsigned int n_column_variables = (int) ceil(log2(n_columns));
    bool row_variables[n_row_variables];
    bool column_variables[n_column_variables];
    memset(row_variables, 0, sizeof(row_variables));
    memset(column_variables, 0, sizeof(column_variables));
    CUDD_VALUE_TYPE** result = create_2d_array(n_rows, n_columns);

    DdNode* root = symbolic;
    for (int i = 0; i < n_rows; i++) {
        memset(column_variables, 0, sizeof(column_variables));
        for (int j = 0; j < n_columns; j++) {
            result[i][j] = evaluate_matrix_BDD(root, row_variables, n_row_variables, column_variables, n_column_variables, 0);
            increment_bit_array(column_variables, n_column_variables);
        }
        increment_bit_array(row_variables, n_row_variables);
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

CUDD_VALUE_TYPE evaluate_matrix_BDD(DdNode* node, bool row_variables[], int n_row_variables, bool column_variables[], int n_column_variables, int index) {
    bool bits[n_row_variables + n_column_variables];
    interleave(row_variables, n_row_variables, column_variables, n_column_variables, bits);
    if (Cudd_IsConstant(node)) {
        return Cudd_V(node);
    }
    if (bits[index]) {
        return evaluate_matrix_BDD(Cudd_T(node), row_variables, n_row_variables, column_variables, n_column_variables, index + 1);
    } else {
        return evaluate_matrix_BDD(Cudd_E(node), row_variables, n_row_variables, column_variables, n_column_variables, index + 1);
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

// void interleave( A[], unsigned char B[], unsigned char result[], int size) {
//     int indexA, indexB, indexR;
//     unsigned char maskA, maskB;
//
//     for (int i = 0; i < size * 8; i++) {
//         indexA = i / 8;
//         indexB = i / 8;
//         indexR = i / 4;
//
//         maskA = 1 << (7 - (i % 8));
//         maskB = 1 << (7 - (i % 8));
//
//         if (A[indexA] & maskA) {
//             result[indexR] |= (1 << (7 - (2 * (i % 8))));
//         }
//
//         if (B[indexB] & maskB) {
//             result[indexR] |= (1 << (6 - (2 * (i % 8))));
//         }
//     }
// }

// CUDD_VALUE_TYPE evaluate_matrix_BDD(DdNode* node, unsigned char row_variables[], unsigned char column_variables[], size_t total_size) {
//     unsigned char bits[total_size];
//     interleave(row_variables, column_variables, bits, total_size);
//     if (Cudd_IsConstant(node)) {
//         return Cudd_V(node);
//     }
//     int index = Cudd_NodeReadIndex(node);
//     if (GET_BIT(bits, index)) {
//         return evaluate_matrix_BDD(Cudd_T(node), row_variables, column_variables, total_size);
//     } else {
//         return evaluate_matrix_BDD(Cudd_E(node), row_variables, column_variables, total_size);
//     }
// }

// void increment_bit_array(unsigned char *array, size_t array_size) {
//     int total_bits = array_size * 8;
//
//     for (int i = total_bits - 1; i >= 0; i--) {
//         if (GET_BIT(array, i) == 0) {
//             SET_BIT(array, i);
//             return;
//         } else {
//             CLEAR_BIT(array, i);  // Set the bit to 0 and carry to the next bit
//         }
//     }
// }

