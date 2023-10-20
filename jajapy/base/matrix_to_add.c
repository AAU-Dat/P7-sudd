#include "matrix_to_add.h"

/* function makes matrix to ADD*/
int matrixToADD(double **matrix, DdManager *gbm, DdNode **E, DdNode ***x, DdNode ***y, DdNode ***xn, DdNode ***yn, int *nx, int *ny, int *m, int *n)
{
   // write the matrix to a file
   writeMatrixToFile(matrix, m, n);

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
   Cudd_addRead(file, gbm, E, x, y, xn, yn, nx, ny, m, n, 0, 2, 1, 2);

   // clean up
   fclose(file);
   remove(filename);
   
   return 0;
}

/* function writes matrix to file*/
int writeMatrixToFile(double **matrix, int *m, int *n)
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
