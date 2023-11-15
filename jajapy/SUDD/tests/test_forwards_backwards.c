#include "test_forwards_backwards.h"

START_TEST(_forwards_2x2_2_n_obs)
{
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int n_obs = 2;
    int n_vars = 1;
    int n_states = 2;

    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    double P[2][2] = {
        {1, 2},
        {3, 4}};
    DdNode* _P = matrix_2x2(manager, P, row_var, col_var);

    double omega0[2] = {1, 2};
    DdNode* _omega0 = vector_2x1(manager, omega0, row_var);

    double omega1[2] = {3, 4};
    DdNode* _omega1 = vector_2x1(manager, omega1, row_var);

    DdNode* _omega[2] = {_omega0, _omega1};

    double pi[2] = {1, 2};
    DdNode* _pi = vector_2x1(manager, pi, row_var);

    // make matrix for omega that has the size k_j+1 x states
    double** omeganum = (double**)malloc((n_obs + 2) * sizeof(double*));
    for (int i = 0; i <= n_obs + 1; i++)
    {
        omeganum[i] = (double*)malloc(n_states * sizeof(double));
    }
    omeganum[0][0] = 1;
    omeganum[0][1] = 2;
    omeganum[1][0] = 3;
    omeganum[1][1] = 4;

    // make matrix for P that has the size states x states
    double** Pnum = (double**)malloc(n_states * sizeof(double*));
    double* pinum = (double*)malloc(n_states * sizeof(double));
    for (int i = 0; i < n_states; i++)
    {
        Pnum[i] = (double*)malloc(n_states * sizeof(double));
    }
    Pnum[0][0] = 1;
    Pnum[0][1] = 2;
    Pnum[1][0] = 3;
    Pnum[1][1] = 4;

    pinum[0] = 1;
    pinum[1] = 2;

    double** alpha = forwards_numeric(omeganum, Pnum, pinum, n_states, n_obs);

    // Act
    DdNode** _alpha = _forwards(manager, _omega, _P, _pi, &row_var, &col_var, n_vars, n_obs);
    double** alpha0 = add_to_matrix(_alpha[0], 2, 1);
    double** alpha1 = add_to_matrix(_alpha[1], 2, 1);
    double** alpha2 = add_to_matrix(_alpha[2], 2, 1);

    // Assert
    for (int s = 0; s < n_states; s++)
    {
        ck_assert_double_eq(alpha[0][s], alpha0[s][0]);
        ck_assert_double_eq(alpha[1][s], alpha1[s][0]);
        ck_assert_double_eq(alpha[2][s], alpha2[s][0]);
    }
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Clean
    for (int i = 0; i < n_obs + 2; i++)
    {
        free(omeganum[i]);
    }
    
    for (int i = 0; i < n_states; i++)
    {
        free(Pnum[i]);
    }
    
    for (int i = 0; i < n_states; i++)
    {
        free(alpha[i]);
    }

    free(omeganum);
    free(Pnum);
    free(pinum);
    free(alpha);


    Cudd_Quit(manager);
}
END_TEST

START_TEST(_backwards_2x2_2_n_obs)
{
    // Arrange
    // Arrange: CUDD
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    int n_obs = 2;
    int n_vars = 1;
    int n_states = 2;

    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    double P[2][2] = {
        {1, 2},
        {3, 4}};
    DdNode* _P = matrix_2x2(manager, P, row_var, col_var);

    double omega0[2] = {1, 2};
    DdNode* _omega0 = vector_2x1(manager, omega0, row_var);

    double omega1[2] = {3, 4};
    DdNode* _omega1 = vector_2x1(manager, omega1, row_var);

    DdNode* _omega[2] = {_omega0, _omega1};

    double pi[2] = {1, 2};
    DdNode* _pi = vector_2x1(manager, pi, row_var);

    // Arrange: Numeric
    double** omeganum = (double**)malloc((n_obs + 2) * sizeof(double*));
    for (int i = 0; i <= n_obs + 1; i++)
    {
        omeganum[i] = (double*)malloc(n_states * sizeof(double));
    }
    omeganum[0][0] = 1;
    omeganum[0][1] = 2;
    omeganum[1][0] = 3;
    omeganum[1][1] = 4;

    // Make matrix for P that has the size states x states
    double** Pnum = (double**)malloc(n_states * sizeof(double*));
    double* pinum = (double*)malloc(n_states * sizeof(double));
    for (int i = 0; i < n_states; i++)
    {
        Pnum[i] = (double*)malloc(n_states * sizeof(double));
    }
    Pnum[0][0] = 1;
    Pnum[0][1] = 2;
    Pnum[1][0] = 3;
    Pnum[1][1] = 4;

    pinum[0] = 1;
    pinum[1] = 2;

    double** beta = backwards_numeric(omeganum, Pnum, pinum, n_states, n_obs);

    // Act
    DdNode** _beta = _backwards(manager, _omega, _P, _pi, &row_var, &col_var, n_vars, n_obs);
    double** beta0 = add_to_matrix(_beta[0], 2, 1);
    double** beta1 = add_to_matrix(_beta[1], 2, 1);
    double** beta2 = add_to_matrix(_beta[2], 2, 1);

    // Assert
    for (int s = 0; s < n_states; s++)
    {
        ck_assert_double_eq(beta[0][s], beta0[s][0]);
        ck_assert_double_eq(beta[1][s], beta1[s][0]);
        ck_assert_double_eq(beta[2][s], beta2[s][0]);
    }
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Clean
    for (int i = 0; i < n_obs + 2; i++)
        {
            free(omeganum[i]);
        }
        
        for (int i = 0; i < n_states; i++)
        {
            free(Pnum[i]);
        }
        
        for (int i = 0; i < n_states; i++)
        {
            free(beta[i]);
        }

        free(omeganum);
        free(Pnum);
        free(pinum);
        free(beta);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(_forwards_3x3_3_n_obs)
{
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    const int n_obs = 3;
    int n_vars = 2;
    int n_states = 3;

    DdNode* row_var_0 = Cudd_addNewVar(manager);
    Cudd_Ref(row_var_0);
    DdNode* col_var_0 = Cudd_addNewVar(manager);
    Cudd_Ref(col_var_0);
    DdNode* row_var_1 = Cudd_addNewVar(manager);
    Cudd_Ref(row_var_1);
    DdNode* col_var_1 = Cudd_addNewVar(manager);
    Cudd_Ref(col_var_1);

    DdNode* row_vars[2] = {row_var_0, row_var_1};
    DdNode* col_vars[2] = {col_var_0, col_var_1};

    double P[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};
    DdNode* _P = matrix_3x3(manager, P, row_vars, col_vars);

    double pi[3] = {1, 2, 3};
    DdNode* _pi = vector_3x1(manager, pi, row_vars);

    double omega0[3] = {1, 2, 3};
    DdNode* _omega0 = vector_3x1(manager, omega0, row_vars);

    double omega1[3] = {4, 5, 6};
    DdNode* _omega1 = vector_3x1(manager, omega1, row_vars);

    double omega2[3] = {7, 8, 9};
    DdNode* _omega2 = vector_3x1(manager, omega2, row_vars);

    DdNode* _omega[3] = {_omega0, _omega1, _omega2};


    double** omeganum = (double**)malloc((n_obs + 2) * sizeof(double*));
    for (int i = 0; i <= n_obs + 1; i++)
    {
        omeganum[i] = (double*)malloc(n_states * sizeof(double));
    }
    omeganum[0][0] = 1;
    omeganum[0][1] = 2;
    omeganum[0][2] = 3;
    omeganum[1][0] = 4;
    omeganum[1][1] = 5;
    omeganum[1][2] = 6;
    omeganum[2][0] = 7;
    omeganum[2][1] = 8;
    omeganum[2][2] = 9;

    // make matrix for P that has the size states x states
    double** Pnum = (double**)malloc(n_states * sizeof(double*));
    double* pinum = (double*)malloc(n_states * sizeof(double));
    for (int i = 0; i < n_states; i++)
    {
        Pnum[i] = (double*)malloc(n_states * sizeof(double));
    }
    Pnum[0][0] = 1;
    Pnum[0][1] = 2;
    Pnum[0][2] = 3;
    Pnum[1][0] = 4;
    Pnum[1][1] = 5;
    Pnum[1][2] = 6;
    Pnum[2][0] = 7;
    Pnum[2][1] = 8;
    Pnum[2][2] = 9;

    pinum[0] = 1;
    pinum[1] = 2;
    pinum[2] = 3;

    double** alpha = forwards_numeric(omeganum, Pnum, pinum, n_states, n_obs);

    // Act
    DdNode** _alpha = _forwards(manager, _omega, _P, _pi, row_vars, col_vars, n_vars, n_obs);
    double** alpha0 = add_to_matrix(_alpha[0], n_states, 1);
    double** alpha1 = add_to_matrix(_alpha[1], n_states, 1);
    double** alpha2 = add_to_matrix(_alpha[2], n_states, 1);
    double** alpha3 = add_to_matrix(_alpha[3], n_states, 1);

    // Assert
    for (int s = 0; s < n_states; s++)
    {
        ck_assert_double_eq(alpha[0][s], alpha0[s][0]);
        ck_assert_double_eq(alpha[1][s], alpha1[s][0]);
        ck_assert_double_eq(alpha[2][s], alpha2[s][0]);
        ck_assert_double_eq(alpha[3][s], alpha3[s][0]);
    }
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Clean
    for (int i = 0; i < n_obs + 2; i++)
        {
            free(omeganum[i]);
        }
        
        for (int i = 0; i < n_states; i++)
        {
            free(Pnum[i]);
        }
        
        for (int i = 0; i < n_states; i++)
        {
            free(alpha[i]);
        }

    free(omeganum);
    free(Pnum);
    free(pinum);
    free(alpha);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(_backwards_3x3_3_n_obs)
{
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    const int n_obs = 3;
    int n_vars = 2;
    int n_states = 3;

    DdNode* row_var_0 = Cudd_addNewVar(manager);
    Cudd_Ref(row_var_0);
    DdNode* col_var_0 = Cudd_addNewVar(manager);
    Cudd_Ref(col_var_0);
    DdNode* row_var_1 = Cudd_addNewVar(manager);
    Cudd_Ref(row_var_1);
    DdNode* col_var_1 = Cudd_addNewVar(manager);
    Cudd_Ref(col_var_1);

    DdNode* row_vars[2] = {row_var_0, row_var_1};
    DdNode* col_vars[2] = {col_var_0, col_var_1};

    double P[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};
    DdNode* _P = matrix_3x3(manager, P, row_vars, col_vars);

    double pi[3] = {1, 2, 3};
    DdNode* _pi = vector_3x1(manager, pi, row_vars);

    double omega0[3] = {1, 2, 3};
    DdNode* _omega0 = vector_3x1(manager, omega0, row_vars);

    double omega1[3] = {4, 5, 6};
    DdNode* _omega1 = vector_3x1(manager, omega1, row_vars);

    double omega2[3] = {7, 8, 9};
    DdNode* _omega2 = vector_3x1(manager, omega2, row_vars);

    DdNode* _omega[3] = {_omega0, _omega1, _omega2};

    double** omeganum = (double**)malloc((n_obs + 2) * sizeof(double*));
    for (int i = 0; i <= n_obs + 1; i++)
    {
        omeganum[i] = (double*)malloc(n_states * sizeof(double));
    }
    omeganum[0][0] = 1;
    omeganum[0][1] = 2;
    omeganum[0][2] = 3;
    omeganum[1][0] = 4;
    omeganum[1][1] = 5;
    omeganum[1][2] = 6;
    omeganum[2][0] = 7;
    omeganum[2][1] = 8;
    omeganum[2][2] = 9;

    // make matrix for P that has the size states x states
    double** Pnum = (double**)malloc(n_states * sizeof(double*));
    double* pinum = (double*)malloc(n_states * sizeof(double));
    for (int i = 0; i < n_states; i++)
    {
        Pnum[i] = (double*)malloc(n_states * sizeof(double));
    }
    Pnum[0][0] = 1;
    Pnum[0][1] = 2;
    Pnum[0][2] = 3;
    Pnum[1][0] = 4;
    Pnum[1][1] = 5;
    Pnum[1][2] = 6;
    Pnum[2][0] = 7;
    Pnum[2][1] = 8;
    Pnum[2][2] = 9;

    pinum[0] = 1;
    pinum[1] = 2;
    pinum[2] = 3;

    double** beta = backwards_numeric(omeganum, Pnum, pinum, n_states, n_obs);

    // Act
    DdNode** _beta = _backwards(manager, _omega, _P, _pi, row_vars, col_vars, n_vars, n_obs);
    double** beta0 = add_to_matrix(_beta[0], n_states, 1);
    double** beta1 = add_to_matrix(_beta[1], n_states, 1);
    double** beta2 = add_to_matrix(_beta[2], n_states, 1);
    double** beta3 = add_to_matrix(_beta[3], n_states, 1);

    // Assert
    for (int s = 0; s < n_states; s++)
    {
        ck_assert_double_eq(beta[0][s], beta0[s][0]);
        ck_assert_double_eq(beta[1][s], beta1[s][0]);
        ck_assert_double_eq(beta[2][s], beta2[s][0]);
        ck_assert_double_eq(beta[3][s], beta3[s][0]);
    }
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Clean
    for (int i = 0; i < n_obs + 2; i++)
        {
            free(omeganum[i]);
        }
        
        for (int i = 0; i < n_states; i++)
        {
            free(Pnum[i]);
        }
        
        for (int i = 0; i < n_states; i++)
        {
            free(beta[i]);
        }

    free(omeganum);
    free(Pnum);
    free(pinum);
    free(beta);
    Cudd_Quit(manager);
}
END_TEST

START_TEST(fb_backwards_3x3_3_n_obs) {
    // Arrange
    int n_states = 3, n_obs = 3;

    CUDD_VALUE_TYPE omega[] = {1, 2, 3, 
                               4, 5, 6, 
                               7, 8, 9};
    CUDD_VALUE_TYPE ** omeganum = (double**)malloc((n_obs + 1) * sizeof(double*));
    for (int i = 0; i < n_obs + 1; i++)
    {
        omeganum[i] = (double*)malloc(n_states * sizeof(double));
    }
    omeganum[0][0] = 1;
    omeganum[0][1] = 2;
    omeganum[0][2] = 3;
    omeganum[1][0] = 4;
    omeganum[1][1] = 5;
    omeganum[1][2] = 6;
    omeganum[2][0] = 7;
    omeganum[2][1] = 8;
    omeganum[2][2] = 9;

    double P[] = {1, 2, 3, 
                  4, 5, 6, 
                  7, 8, 9};
    CUDD_VALUE_TYPE ** Pnum = (double**)malloc((n_obs + 1) * sizeof(double*));
    for (int i = 0; i < n_obs + 1; i++)
    {
        Pnum[i] = (double*)malloc(n_states * sizeof(double));
    }
    Pnum[0][0] = 1;
    Pnum[0][1] = 2;
    Pnum[0][2] = 3;
    Pnum[1][0] = 4;
    Pnum[1][1] = 5;
    Pnum[1][2] = 6;
    Pnum[2][0] = 7;
    Pnum[2][1] = 8;
    Pnum[2][2] = 9;

    CUDD_VALUE_TYPE pi[] = {1, 2, 3};

    // Act
    CUDD_VALUE_TYPE beta[] = {0, 0, 0, 
                              0, 0, 0, 
                              0, 0, 0, 
                              0, 0, 0};

    // Act
    int err = backwards(omega, P, pi, n_states, n_obs, beta);
    ck_assert(err == 0);

    // Assert
    double** betanum = backwards_numeric(omeganum, Pnum, pi, n_states, n_obs);

    for (int t = 0; t < n_obs; t++) {
        for (int s = 0; s < n_states; s++) {
            ck_assert_double_eq(betanum[t][s], beta[t * n_states + s]);
        }
    }

    // Clean
    for (int i = 0; i <= n_obs; i++)
    {
        free(omeganum[i]);
    }
    for (int i = 0; i < n_states; i++)
    {
        free(Pnum[i]);
    }
    for (int i = 0; i < n_states; i++)
    {
        free(betanum[i]);
    }
    free(omeganum);
    free(Pnum);
    free(betanum);
}
END_TEST

START_TEST(fb_forwards_3x3_3_n_obs) {
    // Arrange
    int n_states = 3, n_obs = 3;

    CUDD_VALUE_TYPE omega[] = {1, 2, 3, 
                               4, 5, 6, 
                               7, 8, 9};
    CUDD_VALUE_TYPE ** omeganum = (double**)malloc((n_obs + 1) * sizeof(double*));
    for (int i = 0; i < n_obs + 1; i++)
    {
        omeganum[i] = (double*)malloc(n_states * sizeof(double));
    }
    omeganum[0][0] = 1;
    omeganum[0][1] = 2;
    omeganum[0][2] = 3;
    omeganum[1][0] = 4;
    omeganum[1][1] = 5;
    omeganum[1][2] = 6;
    omeganum[2][0] = 7;
    omeganum[2][1] = 8;
    omeganum[2][2] = 9;

    double P[] = {1, 2, 3, 
                  4, 5, 6, 
                  7, 8, 9};
    CUDD_VALUE_TYPE ** Pnum = (double**)malloc((n_obs + 1) * sizeof(double*));
    for (int i = 0; i < n_obs + 1; i++)
    {
        Pnum[i] = (double*)malloc(n_states * sizeof(double));
    }
    Pnum[0][0] = 1;
    Pnum[0][1] = 2;
    Pnum[0][2] = 3;
    Pnum[1][0] = 4;
    Pnum[1][1] = 5;
    Pnum[1][2] = 6;
    Pnum[2][0] = 7;
    Pnum[2][1] = 8;
    Pnum[2][2] = 9;

    CUDD_VALUE_TYPE pi[] = {1, 2, 3};

    // Act
    CUDD_VALUE_TYPE beta[] = {0, 0, 0, 
                              0, 0, 0, 
                              0, 0, 0, 
                              0, 0, 0};

    // Act
    int err = forwards(omega, P, pi, n_states, n_obs, beta);
    ck_assert(err == 0);

    // Assert
    double** betanum = forwards_numeric(omeganum, Pnum, pi, n_states, n_obs);

    for (int t = 0; t < n_obs; t++) {
        for (int s = 0; s < n_states; s++) {
            ck_assert_double_eq(betanum[t][s], beta[t * n_states + s]);
        }
    }

    // Clean
    for (int i = 0; i <= n_obs; i++)
    {
        free(omeganum[i]);
    }
    for (int i = 0; i < n_states; i++)
    {
        free(Pnum[i]);
    }
    for (int i = 0; i < n_states; i++)
    {
        free(betanum[i]);
    }
    free(omeganum);
    free(Pnum);
    free(betanum);
}
END_TEST

START_TEST(numerical_forwardstest) {
    // Arrange
    int states = 3, n_obs = 3;
    // make matrix for omega that has the size k_j+1 x states
    double** omega = (double**)malloc((n_obs + 1) * sizeof(double*));
    for (int i = 0; i < n_obs + 1; i++)
    {
        omega[i] = (double*)malloc(states * sizeof(double));
    }
    omega[0][0] = 1;
    omega[0][1] = 2;
    omega[0][2] = 3;
    omega[1][0] = 4;
    omega[1][1] = 5;
    omega[1][2] = 6;
    omega[2][0] = 7;
    omega[2][1] = 8;
    omega[2][2] = 9;

    // make matrix for P that has the size states x states
    double** P = (double**)malloc(states * sizeof(double*));
    double* pi = (double*)malloc(states * sizeof(double));
    for (int i = 0; i < states; i++)
    {
        P[i] = (double*)malloc(states * sizeof(double));
    }
    P[0][0] = 1;
    P[0][1] = 2;
    P[0][2] = 3;
    P[1][0] = 4;
    P[1][1] = 5;
    P[1][2] = 6;
    P[2][0] = 7;
    P[2][1] = 8;
    P[2][2] = 9;

    pi[0] = 1;
    pi[1] = 2;
    pi[2] = 3;
    
    // Act
    double** alpha = forwards_numeric(omega, P, pi, states, n_obs);

    // Assert
    ck_assert_double_eq(1, 1);

    // Clean
    for (int i = 0; i <= n_obs; i++)
    {
        free(omega[i]);
    }
    for (int i = 0; i < states; i++)
    {
        free(P[i]);
    }
    for (int i = 0; i < states; i++)
    {
        free(alpha[i]);
    }
    free(omega);
    free(P);
    free(pi);
    free(alpha);
}
END_TEST

START_TEST(numerical_backwardstest) {
    // Arrange
    int states = 3, n_obs = 3;
    // make matrix for omega that has the size k_j+1 x states
    double** omega = (double**)malloc((n_obs + 1) * sizeof(double*));
    for (int i = 0; i < n_obs + 1; i++)
    {
        omega[i] = (double*)malloc(states * sizeof(double));
    }
    omega[0][0] = 1;
    omega[0][1] = 2;
    omega[0][2] = 3;
    omega[1][0] = 4;
    omega[1][1] = 5;
    omega[1][2] = 6;
    omega[2][0] = 7;
    omega[2][1] = 8;
    omega[2][2] = 9;

    // make matrix for P that has the size states x states
    double** P = (double**)malloc(states * sizeof(double*));
    double* pi = (double*)malloc(states * sizeof(double));
    for (int i = 0; i < states; i++)
    {
        P[i] = (double*)malloc(states * sizeof(double));
    }
    P[0][0] = 1;
    P[0][1] = 2;
    P[0][2] = 3;
    P[1][0] = 4;
    P[1][1] = 5;
    P[1][2] = 6;
    P[2][0] = 7;
    P[2][1] = 8;
    P[2][2] = 9;

    pi[0] = 1;
    pi[1] = 2;
    pi[2] = 3;
 
    // Act
    double** beta = backwards_numeric(omega, P, pi, states, n_obs);

    // Assert
    ck_assert_double_eq(1, 1);

    // Clean
    for (int i = 0; i <= n_obs; i++)
    {
        free(omega[i]);
    }
    for (int i = 0; i < states; i++)
    {
        free(P[i]);
    }
    for (int i = 0; i < states; i++)
    {
        free(beta[i]);
    }
    free(omega);
    free(P);
    free(pi);
    free(beta);
}
END_TEST

Suite* forwards_backwards_suite(void)
{
    Suite* s;
    TCase* tc_forwards_backwards;

    s = suite_create("forwards backwards");

    tc_forwards_backwards = tcase_create("forwards backwards");

    tcase_add_test(tc_forwards_backwards, _forwards_2x2_2_n_obs);
    tcase_add_test(tc_forwards_backwards, _backwards_2x2_2_n_obs);
    tcase_add_test(tc_forwards_backwards, _forwards_3x3_3_n_obs);
    tcase_add_test(tc_forwards_backwards, _backwards_3x3_3_n_obs);
    tcase_add_test(tc_forwards_backwards, fb_forwards_3x3_3_n_obs);
    tcase_add_test(tc_forwards_backwards, fb_backwards_3x3_3_n_obs);
    tcase_add_test(tc_forwards_backwards, numerical_forwardstest);
    tcase_add_test(tc_forwards_backwards, numerical_backwardstest);

    suite_add_tcase(s, tc_forwards_backwards);
    return s;
}
