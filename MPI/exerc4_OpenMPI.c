#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define tmat double
#define tmat_MPI MPI_DOUBLE
#define printf_tmat "%5.2lf\t"

int n;
int tam_mat;

tmat *A;
tmat *B;
tmat *C;
tmat *D;    // resultado do processamento serial
tmat *D2;   // resultado do processamento paralelo
tmat *Bt;   // matriz B transposta

int quantProcs, rank;


unsigned long long GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(unsigned long long)1000000+tv.tv_usec;
}


tmat *cria_matriz(int aleatoria) {
  register unsigned int i;
  tmat *m = (tmat *) malloc( sizeof(tmat) * tam_mat);
  if (m==NULL) {
    perror("Erro na alocacao de memoria\n");
    exit(EXIT_FAILURE);
    return NULL;
  }
  if (aleatoria==1) {
    srand( (int) GetTimeStamp() );
    for (i=0; i<tam_mat; i++)
      m[i] = rand() % 10;
  }
  return m;
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

// Assume que D = A * B + C = C + A * B
// então, inicializa o acúmulo da multiplicação com C

void processa(int tam, int mostra)
{
  long t1,t2;
  unsigned long long t_start;
  n = tam;
  tam_mat = (n*n);


  // Se for o master  
  if (rank == 0)
  {
    A = cria_matriz(1);
    B = cria_matriz(1);
    C = cria_matriz(1);
    D = cria_matriz(0);
    D2 = cria_matriz(0);

    // cria a matriz transposta de B para que o cálculo fique a soma do produto de dois arrays sequenciais
    // Calcula B transposta
    Bt = cria_matriz(0);
    for (int lin=0; lin<n; lin++)
      for (int col=0; col<n; col++)
        Bt[col*n+lin] = B[lin*n+col];

    // -----------------------------
    // Faz o cálculo de forma serial
    // -----------------------------
    // Pega tempo do início da execução
    t_start = GetTimeStamp();
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
    // tempo da execução serial
    t1 = (long) (GetTimeStamp()-t_start);
  } 
 

  // -------------------------------
  // Faz o cálculo de forma paralela
  // -------------------------------
  // Pega tempo do início da execução
  if (rank == 0) t_start = GetTimeStamp();

  // Quantidade de linhas da matriz a ser processada por cada nodo
  int num_linhas = n / quantProcs;
  int tam_part = (num_linhas*n);

  // aloca memória para os workers receberem Bt por broadcast
  if (rank != 0) {
    Bt = cria_matriz(0);
  }  

  // Cria matrizes parciais com num_linhas sobre A e C para calcula a multiplicação
  // Resultado da multiplicação é calculado em Cpart
  tmat *Apart = (tmat *) malloc( sizeof(tmat) * tam_part);
  tmat *Cpart = (tmat *) malloc( sizeof(tmat) * tam_part);

  // envia matriz transposta Bt para todos os nodos
  MPI_Bcast(Bt, tam_mat, tmat_MPI, 0, MPI_COMM_WORLD);
  // envia linhas de A e C para cada um dos nodos
  MPI_Scatter(A, tam_part, tmat_MPI, Apart, tam_part, tmat_MPI, 0, MPI_COMM_WORLD);
  MPI_Scatter(C, tam_part, tmat_MPI, Cpart, tam_part, tmat_MPI, 0, MPI_COMM_WORLD);

  for(int linha=0; linha<num_linhas; linha++) {
    for(int col=0; col<n; col++) {
      tmat S = Cpart[linha*n+col];
      tmat *pA = Apart + (linha*n);   // início da linha a somar na multiplicacao
      tmat *pBt = Bt + (col*n); // início da linha transposta a somar na multiplicacao
      for (int k=0; k<n; k++)
        S += *(pA+k) * *(pBt+k);
      Cpart[linha*n+col] = S;  
    }
  }

  // recebe o resultado da multiplicação em D2
  MPI_Gather(Cpart, tam_part, tmat_MPI, D2, tam_part, tmat_MPI, 0, MPI_COMM_WORLD);  
  
  // libera memória do cálculo parcial
  free(Apart);
  free(Cpart);

  // caso tenha "resto" na divisão de linhas pelo número de workers, calcula as linhas restantes no master
  int linha_resto_ini = num_linhas * quantProcs;
  if (rank==0) {
    for(int linha = linha_resto_ini; linha<n; linha++) {
      for(int col=0; col<n; col++) {
        tmat S = C[linha*n+col];
        tmat *pA = A + (linha*n);   // início da linha a somar na multiplicacao
        tmat *pBt = Bt + (col*n); // início da linha transposta a somar na multiplicacao
        for (int k=0; k<n; k++)
          S += *(pA+k) * *(pBt+k);
        D2[linha*n+col] = S;  
      }
    }
  }

  // fim do processamento
  if (rank == 0) t2 = (long) (GetTimeStamp()-t_start);

  if (rank == 0)
  {
    // Valida cálculo
    int erro=0;
    for (int c=0; c<tam_mat; c++) {
      if (D[c]!=D2[c]) {
        erro=1;
        break;
      }
    }
  
    printf("n = %d, serial.   Tempo: %lu microssegundos\n", n, t1);
    printf("n = %d, paralelo. Tempo: %lu microssegundos\n", n, t2);
    if (linha_resto_ini<n) {
      printf("resto de linhas de %d a %d calculadas em serial pelo master\n", linha_resto_ini, n-1);
    }
    if (erro) printf("--ERRO NO CALCULO!!!!!\n");

    if (mostra) 
    {
      mostra_matriz(A,"A");
      mostra_matriz(B,"B");
      mostra_matriz(C,"C");
      mostra_matriz(Bt,"B transposta");
      mostra_matriz(D,"D = A*B+C");
      mostra_matriz(D2,"D2 = A*B+C");

      free(Bt);
      free(A);
      free(B);
      free(C);
      free(D);
      free(D2);
    }
  }

  return;
}


int main(int argc, char *argv[]) 
{

  MPI_Status Stat;
  // Inicializa o MPI
  MPI_Init(&argc,&argv);
  // Obtem a quantidade de processos
  MPI_Comm_size(MPI_COMM_WORLD, &quantProcs);
  // Obtem o rank (identificador) do processo
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
  int tn[3] = { 10, 100, 1000 };
  for(int cn=0; cn<3; cn++)
    processa(tn[cn],0);
   // teste do cálculo
   // processa(10,1);
  
  MPI_Finalize();
     
  return 0;
}
