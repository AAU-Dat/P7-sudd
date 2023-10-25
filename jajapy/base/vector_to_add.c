#include "vector_to_add.h"

/* function makes vector to ADD*/
int vectorToADD(double **vector, DdManager *gbm, DdNode **E, DdNode ***x, DdNode ***y, DdNode ***xn, DdNode ***yn, int *nx, int *ny, int *m)
{
   // write the vector to a file
   int result = writeVectorToFile(vector, m);

   if (result == 1) {
      return 1;
   }

   // read the file
   FILE *file;
   char filename[40];
   sprintf(filename, "vector%d.txt", getpid());
   file = fopen(filename, "r");
   if (file == NULL)
   {
      perror("Error opening file");
      return 1; // Return an error code
   }

   // n is set to 1 as we are working with vectors
   int *n;
   int nn = 1;
   n = &nn;
   // read we now take the file and make it into an ADD
   Cudd_addRead(file, gbm, E, x, y, xn, yn, nx, ny, m, n, 0, 2, 1, 2);

   // clean up
   fclose(file);
   remove(filename);
   
   return 0;
}

/* function writes vector to file*/
int writeVectorToFile(double **vector, int *m)
{
   FILE *file;
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

   // If vector is actually a 2-dimensional matrix, and not a column vector, throw an error
   if (vector[0][1]) {
    perror("ERROR: Function vectorToADD was called with a 2-dimensional matrix and not a column vector");
    fclose(file);
    remove(filename);
    return 1; // Return an error code
   }

   for (i = 0; i < *m; i++)
   {
         fprintf(file, "%d 0 %f\n", i, vector[i][0]);  
   }

   fclose(file);

   return 0;
}
