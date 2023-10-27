#include "conversion.h"

/* function makes vector to ADD*/
int vector_to_add(
    double* vector, 
    DdManager* manager, 
    DdNode** E, 
    DdNode*** x, 
    DdNode*** y, 
    DdNode*** xn, 
    DdNode*** yn, 
    int* nx, 
    int* ny, 
    int* m)
{
   // write the vector to a file
   int result = write_vector_to_file(vector, m);

   if (result == 1) {
      return 1;
   }

   // read the file
   FILE* file;
   char filename[40];
   sprintf(filename, "vector%d.txt", getpid());
   file = fopen(filename, "r");
   if (file == NULL)
   {
      perror("Error opening file");
      return 1; // Return an error code
   }

   // n is set to 1 as we are working with vectors
   int* n;
   int nn = 1;
   n = &nn;
   // read we now take the file and make it into an ADD
   Cudd_addRead(file, manager, E, x, y, xn, yn, nx, ny, m, n, 0, 2, 1, 2);

   // clean up
   fclose(file);
   remove(filename);
   
   return 0;
}

/* function writes vector to file*/
int write_vector_to_file(double* vector, int* m)
{
   FILE* file;
   char filename[40];
   sprintf(filename, "vector%d.txt", getpid());
   file = fopen(filename, "w");
   if (file == NULL)
   {
      perror("Error opening file");
      return 1; // Return an error code
   }
   fprintf(file, "%d 1\n", *m);

   int i;

   for (i = 0; i < *m; i++)
   {
         fprintf(file, "%d 0 %f\n", i, vector[i]);  
   }

   fclose(file);

   return 0;
}

CUDD_VALUE_TYPE** add_to_matrix(DdNode* symbolic, int n_rows, int n_columns) {
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
            result[i][j] = evaluate_matrix_bdd(root, bits, 0);
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

CUDD_VALUE_TYPE evaluate_matrix_bdd(DdNode* node, bool bits[], int index) {
    if (Cudd_IsConstant(node)) {
        return Cudd_V(node);
    }
    if (bits[index]) {
        return evaluate_matrix_bdd(Cudd_T(node), bits, index + 1);
    } else {
        return evaluate_matrix_bdd(Cudd_E(node), bits, index + 1);
    }
}

void interleave(bool A[], int size_a, bool B[], int size_b, bool result[]) {
    int a_index = 0, b_index = 0, r_index = 0;

    while (a_index < size_a && b_index < size_b) {
        result[r_index++] = A[a_index++];
        result[r_index++] = B[b_index++];
    }

    while (a_index < size_a) {
        result[r_index++] = A[a_index++];
    }

    while (b_index < size_b) {
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

int matrix_to_add(
    double** matrix, 
    DdManager* manager, 
    DdNode** E, 
    DdNode*** x, 
    DdNode*** y, 
    DdNode*** xn, 
    DdNode*** yn, 
    int* nx, 
    int* ny, 
    int* m, 
    int* n)
{
   // write the matrix to a file
   write_matrix_to_file(matrix, m, n);

   // read the file
   FILE *file;
   char filename[40];
   sprintf(filename, "matrix%d.txt", getpid());
   file = fopen(filename, "r");
   if (file == NULL)
   {
      perror("Error opening file");
      return 1; // Return an error code
   }
   // read we now take the file and make it into an ADD
   Cudd_addRead(file, manager, E, x, y, xn, yn, nx, ny, m, n, 0, 2, 1, 2);

   // clean up
   fclose(file);
   remove(filename);
   
   return 0;
}

/* function writes matrix to file*/
int write_matrix_to_file(double** matrix, int* m, int* n)
{
   FILE *file;
   char filename[40];
   sprintf(filename, "matrix%d.txt", getpid());
   file = fopen(filename, "w");
   if (file == NULL)
   {
      perror("Error opening file");
      return 1; // Return an error code
   }
   fprintf(file, "%d %d\n", *m, *n);

   int i, j;
   
   for (i = 0; i < *m; i++)
   {
      for (j = 0; j < *n; j++)
      {
         fprintf(file, "%d %d %f\n", i, j, matrix[i][j]);
      }
   }

   fclose(file);

   return 0;
}
