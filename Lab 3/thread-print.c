/*
 * Exercício 3 - Threads POSIX com variável global compartilhada (thread-print.c)
 * Compilar: gcc -Wall thread-print.c -o thread-print -lpthread
 * Base: exemplo de Carlos Maziero (DINF/UFPR), comentado por Maria Eduarda da Nóbrega.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 16

int x = 0 ;   // variável GLOBAL: fica no espaço de endereçamento do processo,
              // portanto é a MESMA para todas as threads (compartilhada)

void *threadBody (void *id)
{
  long tid = (long) id ;

  x++ ;                                          // incrementa o x compartilhado
  printf ("t%02ld: Olá!   (x=%02d)\n", tid, x) ;
  sleep (3) ;                                    // enquanto dorme, as outras threads rodam
  x++ ;                                          // incrementa de novo
  printf ("t%02ld: Tchau! (x=%02d)\n", tid, x) ;
  pthread_exit (NULL) ;
}

int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;
  long i, status ;

  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: criando thread %02ld\n", i) ;
    status = pthread_create (&thread[i], NULL, threadBody, (void *) i) ;
    if (status)
    {
      perror ("pthread_create") ;
      exit (1) ;
    }
  }
  printf ("Main: fim\n") ;
  pthread_exit (NULL) ;
}