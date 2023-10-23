#include "helper.h"
#include <cudd.h>

DdNode* matrix_2x2(DdManager* manager, double matrix[2][2]) {
    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    DdNode* _00 = Cudd_addConst(manager, matrix[0][0]);
    Cudd_Ref(_00);
    DdNode* _01 = Cudd_addConst(manager, matrix[0][1]);
    Cudd_Ref(_01);
    DdNode* _10 = Cudd_addConst(manager, matrix[1][0]);
    Cudd_Ref(_10);
    DdNode* _11 = Cudd_addConst(manager, matrix[1][1]);
    Cudd_Ref(_11);

    DdNode* _then = Cudd_addIte(
        manager,
        col_var,
        _11,
        _10
    );
    Cudd_Ref(_then);

    DdNode* _else = Cudd_addIte(
        manager,
        col_var,
        _01,
        _00
    );
    Cudd_Ref(_else);

    DdNode* final = Cudd_addIte(
        manager,
        row_var,
        _then,
        _else
    );
    Cudd_Ref(final);

    Cudd_RecursiveDeref(manager, row_var);
    Cudd_RecursiveDeref(manager, col_var);
    Cudd_RecursiveDeref(manager, _00);
    Cudd_RecursiveDeref(manager, _01);
    Cudd_RecursiveDeref(manager, _10);
    Cudd_RecursiveDeref(manager, _11);
    Cudd_RecursiveDeref(manager, _else);
    Cudd_RecursiveDeref(manager, _then);

    return final;
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
