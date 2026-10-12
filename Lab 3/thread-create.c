/*
 * Exercício 1 - Criação de threads POSIX (thread-create.c)
 * Compilar: gcc -Wall thread-create.c -o thread-create -lpthread
 * Base: exemplo de Carlos Maziero (DINF/UFPR), comentado por Maria Eduarda da Nóbrega.
 */
#include <pthread.h>   // pthread_create, pthread_exit, pthread_t
#include <stdio.h>     // printf, perror
#include <stdlib.h>    // exit
#include <unistd.h>    // sleep

#define NUM_THREADS 16 // quantidade de threads que a main vai criar

// Corpo executado por cada thread. Recebe um ponteiro genérico (void *)
// que aqui carrega o número da thread, e devolve void * (nada, no caso).
void *threadBody (void *id)
{
  long tid = (long) id ;                // converte o argumento de volta para inteiro

  printf ("t%02ld: Olá!\n", tid) ;      // anuncia que a thread começou
  sleep (3) ;                           // bloqueia por 3 s: a CPU fica livre para as outras
  printf ("t%02ld: Tchau!\n", tid) ;    // anuncia que vai terminar
  pthread_exit (NULL) ;                 // encerra somente esta thread, sem valor de retorno
}

int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;      // vetor com os identificadores das threads
  long i, status ;                      // i: contador / id; status: retorno de pthread_create

  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: criando thread %02ld\n", i) ;
    // cria a thread: guarda o ID em thread[i], atributos padrão (NULL),
    // função a executar (threadBody) e argumento (o índice i, convertido p/ void *)
    status = pthread_create (&thread[i], NULL, threadBody, (void *) i) ;
    if (status)                         // retorno diferente de 0 indica erro
    {
      perror ("pthread_create") ;
      exit (1) ;
    }
  }
  printf ("Main: fim\n") ;
  // encerra só a thread main; o processo continua vivo até as demais terminarem.
  // (um return/exit aqui mataria o processo inteiro e as threads)
  pthread_exit (NULL) ;
}