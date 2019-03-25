#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MatrixIO.h"

long int *generateRandomMatriz(unsigned int lines, unsigned int columns, int lowerLimit, int upperLimit)
{
 register unsigned int i, j;

 long int *mat = (long int *) malloc(sizeof(long int) * lines * columns);

 if (mat == NULL)
 {
     perror("I cannot allocate memory\n");
     exit(EXIT_FAILURE);
     return NULL;
 }

 srand( time(NULL) );
 for (i=0; i<lines; i++)
     for(j=0; j<columns; j++)
        mat[i*columns + j] = (rand() % ((upperLimit + 1) - lowerLimit)) + lowerLimit;

 return mat;
}

int main (int argc, char **argv)
{
 unsigned int lines, columns;
 long int lowerLimit, upperLimit;
 char *fileName;
 long int *mat;

 switch(argc)
 {
     case 0:
     case 1:
     case 2:
         fprintf(stderr, "Uso:\n\t %s <number of lines> <number of colums>\n or", argv[0]);
         fprintf(stderr, "\n\t %s <number of lines> <number of colums> <output file>\n or", argv[0]);
         fprintf(stderr, "\n\t %s <number of lines> <number of colums> <lower limit> <upper limit>\n or", argv[0]);
         fprintf(stderr, "\n\t %s <number of lines> <number of colums> <lower limit> <upper limit> <output file>\n", argv[0]);

         exit(EXIT_FAILURE);
         break;
     case 3:
         lines = atoi(argv[1]);
         columns = atoi(argv[2]);
         lowerLimit = -(lines*columns);
         upperLimit = (lines*columns);
         fileName = NULL;

         fprintf(stdout, "Generating %d, %d matrix, without values limits to stdout\n", lines, columns);
         break;
     case 4:
         lines = atoi(argv[1]);
         columns = atoi(argv[2]);
         lowerLimit = -(lines*columns); 
         upperLimit = (lines*columns);
         fileName = argv[3];

         fprintf(stdout, "Generating %d, %d matrix, without values limits to %s\n", lines, columns, fileName);
         break;
     case 5:
         lines = atoi(argv[1]);
         columns = atoi(argv[2]);
         lowerLimit = atoi(argv[3]);
         upperLimit = atoi(argv[4]);
         fileName = NULL;

         fprintf(stdout, "Generating %d, %d matrix, with values [%ld, %ld] to stdout\n", 
                          lines, columns, lowerLimit, upperLimit);
         break;

    case 6:
        lines = atoi(argv[1]);
        columns = atoi(argv[2]);
        lowerLimit = atoi(argv[3]);
        upperLimit = atoi(argv[4]);
        fileName = argv[5];

        fprintf(stdout, "Generating %d, %d matrix, with values [%ld, %ld] to file %s\n",
                                 lines, columns, lowerLimit, upperLimit, fileName);
        break;
 }

 mat = generateRandomMatriz(lines, columns, lowerLimit, upperLimit);
 printMatrix(mat, lines, columns, fileName);
 free(mat);
 return 0;
}
