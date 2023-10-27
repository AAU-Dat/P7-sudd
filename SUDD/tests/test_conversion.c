#include "test_conversion.h"

START_TEST(matrix_to_add__2x2)
{
    int n = 2; // number of columns
    int m = 2; // number of rows

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double** matrix = (double**)malloc(m * sizeof(double*));
    matrix[0] = (double*)malloc(n * sizeof(double*));
    matrix[1] = (double*)malloc(n * sizeof(double*));
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;

    matrix_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);

    CUDD_VALUE_TYPE** actual = add_to_matrix(E, m, n);

    // Assert
    CUDD_VALUE_TYPE expected[2][2] = {
        {1, 2},
        {3, 4}};
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[1][1], actual[1][1]);

    // free memory of matrix
    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(manager);
    free(matrix);
}
END_TEST

START_TEST(matrix_to_add__1x4)
{
    int m = 1; // number of rows
    int n = 4; // number of columns

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double** matrix = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }

    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[0][3] = 4;

    matrix_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);
    CUDD_VALUE_TYPE** actual = add_to_matrix(E, 1, 4);
    CUDD_VALUE_TYPE expected[1][4] = {
        {1, 2, 3, 4}};

    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[0][2], actual[0][2]);
    ck_assert_double_eq(expected[0][3], actual[0][3]);

    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(manager);
    free(matrix);
}
END_TEST

START_TEST(matrix_to_add__4x1)
{
    int m = 4; // number of rows
    int n = 1; // number of columns

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double** matrix = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }

    matrix[0][0] = 1;
    matrix[1][0] = 2;
    matrix[2][0] = 3;
    matrix[3][0] = 4;

    matrix_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);

    CUDD_VALUE_TYPE** actual = add_to_matrix(E, 4, 1);
    CUDD_VALUE_TYPE expected[4][1] = {
        {1},
        {2},
        {3},
        {4}};

    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[2][0], actual[2][0]);
    ck_assert_double_eq(expected[3][0], actual[3][0]);

    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(manager);
    free(matrix);
}
END_TEST

START_TEST(matrix_to_add__1x1)
{
    int m = 1; // number of rows
    int n = 1; // number of columns

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double** matrix = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }

    matrix[0][0] = 1;

    matrix_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);

    CUDD_VALUE_TYPE** actual = add_to_matrix(E, 1, 1);

    // Assert
    CUDD_VALUE_TYPE expected[1][1] = {{1}};
    ck_assert_double_eq(expected[0][0], actual[0][0]);

    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(manager);
    free(matrix);
}
END_TEST

START_TEST(matrix_to_add__3x3)
{
    int m = 3; // number of rows
    int n = 3; // number of columns

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double** matrix = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }

    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    matrix[1][2] = 6;
    matrix[2][0] = 7;
    matrix[2][1] = 8;
    matrix[2][2] = 9;

    matrix_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);

    CUDD_VALUE_TYPE**actual = add_to_matrix(E, 3, 3);

    // Assert
    CUDD_VALUE_TYPE expected[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[0][2], actual[0][2]);

    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[1][1], actual[1][1]);
    ck_assert_double_eq(expected[1][2], actual[1][2]);

    ck_assert_double_eq(expected[2][0], actual[2][0]);
    ck_assert_double_eq(expected[2][1], actual[2][1]);
    ck_assert_double_eq(expected[2][2], actual[2][2]);

    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(manager);
    free(matrix);
}
END_TEST

START_TEST(add_to_matrix__4x4_same_row)
{
    int m = 4; // number of rows
    int n = 4; // number of columns

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double** matrix = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }

    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[0][4] = 4;

    matrix[1][0] = 1;
    matrix[1][1] = 2;
    matrix[1][2] = 3;
    matrix[1][4] = 4;

    matrix[2][0] = 1;
    matrix[2][1] = 2;
    matrix[2][2] = 3;
    matrix[2][4] = 4;

    matrix[3][0] = 1;
    matrix[3][1] = 2;
    matrix[3][2] = 3;
    matrix[3][4] = 4;

    matrix_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);

    CUDD_VALUE_TYPE**actual = add_to_matrix(E, m, n);

    // Assert
    for (int i = 0; i < m; i++) {
        for (int j = 0; i < n; j++) {
            ck_assert_double_eq(matrix[i][j], actual[i][j]);
        }
    }

    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(manager);
    free(matrix);
}
END_TEST


START_TEST(add_to_matrix__2x2)
{
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    double matrix[2][2] = {
        {1, 2},
        {3, 4}};
    DdNode* _matrix = matrix_2x2(manager, matrix, row_var, col_var);
    Cudd_Ref(_matrix);

    // Act
    CUDD_VALUE_TYPE** actual = add_to_matrix(_matrix, 2, 2);

    // Assert
    ck_assert_double_eq(matrix[0][0], actual[0][0]);
    ck_assert_double_eq(matrix[0][1], actual[0][1]);
    ck_assert_double_eq(matrix[1][0], actual[1][0]);
    ck_assert_double_eq(matrix[1][1], actual[1][1]);
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

START_TEST(add_to_matrix__1x4)
{
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    double matrix[2][2] = {
        {1, 2},
        {3, 4}};
    DdNode* _matrix = matrix_2x2(manager, matrix, row_var, col_var);
    Cudd_Ref(_matrix);
    // Act
    CUDD_VALUE_TYPE** actual = add_to_matrix(_matrix, 1, 4);

    // Assert
    CUDD_VALUE_TYPE expected[1][4] = {
        {1, 2, 3, 4}};
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[0][2], actual[0][2]);
    ck_assert_double_eq(expected[0][3], actual[0][3]);
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

START_TEST(add_to_matrix__4x1)
{
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* row_var = Cudd_addNewVar(manager);
    Cudd_Ref(row_var);
    DdNode* col_var = Cudd_addNewVar(manager);
    Cudd_Ref(col_var);

    double matrix[2][2] = {
        {1, 2},
        {3, 4}};
    DdNode* _matrix = matrix_2x2(manager, matrix, row_var, col_var);
    Cudd_Ref(_matrix);
    // Act
    CUDD_VALUE_TYPE** actual = add_to_matrix(_matrix, 4, 1);

    // Assert
    CUDD_VALUE_TYPE expected[4][1] = {
        {1},
        {2},
        {3},
        {4}};
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[2][0], actual[2][0]);
    ck_assert_double_eq(expected[3][0], actual[3][0]);
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

START_TEST(add_to_matrix__1x1)
{
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode* add_one = Cudd_addConst(manager, 1);
    Cudd_Ref(add_one);

    // Act
    CUDD_VALUE_TYPE** actual = add_to_matrix(add_one, 1, 1);

    // Assert
    CUDD_VALUE_TYPE expected[1][1] = {{1}};
    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // Cleanup
    Cudd_RecursiveDeref(manager, add_one);

    Cudd_Quit(manager);
}
END_TEST

START_TEST(add_to_matrix__3x3)
{
    // Arrange
    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

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

    double matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};
    DdNode* _matrix = matrix_3x3(manager, matrix, row_vars, col_vars);

    // Act
    CUDD_VALUE_TYPE** actual = add_to_matrix(_matrix, 3, 3);

    // Assert
    CUDD_VALUE_TYPE expected[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};
    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    ck_assert_double_eq(expected[0][0], actual[0][0]);
    ck_assert_double_eq(expected[0][1], actual[0][1]);
    ck_assert_double_eq(expected[0][2], actual[0][2]);

    ck_assert_double_eq(expected[1][0], actual[1][0]);
    ck_assert_double_eq(expected[1][1], actual[1][1]);
    ck_assert_double_eq(expected[1][2], actual[1][2]);

    ck_assert_double_eq(expected[2][0], actual[2][0]);
    ck_assert_double_eq(expected[2][1], actual[2][1]);
    ck_assert_double_eq(expected[2][2], actual[2][2]);

    // Cleanup
    Cudd_Quit(manager);
}
END_TEST

START_TEST(vector_to_add__4x1)
{
    int m = 4; // number of rows
    int n = 1; // number of columns

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double* vector = (double*)malloc(m * sizeof(double));

    vector[0] = 1;
    vector[1] = 2;
    vector[2] = 3;
    vector[3] = 4;

    vector_to_add(vector, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m);

    CUDD_VALUE_TYPE** actual = add_to_matrix(E, 4, 1);
    CUDD_VALUE_TYPE expected[4] = {1, 2, 3, 4};

    ck_assert_double_eq(expected[0], actual[0][0]);
    ck_assert_double_eq(expected[1], actual[1][0]);
    ck_assert_double_eq(expected[2], actual[2][0]);
    ck_assert_double_eq(expected[3], actual[3][0]);

    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    Cudd_Quit(manager);
    free(vector);
}
END_TEST

START_TEST(vector_to_add__1x1)
{
    int m = 1; // number of rows
    int n = 1; // number of columns

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double* vector = (double*)malloc(m * sizeof(double));

    vector[0] = 1;

    vector_to_add(vector, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m);

    CUDD_VALUE_TYPE** actual = add_to_matrix(E, 1, 1);

    // Assert
    CUDD_VALUE_TYPE expected[1] = {1};
    ck_assert_double_eq(expected[0], actual[0][0]);

    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    Cudd_Quit(manager);
    free(vector);
}
END_TEST

// This test checks that the function vector_to_add fails when called on a 2x2 matrix
START_TEST(vector_to_add__2x2)
{
    int n = 2; // number of columns
    int m = 2; // number of rows

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double** matrix = (double**)malloc(m * sizeof(double*));
    matrix[0] = (double*)malloc(n * sizeof(double*));
    matrix[1] = (double*)malloc(n * sizeof(double*));
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;

    printf("\nThe following print should be an error coming from test (vector_2x2) in test_vector_to_add.c: \n");

    // Gets the result from the function. Expected value is 1, as we wrongly called the function on a 2D-matrix
    int result = vector_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m);

    // Make sure that the function failed
    ck_assert_int_ne(result, 0);

    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    // free memory of matrix
    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(manager);
    free(matrix);
}
END_TEST

// This test checks that the function vector_to_add fails when called on a 3x3 matrix
START_TEST(vector_to_add__3x3)
{
    int m = 3; // number of rows
    int n = 3; // number of columns

    DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode* E;
    DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
    DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
    int nx = 0, ny = 0;

    double** matrix = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }

    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    matrix[1][2] = 6;
    matrix[2][0] = 7;
    matrix[2][1] = 8;
    matrix[2][2] = 9;

    printf("\nThe following print should be an error coming from test (vector_3x3) in test_vector_to_add.c: \n");

    // Gets the result from the function. Expected value is 1, as we wrongly called the function on a 2D-matrix
    int result3x3 = vector_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m);

    // Make sure that the function failed
    ck_assert_int_ne(result3x3, 0);

    ck_assert_int_eq(Cudd_DebugCheck(manager), 0);

    for (int i = 0; i < m; i++)
    {
        free(matrix[i]);
    }
    Cudd_Quit(manager);
    free(matrix);
}
END_TEST

Suite* conversion_suite(void)
{
    Suite* s;
    TCase* tc_conversion;

    s = suite_create("conversion");

    /* Core test case */
    tc_conversion = tcase_create("conversion");

    tcase_add_test(tc_conversion, matrix_to_add__2x2);
    tcase_add_test(tc_conversion, matrix_to_add__1x4);
    tcase_add_test(tc_conversion, matrix_to_add__4x1);
    tcase_add_test(tc_conversion, matrix_to_add__1x1);
    tcase_add_test(tc_conversion, matrix_to_add__3x3);
    tcase_add_test(tc_conversion, add_to_matrix__4x4_same_row);
    tcase_add_test(tc_conversion, add_to_matrix__2x2);
    tcase_add_test(tc_conversion, add_to_matrix__1x4);
    tcase_add_test(tc_conversion, add_to_matrix__4x1);
    tcase_add_test(tc_conversion, add_to_matrix__1x1);
    tcase_add_test(tc_conversion, add_to_matrix__3x3);
    tcase_add_test(tc_conversion, vector_to_add__4x1);
    tcase_add_test(tc_conversion, vector_to_add__1x1);
    // tcase_add_test(tc_core, matrix_2x2);
    // tcase_add_test(tc_core, matrix_3x3);
    suite_add_tcase(s, tc_conversion);

    return s;
}
