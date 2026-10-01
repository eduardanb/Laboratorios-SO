/*
Criação de processos em UNIX, com execução de outro binário

Compilar com gcc -Wall fork-execve.c -o fork-execve

Carlos Maziero, DINF/UFPR 2020

Exercício 2 - versão comentada.

Resumo: igual ao fork.c, mas em vez de dormir, o filho substitui sua
imagem de memória pelo programa /bin/date através de execve(). O pai
espera o filho terminar com wait().
*/

#include <unistd.h>     // fork(), execve(), getpid(), getppid()
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <sys/types.h>  // tipo pid_t
#include <sys/wait.h>   // wait()

// argc/argv: argumentos da linha de comando deste programa;
// envp: vetor com as variáveis de ambiente (terminado em NULL).
// Serão repassados ao programa executado pelo execve.
int main (int argc, char *argv[], char *envp[])
{
  int retval ;  // retorno do fork(): <0 erro, 0 filho, >0 pai (PID do filho)

  // Impresso apenas uma vez, pelo processo original.
  printf ("Ola, sou o processo %5d\n", getpid()) ;

  // Cria o processo filho (cópia do pai). A partir daqui há dois processos.
  retval = fork () ;

  // Impresso pelos dois processos. Para o pai, retval é o PID do filho;
  // para o filho, retval é 0 e getppid() é o PID do pai.
  printf ("[retval: %5d] sou %5d, filho de %5d\n", retval, getpid(), getppid()) ;

  if ( retval < 0 )       // erro no fork ()
  {
    perror ("Erro: ") ;   // mostra o motivo da falha
    exit (1) ;
  }
  else
    if ( retval > 0 )     // sou o processo pai
      wait (0) ;          // pai bloqueia (ESPERA) até o filho terminar
    else                  // sou o processo filho
    {
      // execve(caminho, argv, envp) SUBSTITUI o código, os dados e a pilha
      // do processo atual pelo programa /bin/date. O PID permanece o mesmo.
      // Se der certo, execve NUNCA retorna: o que vem depois (perror, printf
      // "Tchau", exit) deixa de existir no filho, que passa a executar o
      // date, imprime a data/hora e termina com exit do próprio date.
      execve ("/bin/date", argv, envp) ;

      // Só chega aqui se execve FALHOU (retornou -1), por exemplo, se o
      // arquivo não existe ou não tem permissão de execução. Então perror
      // informa o motivo, e o filho CONTINUA executando o código original
      // (cai no printf "Tchau" abaixo e termina com exit(0)).
      perror ("Erro") ;
    }

  // Caso normal (execve ok): só o PAI chega aqui, depois do wait; o filho
  // virou o /bin/date. Caso de falha do execve: pai e filho imprimem.
  printf ("Tchau de %5d!\n", getpid()) ;
  exit (0) ;
}