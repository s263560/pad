#include <stdio.h>
#include <stdlib.h>
#include "MatrixIO.h"

void printMatrix(long int *mat, unsigned int lines, unsigned int columns, char *fileName)
{
  register unsigned int i, j;
  register long int value;
  FILE *arq;

  if (fileName == NULL)
        arq = stdout;
  else
  {
      arq = fopen(fileName, "w");
      if (arq == NULL)
      {
        perror("I cannot open output file.\n");
        exit(EXIT_FAILURE);
      }
  }

  for (i=0; i<lines; i++)
  {
      for(j=0; j<columns; j++)
      {
          value = mat[i*columns + j];
          if (value >= 0)
            fprintf(arq, " %ld  ",value);
          else
            fprintf(arq, "%ld  ", value);
      }
      fprintf(arq, "\n");
  }

  if (fileName != NULL)
     fclose(arq);
  return;
}

long int *readMatrix(unsigned int lines, unsigned int columns, char *fileName)
{
 register unsigned int i, j;
 long int *mat;
 FILE *arq;

 if (fileName == NULL)
 {
        fprintf(stderr,"File name cannot be null\n");
        exit(EXIT_FAILURE);
 }
 
 arq = fopen(fileName,"r");
 if (arq == NULL)
 {
   perror("I cannot open file to read\n");
   exit(EXIT_FAILURE);
 }

 mat = (long int *) malloc (sizeof(long int) * lines * columns);

 if (mat == NULL)
 {
    perror("I cannot allocate memory\n");
    exit(EXIT_FAILURE);
 }

 for (i=0; i<lines; i++)
     for(j=0; j<columns; j++)
     {
         fscanf(arq, "%ld", &mat[i * columns + j]);
     }

 fclose(arq);
 return mat;
}
