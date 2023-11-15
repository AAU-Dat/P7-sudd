#include "test_conversion.h"

// START_TEST(add_to_matrix__4x4_same_row)
// {
//     int m = 4; // number of rows
//     int n = 4; // number of columns
//
//     DdManager* manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
//     DdNode* E;
//     DdNode** x = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
//     DdNode** y = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
//     DdNode** xn = (DdNode**)malloc(ceil(log2(m)) * sizeof(DdNode*));
//     DdNode** yn = (DdNode**)malloc(ceil(log2(n)) * sizeof(DdNode*));
//     int nx = 0, ny = 0;
//
//     double** matrix = (double**)malloc(m * sizeof(double*));
//     for (int i = 0; i < m; i++)
//     {
//         matrix[i] = (double*)malloc(n * sizeof(double));
//     }
//
//     matrix[0][0] = 1;
//     matrix[0][1] = 2;
//     matrix[0][2] = 3;
//     matrix[0][3] = 4;
//
//     matrix[1][0] = 1;
//     matrix[1][1] = 2;
//     matrix[1][2] = 3;
//     matrix[1][3] = 4;
//
//     matrix[2][0] = 1;
//     matrix[2][1] = 2;
//     matrix[2][2] = 3;
//     matrix[2][3] = 4;
//
//     matrix[3][0] = 1;
//     matrix[3][1] = 2;
//     matrix[3][2] = 3;
//     matrix[3][3] = 4;
//
//     matrix_to_add(matrix, manager, &E, &x, &y, &xn, &yn, &nx, &ny, &m, &n);
//
//     CUDD_VALUE_TYPE**actual = add_to_matrix(E, m, n);
//
//     // Assert
//     for (int i = 0; i < m; i++) {
//         for (int j = 0; j < n; j++) {
//             ck_assert_double_eq(matrix[i][j], actual[i][j]);
//         }
//     }
//
//     for (int i = 0; i < m; i++)
//     {
//         free(matrix[i]);
//     }
//     Cudd_Quit(manager);
//     free(matrix);
// }
// END_TEST


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

Suite* conversion_suite(void)
{
    Suite* s;
    TCase* tc_conversion;

    s = suite_create("conversion");

    /* Core test case */
    tc_conversion = tcase_create("conversion");

    // tcase_add_test(tc_conversion, add_to_matrix__4x4_same_row);
    tcase_add_test(tc_conversion, add_to_matrix__2x2);
    tcase_add_test(tc_conversion, add_to_matrix__1x1);
    tcase_add_test(tc_conversion, add_to_matrix__3x3);
    
    suite_add_tcase(s, tc_conversion);

    return s;
}
