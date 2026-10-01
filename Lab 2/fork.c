/*
Criação de processos em UNIX.

Compilar com gcc -Wall fork.c -o fork

Carlos Maziero, DINF/UFPR 2020

Exercício 1 - versão comentada.

Resumo: o programa imprime seu PID, cria um processo filho com fork(),
ambos imprimem uma linha identificando-se, o pai espera o filho terminar
(wait), o filho dorme 5 segundos, e por fim ambos imprimem "Tchau" e
terminam.
*/

#include <unistd.h>     // fork(), getpid(), getppid(), sleep()
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <sys/types.h>  // tipo pid_t
#include <sys/wait.h>   // wait()

int main ()
{
  int retval ;  // guarda o valor de retorno do fork():
                //   < 0 : erro (filho não foi criado)
                //   = 0 : estamos executando no processo FILHO
                //   > 0 : estamos no processo PAI; valor = PID do filho

  // Executado apenas uma vez, pelo processo original (futuro pai).
  // getpid() devolve o PID do processo que está executando a chamada.
  printf ("Ola, sou o processo %5d\n", getpid()) ;

  // fork() cria um NOVO processo que é uma cópia do atual (mesmo código,
  // cópia da pilha, dados e registradores, inclusive o ponto de execução).
  // A partir daqui existem DOIS processos executando a próxima linha:
  // o pai recebe o PID do filho em retval; o filho recebe 0.
  retval = fork () ;

  // Esta linha é executada pelos dois processos (cada um com seu retval e
  // seu getpid()). getppid() devolve o PID do pai: para o filho é o PID do
  // processo original; para o pai é o PID do shell (ou quem o executou).
  // A ordem entre as duas impressões depende do escalonador (não é
  // determinística).
  printf ("[retval: %5d] sou %5d, filho de %5d\n", retval, getpid(), getppid()) ;

  if ( retval < 0 )    // erro no fork() (ex.: limite de processos atingido)
  {
    perror ("Erro") ;  // imprime a mensagem de erro associada a errno
    exit (1) ;         // termina com código de saída 1 (falha)
  }
  else
    if ( retval > 0 )  // sou o processo pai
      wait (0) ;       // bloqueia o pai até que o filho termine (evita que
                       // o filho vire "zumbi" e garante que o pai só termine
                       // depois do filho). O pai fica em ESPERA (~5 s).
    else               // sou o processo filho (retval == 0)
      sleep (5) ;      // filho suspende-se por 5 segundos (ESPERA); depois
                       // segue para o printf final

  // Executado pelos dois processos: o filho após 5 s de sleep, e o pai logo
  // após o filho terminar (o wait retorna quando o filho executa exit).
  // Portanto "Tchau" do filho sempre aparece ANTES do "Tchau" do pai.
  printf ("Tchau de %5d!\n", getpid()) ;
  exit (0) ;           // término normal (código 0)
}