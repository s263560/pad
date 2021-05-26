#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define tmat double
#define printf_tmat "%5.2lf\t"

int n;
int tam_mat;

tmat *A;
tmat *B;
tmat *C;
tmat *D;    // resultado do processamento serial
tmat *D2;   // resultado do processamento paralelo
tmat *Bt;   // matriz B transposta

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

// Assume que D = A * B + C = C + A * B
// então, inicializa o acúmulo da multiplicação com C

void processa(int tam, int mostra)
{
  long t1,t2;
  n = tam;
  tam_mat = (n*n);

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
  unsigned long long t_start = GetTimeStamp();

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

  // fim do processamento
  t1 = (long) (GetTimeStamp()-t_start);
  // -------------------------------
  // Faz o cálculo de forma paralela
  // -------------------------------
  // Pega tempo do início da execução
  t_start = GetTimeStamp();

  #pragma acc parallel loop copyin(A[0:tam_mat],Bt[0:tam_mat],C[0:tam_mat]), copyout(D2[0:tam_mat])
    for (int c=0; c<tam_mat; c++) {
      int linha=c/n;  
      int col=c%n;
      tmat S = C[c];
      tmat *pA = A + (linha*n);  // início da linha a somar na multiplicacao
      tmat *pBt = Bt + (col*n);  // início da linha transposta a somar na multiplicacao
      for (int k=0; k<n; k++)
        S += *(pA+k) * *(pBt+k);
      D2[c] = S;  
    }
  
  // fim do processamento
  t2 = (long) (GetTimeStamp()-t_start);
  
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
  if (erro) printf("--ERRO NO CALCULO!!!!!\n");

  if (mostra) 
  {
    mostra_matriz(A,"A");
    mostra_matriz(B,"B");
    mostra_matriz(C,"C");
    mostra_matriz(Bt,"B transposta");
    mostra_matriz(D,"D = A*B+C");
    mostra_matriz(D2,"D2 = A*B+C");
  }

  free(Bt);
  
  free(A);
  free(B);
  free(C);
  free(D);
  free(D2);
  return;
}


int main() 
{
   // teste do cálculo
   //processa(10,1);
 
   int tn[3] = { 10, 100, 1000 };
   for(int cn=0; cn<3; cn++)
     processa(tn[cn],0);
     
   return 0;
}
