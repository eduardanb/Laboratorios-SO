/*
 * Exercício 2 - Threads POSIX com pthread_join (thread-join.c)
 * Compilar: gcc -Wall thread-join.c -o thread-join -lpthread
 * Base: exemplo de Carlos Maziero (DINF/UFPR), comentado Maria Eduarda da Nóbrega.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 16

void *threadBody (void *id)
{
  long tid = (long) id ;                // número da thread recebido da main

  printf ("t%02ld: Olá!\n", tid) ;
  sleep (3) ;                           // simula trabalho / espera
  printf ("t%02ld: Tchau!\n", tid) ;
  pthread_exit (NULL) ;                 // termina; o estado fica disponível para o join
}

int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;
  pthread_attr_t attr ;                 // objeto de atributos das threads
  long i, status ;

  // inicializa attr com valores padrão e declara as threads como JOINABLE,
  // ou seja, a main poderá esperar por elas e recuperar seu término
  pthread_attr_init (&attr) ;
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ;

  // fase 1: cria todas as threads (passando attr)
  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: criando thread %02ld\n", i) ;
    status = pthread_create (&thread[i], &attr, threadBody, (void *) i) ;
    if (status)
    {
      perror ("pthread_create") ;
      exit (1) ;
    }
  }

  // fase 2: espera cada thread terminar, em ordem (0, 1, 2, ...)
  // pthread_join bloqueia a main até a thread indicada encerrar
  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: aguardando thread %02ld\n", i);
    status = pthread_join (thread[i], NULL) ;   // NULL: não queremos o valor de retorno
    if (status)
    {
      perror ("pthread_join") ;
      exit (1) ;
    }
  }
  printf ("Main: fim\n") ;               // só chega aqui depois de TODAS terminarem
  pthread_attr_destroy (&attr) ;         // libera o objeto de atributos
  pthread_exit (NULL) ;
}