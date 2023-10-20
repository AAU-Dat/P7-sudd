#include "helper.h"
#include <cudd.h>

DdNode* matrix_2x2(DdManager* manager, double matrix[2][2], DdNode* row_var, DdNode* col_var) {
    return Cudd_addIte(
        manager,
        row_var,
        Cudd_addIte(
            manager,
            col_var,
            Cudd_addConst(manager, matrix[1][1]), 
            Cudd_addConst(manager, matrix[1][0])
        ), 
        Cudd_addIte(
            manager,
            col_var,
            Cudd_addConst(manager, matrix[0][1]), 
            Cudd_addConst(manager, matrix[0][0])
        )
    );
}

DdNode* vector_2x1(DdManager* manager, double vector[2], DdNode* row_var) {
    return Cudd_addIte(
        manager,
        row_var,
        Cudd_addConst(manager, vector[1]),
        Cudd_addConst(manager, vector[0])
    );
}
