/*
Criação de processos em UNIX, com impressão de valores de variável.

Compilar com gcc -Wall fork-print.c -o fork-print

Carlos Maziero, DINF/UFPR 2020

Exercício 3 - versão comentada.

Resumo: mostra que, após o fork, pai e filho têm espaços de memória
SEPARADOS. A variável x existe nos dois processos, mas cada um tem sua
própria cópia: alterar x no filho não afeta o x do pai.
*/

#include <unistd.h>     // fork(), getpid(), sleep()
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <sys/types.h>  // tipo pid_t
#include <sys/wait.h>   // wait()

int main ()
{
  int retval, x ;  // retval: retorno do fork(); x: variável de teste

  x = 0 ;          // x vale 0 ANTES do fork, no processo original

  // Cria o filho. O filho recebe uma CÓPIA da memória do pai, logo seu x
  // também vale 0 neste instante (mas é outra variável, em outro processo).
  retval = fork () ;

  // Executado pelos dois processos: ambos imprimem x = 0 (valor herdado).
  // A ordem das duas linhas depende do escalonador.
  printf ("No processo %5d x vale %d\n", getpid(), x) ;

  if ( retval < 0 )      // erro no fork()
  {
    perror ("Erro") ;
    exit (1) ;
  }
  else
    if ( retval > 0 )    // sou o processo pai
    {
      x = 0 ;            // atribui 0 ao x do PAI (já era 0: nada muda)
      wait (0) ;         // pai ESPERA o filho terminar
    }
    else                 // sou o processo filho
    {
      x++ ;              // incrementa apenas o x do FILHO: passa a valer 1
      sleep (5) ;        // filho dorme 5 s (ESPERA) antes de continuar
    }

  // Impressão final: o filho mostra x = 1 e o pai mostra x = 0, provando
  // que as memórias são independentes. O filho imprime primeiro (o pai só
  // acorda do wait depois que o filho termina).
  printf ("No processo %5d x vale %d\n", getpid(), x) ;
  exit (0) ;
}