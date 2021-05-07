#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define tmat double
#define printf_tmat "%5.2lf\t"

int THREADs;
int n;
int tam_mat;

tmat *A;
tmat *B;
tmat *C;
tmat *D;
tmat *Bt;     // matriz B transposta


tmat *cria_matriz(int aleatoria) {
  register unsigned int i;
  tmat *m = (tmat *) malloc( sizeof(tmat) * tam_mat);
  if (m==NULL) {
    perror("Erro na alocacao de memoria\n");
    exit(EXIT_FAILURE);
    return NULL;
  }
  if (aleatoria==1) {
    srand( time(NULL) );
    for (i=0; i<tam_mat; i++)
      m[i] = rand() % 10;
  }
  return m;
}

// Assume que D = A * B + C = C + A * B
// então, inicializa o acúmulo da multiplicação com C
void Calcula_Threads()
{
  #pragma omp parallel num_threads(THREADs)
  {
    #pragma omp for
    for (int c=0; c<tam_mat; c++) {
      int linha=c/n;  
      int col=c%n;
      tmat S = C[c];
      tmat *pA = A + (linha*n);  // início da linha a somar na multiplicacao
      tmat *pBt = Bt + (col*n);  // início da linha transposta a somar na multiplicacao
      for (register int k=0; k<n; k++)
        S += *(pA+k) * *(pBt+k);
      D[c] = S;  
    }
  }  
}

void Calcula_Serial()
{
    for (int c=0; c<tam_mat; c++) {
      int linha=c/n;  
      int col=c%n;
      tmat S = C[c];
      tmat *pA = A + (linha*n);  // início da linha a somar na multiplicacao
      tmat *pBt = Bt + (col*n);  // início da linha transposta a somar na multiplicacao
      for (register int k=0; k<n; k++)
        S += *(pA+k) * *(pBt+k);
      D[c] = S;  
    }
}

unsigned long long GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(unsigned long long)1000000+tv.tv_usec;
}


void mostra_matriz(tmat *A, const char *nome)
{
  printf("Matriz %s:\n",nome);
  for (int lin=0; lin<n; lin++)
  {
    for (int col=0; col<n; col++)
      printf(printf_tmat,A[lin*n+col]);
    printf("\n");
  }   
  printf("\n");
}


long processa(int threads, int tam, int mostra)
{
  THREADs = threads;
  n = tam;
  tam_mat = (n*n);

  long i;

  A = cria_matriz(1);
  B = cria_matriz(1);
  C = cria_matriz(1);
  D = cria_matriz(0);

  // Pega tempo do início da execução
  unsigned long long t_start, t_end;
  t_start = GetTimeStamp();

  // cria a matriz transposta de B para que o cálculo fique a soma do produto de dois arrays sequenciais
  // Calcula B transposta
  Bt = cria_matriz(0);
  for (int lin=0; lin<n; lin++)
    for (int col=0; col<n; col++)
      Bt[col*n+lin] = B[lin*n+col];


  if (threads==1) 
    Calcula_Serial();
  else 
    Calcula_Threads();
  
  // fim do processamento
  t_end = GetTimeStamp();
  
  if (mostra) 
  {
    mostra_matriz(A,"A");
    mostra_matriz(B,"B");
    mostra_matriz(C,"C");
    mostra_matriz(D,"D = A*B+C");
    mostra_matriz(Bt,"B transposta");
  }

  free(Bt);
  
  free(A);
  free(B);
  free(C);
  free(D);
  return (long)(t_end-t_start);
}


int main() 
{
    //teste do cálculo
    //printf("Tempo total de execução: %lu microsegundos\n",processa(3,10,1));
 
   int tn[3] = { 10, 100, 1000 };
   int tt[6] = { 1, 2, 4, 8, 16, 32 };
   
   for(int cn=0; cn<3; cn++)
     for(int ct=0; ct<6; ct++)
     {
       int n = tn[cn];
       int t = tt[ct];
       printf("n = %d, com %d threads. Tempo: %lu microssegundos\n", n, t, processa(t,n,0));
     }
     
   return 0;
}
