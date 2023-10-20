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

DdNode* matrix_3x3(DdManager* manager, double matrix[3][3], DdNode* row_vars[2], DdNode* col_vars[2]) {
    return Cudd_addIte(
        manager,
        row_vars[0],
        Cudd_addIte(
            manager,
            col_vars[0],
            Cudd_addIte(
                manager,
                row_vars[1],
                Cudd_addIte(
                    manager,
                    col_vars[1],
                    Cudd_addConst(manager, 0),
                    Cudd_addConst(manager, 0)
                ),
                Cudd_addIte(
                    manager,
                    col_vars[1],
                    Cudd_addConst(manager, 0),
                    Cudd_addConst(manager, matrix[2][2])
                )
            ),
            Cudd_addIte(
                manager,
                row_vars[1],
                Cudd_addIte(
                    manager,
                    col_vars[1],
                    Cudd_addConst(manager, 0),
                    Cudd_addConst(manager, 0)

                ),
                Cudd_addIte(
                    manager,
                    col_vars[1],
                    Cudd_addConst(manager, matrix[2][1]),
                    Cudd_addConst(manager, matrix[2][0])
                )
            )
        ),
        Cudd_addIte(
            manager,
            col_vars[0],
            Cudd_addIte(
                manager,
                row_vars[1],
                Cudd_addIte(
                    manager,
                    col_vars[1],
                    Cudd_addConst(manager, 0),
                    Cudd_addConst(manager, matrix[1][2])
                ),
                Cudd_addIte(
                    manager,
                    col_vars[1],
                    Cudd_addConst(manager, 0),
                    Cudd_addConst(manager, matrix[0][2])
                )
            ),
            Cudd_addIte(
                manager,
                row_vars[1],
                Cudd_addIte(
                    manager,
                    col_vars[1],
                    Cudd_addConst(manager, matrix[1][1]),
                    Cudd_addConst(manager, matrix[1][0])
                ),
                Cudd_addIte(
                    manager,
                    col_vars[1],
                    Cudd_addConst(manager, matrix[0][1]),
                    Cudd_addConst(manager, matrix[0][0])
                )
            )
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

DdNode* vector_3x1(DdManager* manager, double vector[3], DdNode* row_vars[2]) {
    return Cudd_addIte(
        manager,
        row_vars[0],
        Cudd_addIte(
            manager,
            row_vars[1],
            Cudd_addConst(manager, 0),
            Cudd_addConst(manager, vector[2])
        ),
        Cudd_addIte(
            manager,
            row_vars[1],
            Cudd_addConst(manager, vector[1]),
            Cudd_addConst(manager, vector[0])
        )
    );
}
