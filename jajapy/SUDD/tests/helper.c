#include "helper.h"
#include <cudd.h>

DdNode* matrix_2x2(DdManager* manager, double matrix[2][2], DdNode* row_var, DdNode* col_var) {
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

    Cudd_RecursiveDeref(manager, _00);
    Cudd_RecursiveDeref(manager, _01);
    Cudd_RecursiveDeref(manager, _10);
    Cudd_RecursiveDeref(manager, _11);
    Cudd_RecursiveDeref(manager, _else);
    Cudd_RecursiveDeref(manager, _then);

    return final;
}

// This is terrible, too bad!
DdNode* matrix_3x3(DdManager* manager, double matrix[3][3], DdNode* row_vars[2], DdNode* col_vars[2]) {
    DdNode* _0000 = Cudd_addConst(manager, matrix[0][0]);
    Cudd_Ref(_0000);
    DdNode* _0001 = Cudd_addConst(manager, matrix[0][1]);
    Cudd_Ref(_0001);
    DdNode* _0010 = Cudd_addConst(manager, matrix[1][0]);
    Cudd_Ref(_0010);
    DdNode* _0011 = Cudd_addConst(manager, matrix[1][1]);
    Cudd_Ref(_0011);
    DdNode* _0100 = Cudd_addConst(manager, matrix[0][2]);
    Cudd_Ref(_0100);
    DdNode* _0101 = Cudd_addConst(manager, 0);
    Cudd_Ref(_0101);
    DdNode* _0110 = Cudd_addConst(manager, matrix[1][2]);
    Cudd_Ref(_0110);
    DdNode* _0111 = Cudd_addConst(manager, 0);
    Cudd_Ref(_0111);
    DdNode* _1000 = Cudd_addConst(manager, matrix[2][0]);
    Cudd_Ref(_1000);
    DdNode* _1001 = Cudd_addConst(manager, matrix[2][1]);
    Cudd_Ref(_1001);
    DdNode* _1010 = Cudd_addConst(manager, 0);
    Cudd_Ref(_1010);
    DdNode* _1011 = Cudd_addConst(manager, 0);
    Cudd_Ref(_1011);
    DdNode* _1100 = Cudd_addConst(manager, matrix[2][2]);
    Cudd_Ref(_1100);
    DdNode* _1101 = Cudd_addConst(manager, 0);
    Cudd_Ref(_1101);
    DdNode* _1110 = Cudd_addConst(manager, 0);
    Cudd_Ref(_1110);
    DdNode* _1111 = Cudd_addConst(manager, 0);
    Cudd_Ref(_1111);

    DdNode* _000 = Cudd_addIte(
        manager,
        col_vars[1],
        _0001,
        _0000
    );
    Cudd_Ref(_000);

    DdNode* _001 = Cudd_addIte(
        manager,
        col_vars[1],
        _0011,
        _0010
    );
    Cudd_Ref(_001);

    DdNode* _010 = Cudd_addIte(
        manager,
        col_vars[1],
        _0101,
        _0100
    );
    Cudd_Ref(_010);

    DdNode* _011 = Cudd_addIte(
        manager,
        col_vars[1],
        _0111,
        _0110
    );
    Cudd_Ref(_011);

    DdNode* _100 = Cudd_addIte(
        manager,
        col_vars[1],
        _1001,
        _1000
    );
    Cudd_Ref(_100);

    DdNode* _101 = Cudd_addIte(
        manager,
        col_vars[1],
        _1011,
        _1010
    );
    Cudd_Ref(_101);

    DdNode* _110 = Cudd_addIte(
        manager,
        col_vars[1],
        _1101,
        _1100
    );
    Cudd_Ref(_110);

    DdNode* _111 = Cudd_addIte(
        manager,
        col_vars[1],
        _1111,
        _1110
    );
    Cudd_Ref(_111);
    
    DdNode* _00 = Cudd_addIte(
        manager,
        row_vars[1],
        _001,
        _000
    );
    Cudd_Ref(_00);

    DdNode* _01 = Cudd_addIte(
        manager,
        row_vars[1],
        _011,
        _010
    );
    Cudd_Ref(_01);

    DdNode* _10 = Cudd_addIte(
        manager,
        row_vars[1],
        _101,
        _100
    );
    Cudd_Ref(_10);

    DdNode* _11 = Cudd_addIte(
        manager,
        row_vars[1],
        _110,
        _110
    );
    Cudd_Ref(_11);

    DdNode* _0 = Cudd_addIte(
        manager,
        col_vars[0],
        _01,
        _00
    );
    Cudd_Ref(_0);

    DdNode* _1 = Cudd_addIte(
        manager,
        col_vars[0],
        _11,
        _10
    );
    Cudd_Ref(_1);

    DdNode* root = Cudd_addIte(
        manager,
        row_vars[0],
        _1,
        _0
    );
    Cudd_Ref(root);

    Cudd_RecursiveDeref(manager, _0);
    Cudd_RecursiveDeref(manager, _1);

    Cudd_RecursiveDeref(manager, _00);
    Cudd_RecursiveDeref(manager, _01);
    Cudd_RecursiveDeref(manager, _10);
    Cudd_RecursiveDeref(manager, _11);


    Cudd_RecursiveDeref(manager, _000);
    Cudd_RecursiveDeref(manager, _001);
    Cudd_RecursiveDeref(manager, _010);
    Cudd_RecursiveDeref(manager, _011);
    Cudd_RecursiveDeref(manager, _100);
    Cudd_RecursiveDeref(manager, _101);
    Cudd_RecursiveDeref(manager, _110);
    Cudd_RecursiveDeref(manager, _111);

    Cudd_RecursiveDeref(manager, _0000);
    Cudd_RecursiveDeref(manager, _0001);
    Cudd_RecursiveDeref(manager, _0010);
    Cudd_RecursiveDeref(manager, _0011);
    Cudd_RecursiveDeref(manager, _0100);
    Cudd_RecursiveDeref(manager, _0101);
    Cudd_RecursiveDeref(manager, _0110);
    Cudd_RecursiveDeref(manager, _0111);
    Cudd_RecursiveDeref(manager, _1000);
    Cudd_RecursiveDeref(manager, _1001);
    Cudd_RecursiveDeref(manager, _1010);
    Cudd_RecursiveDeref(manager, _1011);
    Cudd_RecursiveDeref(manager, _1100);
    Cudd_RecursiveDeref(manager, _1101);
    Cudd_RecursiveDeref(manager, _1110);
    Cudd_RecursiveDeref(manager, _1111);

    return root;
}

DdNode* vector_2x1(DdManager* manager, double vector[2], DdNode* row_var) {
    DdNode* _0 = Cudd_addConst(manager, vector[0]);
    Cudd_Ref(_0);
    DdNode* _1 = Cudd_addConst(manager, vector[1]);
    Cudd_Ref(_1);

    DdNode* final =  Cudd_addIte(
        manager,
        row_var,
        _1,
        _0
    );
    Cudd_Ref(final);

    Cudd_RecursiveDeref(manager, _1);
    Cudd_RecursiveDeref(manager, _0);

    return final;
}

DdNode* vector_3x1(DdManager* manager, double vector[3], DdNode* row_vars[2]) {
    DdNode* _00 = Cudd_addConst(manager, vector[0]);
    Cudd_Ref(_00);
    DdNode* _01 = Cudd_addConst(manager, vector[1]);
    Cudd_Ref(_01);
    DdNode* _10 = Cudd_addConst(manager, vector[2]);
    Cudd_Ref(_10);
    DdNode* _11 = Cudd_addConst(manager, 0);
    Cudd_Ref(_11);

    DdNode* _0 = Cudd_addIte(
        manager,
        row_vars[1],
        _01,
        _00
    );
    Cudd_Ref(_0);

    DdNode* _1 = Cudd_addIte(
        manager,
        row_vars[1],
        _11,
        _10
    );
    Cudd_Ref(_1);


    DdNode* root = Cudd_addIte(
        manager,
        row_vars[0],
        _1,
        _0
    );
    Cudd_Ref(root);

    Cudd_RecursiveDeref(manager, _1);
    Cudd_RecursiveDeref(manager, _0);

    Cudd_RecursiveDeref(manager, _00);
    Cudd_RecursiveDeref(manager, _01);
    Cudd_RecursiveDeref(manager, _10);
    Cudd_RecursiveDeref(manager, _11);

    return root;
}

// Calculates alpha numerically
double** forwards_numeric(double** omega, 
                         double** P, 
                         double* pi,
                         int n_states,
                         int n_obs
) {
    // Allocate space for alpha
    double** alpha = (double**) malloc(sizeof(double*) * (n_obs+1));
    for(int s = 0; s < n_states+1; s++) {
        alpha[s] = (double*) malloc(sizeof(double) * (n_states+1));
    }

    // Base case: t = 0
    for (int s = 0; s < n_states; s++)
    {
        alpha[0][s] = pi[s];
    }
    
    // Case: 0 < t <= k_j  
    for (int t = 0; t < n_obs; t++)
    {
        for (int s = 0; s < n_states; s++)
        {
            double temp = 0;
            for (int ss = 0; ss < n_states; ss++)
            {
                temp += P[ss][s] * omega[t][ss] * alpha[t][ss];
                
            }
            alpha[t+1][s] = temp;            
        }
    }

    return alpha;
}

// Calculates beta numerically
double** backwards_numeric(double** omega, 
                         double** P, 
                         double* pi,
                         int n_states,
                         int n_obs
) { 
    // Allocate space for beta
    double** beta = (double**) malloc(sizeof(double**) * (n_obs + 1));
    for(int i = 0; i < n_states + 1; i++) {
        beta[i] = (double*) malloc(sizeof(double) * (n_states + 1));
    }

    // Base case: t = |o|
    *beta[n_obs] = 1;
    for (int s = 0; s < n_states; s++)
    {
        beta[n_obs][s] = 1;
    }

    // Case: 0 <= t < k_j
    for (int t = n_obs; 0 < t; t--)
    {
        for (int s = 0; s < n_states; s++)
        {
            double temp = 0;
            for (int ss = 0; ss < n_states; ss++)
            {
                temp += beta[t][ss] * P[s][ss];
            }
            beta[t-1][s] = omega[t-1][s] * temp;
        }
    }

    return beta;
}