#include "moveToFrontADT.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct node
{
    elemType elem;
    struct node * tail;

}TNode;

typedef TNode * TList;

typedef struct moveToFrontCDT
{
    TList l;
    compare cmp;
    size_t size;
    TList next;

}moveToFrontCDT;

/* Retorna un nuevo conjunto de elementos genéricos. Al inicio está vacío */
moveToFrontADT newMoveToFront(compare cmp)
{
    moveToFrontADT ans = calloc(1, sizeof(moveToFrontCDT));
    ans->cmp = cmp;
    return ans;
}

void freeList(TList l)
{
    if(l == NULL)
        return ;
    freeList(l->tail);
    free(l);
}

/* Libera toda la memoria reservada por el TAD */
void freeMoveToFront(moveToFrontADT moveToFront)
{
    freeList(moveToFront->l);
    free(moveToFront);
}
 

TList addRec(TList l,  elemType elem, compare cmp, int * flag)
{
    if(l == NULL)
    {
        TList aux = malloc(sizeof(TNode));
        aux->elem = elem;
        aux->tail = l;
        *flag = 1;
        return aux;
    }
    if(cmp(l->elem , elem) != 0)
        l->tail = addRec(l->tail , elem, cmp, flag);
    return l;
    
}
/* Inserta un elemento si no está. Lo inserta al final.
** Retorna 1 si lo agregó, 0 si no.
*/
unsigned int add(moveToFrontADT moveToFront, elemType elem)
{
    int flag = 0;
    moveToFront->l = addRec(moveToFront->l, elem, moveToFront->cmp, &flag);
    moveToFront->size += flag;
    return flag;
}
  
/* Retorna la cantidad de elementos que hay en la colección */
unsigned int size(moveToFrontADT moveToFront)
{
    return moveToFront->size;
}
 
/* Se ubica al principio del conjunto, para poder iterar sobre el mismo */
void toBegin(moveToFrontADT moveToFront)
{
    moveToFront->next = moveToFront->l;
}

/* Retorna 1 si hay un elemento siguiente en el iterador, 0 si no */
int hasNext(moveToFrontADT moveToFront)
{
    return (moveToFront->next != NULL);
}

/* Retorna el siguiente elemento. Si no hay siguiente elemento, aborta */
elemType next(moveToFrontADT moveToFront)
{
    if(moveToFront->next == NULL)
        exit(1);
    TList aux = moveToFront->next;
    moveToFront->next = moveToFront->next->tail;
    return aux->elem;
}

// busca el elemento y de encontrarlo lo elimina para ponerlo al principio
TList find(TList l, elemType elem, compare cmp, int* flag, elemType * aux)
{
    if(l == NULL)
        return l;
    if(cmp(l->elem, elem) == 0)
    {
        *flag = 1;
        *aux = l->elem;
        TList new = l->tail;
        free(l);
        return find(new, elem, cmp, flag, aux);
    }
    l->tail = find(l->tail, elem, cmp, flag, aux);
    return l;
}


TList getRec(TList l, elemType elem)
{
    TList aux = malloc(sizeof(TNode));
    aux->elem = elem;
    aux->tail = l;
    return aux;
}

/* Retorna una copia del elemento. Si no existe retorna NULL.
** Para saber si el elemento está, usa la función compare. 
** Si el elemento estaba lo ubica al principio.
 */
elemType * get(moveToFrontADT moveToFront, elemType elem)
{
    int flag = 0;
    elemType aux;
    if(moveToFront->cmp(moveToFront->l->elem , elem) != 0)
    {    
        moveToFront->l = find(moveToFront->l, elem, moveToFront->cmp, &flag, &aux);
        if(flag)
        {
            moveToFront->l = getRec(moveToFront->l, aux);
        }
        else
            return NULL;    
       
    }
    elemType * ans = malloc(sizeof(elem));
    *ans = moveToFront->l->elem;
    return ans;


}