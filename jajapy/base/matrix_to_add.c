#include "matrix_to_add.h"

/* function makes matrix to ADD*/
void matrixToADD(DdManager *gbm, double **matrix, DdNode *E, DdNode **x, DdNode **y, DdNode **xn, DdNode **yn_, int *nx, int *ny, int *m, int *n)
{
   // initialize variables
   FILE *filepointer;
   int size_row, size_col;
   char filename[40];
   sprintf(filename, "matrix%d.txt", getpid());
   // write matrix to file
   writeMatrixToFile(matrix, &m, &n);
   filepointer = fopen(filename, "r");
   
   // read the first line for finding size of matrix for allocating space
   fscanf(filepointer, "%d %d", &size_row, &size_col);

   // this function takes the file and creates the ADD
   Cudd_addRead(filepointer, gbm, E, x, y, xn, yn_, nx, ny, m, n, 0, 2, 1, 2);

   // close the file
   fclose(filepointer);
}

void writeMatrixToFile(double **matrix, int *m, int *n)
{
   FILE *filepointer;

   char filename[40];
   sprintf(filename, "matrix%d.txt", getpid());
   // open file
   filepointer = fopen(filename, "w");

   // write size of matrix to file
   fprintf(filepointer, "%d %d\n", m, n);

   // write into file with a loop
   for (int i = 0; i < m; i++)
   {
      for (int j = 0; j < n; j++)
      {
         fprintf(filepointer, "%d %d %lf\n", i, j, matrix[i][j]);
      }
   }
   fclose(filepointer);
}
