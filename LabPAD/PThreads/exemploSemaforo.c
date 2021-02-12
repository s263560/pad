/**
 * Programa desenvolvido para ilustrar a solucao do problema do
 * produtor/consumidor com o uso de threads e semaforos.
 *
 * Comentado por Prof. Andre Leon S. Gradvohl, Dr.
 *
 * Atualizado em: 03/08/2011
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 20
#define VEZES 60

struct timespec t;

sem_t  mutex, vazio, cheio;
//vazio: semaforo utilizado pra controlar os sinais relativos as posicoes vazias no buffer
//cheio: semaforo utilizado pra controlar os sinais relativos as posicoes preenchidas no buffer
//mutex: semaforo binÃ¡rio utilizado p/ garantir exclusao mutua na regiao critica   

int buffer[N], proxPosCheia, proxPosVazia, cont;
//buffer: utilizado para armazenar os dados produzidos pelo produtor e consumidos pelo consumidor
//proxPosCheia: proxima posicao cheia
//proxPosVazia: proxima posicao vazia
//cont: usado para controlar a quantidade de dados presentes no buffer

void *produtor(void *);
void *consumidor(void *);

int main(void)
{
	//Seta a semente da funcao geradora de numeros aleatorios
	srand(time(NULL));

	cont = 0;
	proxPosCheia = 0;
	proxPosVazia = 0;

	/*Inicializa os semaforos
	1o parametro: variavel semaforo
	2o parametro: indica se um semaforo sera compartilhado entre as threads de um processo ou entre processos
		      o valor 0 indica q/ o semaforo sera compartilhado entre as threads de um processo	(digit o comando
		      "man sem_init" no shell do linux p/ ver os detalhes)
	3o parametro: valor inicial do semaforo
	*/ 
	sem_init(&mutex, 0 , 1);
	sem_init(&vazio, 0, N);
	sem_init(&cheio, 0, 0);
	
	pthread_t thd0, thd1;

	/*
	Incializa as threads
	1o parametro: variavel thread
	2o parametro: indica se uma thread Ã© "joinable", ou seja, se a thread nao sera finalizada ate chegar a uma chamada de funcao
		      pthread_join().
	3o parametro: indica o nome do metodo que irÃ¡ compor o trecho de codigo q/ sera executado pela thread
	4o parametro: utilizado qdo se necessita passar algum paramentro a thread. Pode se passar quaisquer tipos de dados,
		      inclusive uma estrutura de dados qdo houver a necessidade de passar mais de um parametro.
		      (dentro do mÃ©todo chamado realiza-se um cast p/ recuperar os dados)	
	*/
	pthread_create(&thd0, 0, (void *) produtor, NULL);
	pthread_create(&thd1, 0, (void *) consumidor, NULL);

	//Esses dois metodos indicam q/ a tread nÃ£o serÃ¡ finalizada atÃ© ocorrer a chamada dos mesmos (como mencionado anteriormente)
	pthread_join(thd0,0);
	pthread_join(thd1,0);

        printf("\n");	
	exit(0);
}


// Metodo que produz os itens q/ serao inseridos no buffer (numeros aleatorios)
int produz_item()
{
	int val;
	val = rand() % 100;
	printf("\nProduzindo item: %d", val);
	return val;
}

// Metodo utilizado p/ mostra o valor q foi consumido (meramente implementado p/ fins didÃ¡ticos)
void consome_item(int item)
{
	printf("\nCosumindo item: %d", item);
}

//Metodo que a realiza a insercao do dado no buffer
void insere_item(int val)
{
	if(cont < N)
	{
		buffer[proxPosVazia] = val;
		// A utilizacao da divisao em modulo implementa um comportamento circular da utilizacao do buffer
		// ou seja, qdo o contador chegar no valor de N (N % N = 0) o valor da variavel voltara ao inicio do buffer
		proxPosVazia = (proxPosVazia + 1) % N; 
		cont = cont + 1;
		if(cont == N)
			printf("\n############## Buffer completo ##############");
	}
}

// Metodo que realiza a retirada do dado do buffer
int remove_item()
{
	int val;
	if(cont > 0)
	{
		val = buffer[proxPosCheia];
		proxPosCheia = (proxPosCheia + 1) % N;
		cont = cont - 1;
		return val;
	}
}

void *produtor(void *p_arg)
{
	int item;
	register int i=0;
	
	while(i++<VEZES)
	{
		item = produz_item();
		
		// sem_wait (realiza o down no semaforo (ver pag. 81 do livro Sistemas Operacionais - 2a ediÃ§ao - Tanenbaum ))
		// sem_post (realiza o up no semaforo)
		sem_wait(&vazio);
		sem_wait(&mutex);
		
		insere_item(item);

		sem_post(&mutex);
		sem_post(&cheio);

		sleep(item%2);
	}
}

void *consumidor(void *p_arg)
{
	int item;
	register int i=0;

        while(i++<VEZES)
	{
		sem_wait(&cheio);
		sem_wait(&mutex);

		item = remove_item();

		sem_post(&mutex);
		sem_post(&vazio);

		consome_item(item);
		
		sleep(item%3);
	}
}

