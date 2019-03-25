#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "MatrixOperationsOMP.h"
#include "MatrixIO.h"
long int *multiply(long int *A, long int *B, unsigned int linesA, unsigned int columns_lines, unsigned int columnsB) 
{ 
 long int *C;

 C = (long int *) malloc(sizeof(long int) * linesA * columnsB);

 if (C == NULL)
 {
   perror("A cannot allocate memory\n");
   exit(EXIT_FAILURE);
 }
      
 #pragma omp parallel
 {
   register unsigned int i, j, k;
   #pragma omp for
   for (i = 0; i < linesA; i++) 
   {   
      for (j = 0; j < columnsB; j++) 
      {
       register long int dot  = 0;
       for (k = 0; k < columns_lines; k++) 
       {
         dot += A[ i*columns_lines + k] * B[ k*columnsB + j];
       } 
       C[ i*columnsB + j ] = dot;
      }
   }
 }
 return C;
}

int main(int argc, char **argv)
{
 unsigned int linesA, columns_lines, columnsB;
 char *fileNameA;
 char *fileNameB;
 char *fileOutput;
 long int *A;
 long int *B;
 long int *C;

 switch(argc)
 {
     case 0:
     case 1:
     case 2:
     case 3:
     case 4:
     case 5:
     case 6:
         fprintf(stderr, "Uso:\n\t %s <input file A> <number of lines> <number of colums> <input File B> <number of columns> <output file>\n", argv[0]);
         exit(EXIT_FAILURE);
         break;
     case 7:
         fileNameA = argv[1];
         linesA = atoi(argv[2]);
         columns_lines = atoi(argv[3]);
         fileNameB = argv[4];
         columnsB = atoi(argv[5]);
         fileOutput = argv[6];
         break;
 }

#pragma omp parallel 
 {
   #pragma omp sections nowait
   {

     #pragma omp section
     {
           double times[2];
           printf("Reading first matrix from %s\n\n", fileNameA);
           times[0] = omp_get_wtime();
           A = readMatrix(linesA, columns_lines, fileNameA);
           times[1] = omp_get_wtime();
           printf("Read first matrix completed. Time elapsed: %.5g\n\n", times[1] - times[0]);
     }
     #pragma omp section
     {
       double times[2];
       printf("Reading second matrix from %s\n\n", fileNameB);
       times[0] = omp_get_wtime();
       B = readMatrix(columns_lines, columnsB, fileNameB);
       times[1] = omp_get_wtime();
       printf("Read second matrix completed. Time elapsed: %.5g\n\n", times[1] - times[0]);
     }
   }
 }

 double times[2];
 printf("Multiplying Matrixes. ");
 times[0] = omp_get_wtime();
 C = multiply(A, B, linesA, columns_lines, columnsB);
 times[1] = omp_get_wtime();
 printf("Time elapsed: %.5g\n", times[1] - times[0]);

 printf("Generating output file %s.\n\n", fileOutput);
 printMatrix(C, linesA, columnsB, fileOutput);
 free(A); 
 free(B);
 free(C);
 return 0;
}
