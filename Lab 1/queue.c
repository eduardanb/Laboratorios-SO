// PingPongOS - PingPong Operating System
// Implementação de fila genérica (lista circular duplamente encadeada)
// conforme a interface definida em queue.h.
//
// Esta biblioteca apenas manipula ponteiros: não aloca nem libera memória.

#include <stdio.h>
#include "queue.h"

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue)
{
   queue_t *aux ;
   int count ;

   // fila vazia
   if (!queue)
      return 0 ;

   // percorre a fila circular até voltar ao primeiro elemento
   count = 0 ;
   aux = queue ;
   do
   {
      count++ ;
      aux = aux->next ;
   }
   while (aux && aux != queue) ;

   return count ;
}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca.

void queue_print (char *name, queue_t *queue, void print_elem (void*) )
{
   queue_t *aux ;

   printf ("%s: [", name ? name : "") ;

   if (queue && print_elem)
   {
      aux = queue ;
      do
      {
         print_elem ((void *) aux) ;
         aux = aux->next ;
         if (aux && aux != queue)
            printf (" ") ;
      }
      while (aux && aux != queue) ;
   }

   printf ("]\n") ;
}

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
// Retorno: 0 se sucesso, <0 se ocorreu algum erro

int queue_append (queue_t **queue, queue_t *elem)
{
   queue_t *first, *last ;

   // a fila deve existir
   if (!queue)
   {
      fprintf (stderr, "### Erro (queue_append): a fila nao existe\n") ;
      return -1 ;
   }

   // o elemento deve existir
   if (!elem)
   {
      fprintf (stderr, "### Erro (queue_append): o elemento nao existe\n") ;
      return -2 ;
   }

   // o elemento deve estar isolado (nao pode pertencer a nenhuma fila)
   if (elem->prev || elem->next)
   {
      fprintf (stderr, "### Erro (queue_append): o elemento ja pertence a uma fila\n") ;
      return -3 ;
   }

   // fila vazia: o elemento passa a ser o único, apontando para si mesmo
   if (!(*queue))
   {
      elem->next = elem ;
      elem->prev = elem ;
      *queue = elem ;
      return 0 ;
   }

   // fila não vazia: insere entre o último elemento e o primeiro
   first = *queue ;
   last  = first->prev ;

   elem->next  = first ;
   elem->prev  = last ;
   last->next  = elem ;
   first->prev = elem ;

   return 0 ;
}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: 0 se sucesso, <0 se ocorreu algum erro

int queue_remove (queue_t **queue, queue_t *elem)
{
   queue_t *aux ;
   int found ;

   // a fila deve existir
   if (!queue)
   {
      fprintf (stderr, "### Erro (queue_remove): a fila nao existe\n") ;
      return -1 ;
   }

   // a fila nao deve estar vazia
   if (!(*queue))
   {
      fprintf (stderr, "### Erro (queue_remove): a fila esta vazia\n") ;
      return -2 ;
   }

   // o elemento deve existir
   if (!elem)
   {
      fprintf (stderr, "### Erro (queue_remove): o elemento nao existe\n") ;
      return -3 ;
   }

   // o elemento deve pertencer a fila indicada: percorre a fila procurando-o
   found = 0 ;
   aux = *queue ;
   do
   {
      if (aux == elem)
      {
         found = 1 ;
         break ;
      }
      aux = aux->next ;
   }
   while (aux && aux != *queue) ;

   if (!found)
   {
      fprintf (stderr, "### Erro (queue_remove): o elemento nao pertence a fila indicada\n") ;
      return -4 ;
   }

   if (elem->next == elem)
   {
      // único elemento da fila: a fila fica vazia
      *queue = NULL ;
   }
   else
   {
      // religa os vizinhos, retirando o elemento do meio
      elem->prev->next = elem->next ;
      elem->next->prev = elem->prev ;

      // se o removido era o primeiro, o próximo passa a ser o primeiro
      if (*queue == elem)
         *queue = elem->next ;
   }

   // isola o elemento removido
   elem->prev = NULL ;
   elem->next = NULL ;

   return 0 ;
}