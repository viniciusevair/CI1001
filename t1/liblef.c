#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblef.h"

lef_t *cria_lef () {
    lef_t *l;
        
    if (! (l = malloc (sizeof (lef_t))))
        return NULL;

    l->Primeiro = NULL;

    return l;
}

lef_t *destroi_lef (lef_t *l) {
    while (l->Primeiro != NULL) {
        free (obtem_primeiro_lef (l));
    }    
    
    free (l);    

    return NULL;
}

int adiciona_inicio_lef (lef_t *l, evento_t *evento) {
    nodo_lef_t *novo;
    if (! (novo = malloc (sizeof (nodo_lef_t))))
        return 0;
    if (! (novo->evento = malloc (sizeof (evento_t)))) {
        free (novo);
        return 0;
    }

    memcpy (novo->evento, evento, sizeof (evento_t));
    
    if (l->Primeiro == NULL) {    
        novo->prox = NULL;
        l->Primeiro = novo;

        return 1;
    }

    novo->prox = l->Primeiro;
    l->Primeiro = novo;

    return 1;
}

int adiciona_ordem_lef (lef_t *l, evento_t *evento) {
    nodo_lef_t *novo, *atual;
    if (! (novo = malloc (sizeof (nodo_lef_t))))
        return 0;
    if (! (novo->evento = malloc (sizeof (evento_t)))) {
        free (novo);
        return 0;
    }
    novo->prox = NULL;

    memcpy (novo->evento, evento, sizeof (evento_t));

    /*
     * Casos separados: se nao existe nenhum evento, ou se o tempo do novo evento
     * eh menor que o tempo do proximo a ser chamado, adiciona o novo evento
     * na primeira posicao.
     */
    if (l->Primeiro == NULL) {    
        novo->prox = NULL;
        l->Primeiro = novo;

        return 1;
    }

    if (l->Primeiro->evento->tempo > novo->evento->tempo) {
        novo->prox = l->Primeiro;
        l->Primeiro = novo;
        
        return 1;
    }

    /* 
     * Caso geral: caminha pela lista com um nodo ponteiro ate encontrar 
     * dois eventos consecutivos ev1 e ev2 tais que 
     * tempo ev1 < tempo novo evento < tempo ev2,
     * e adiciona o novo evento entre tais eventos.
     */
    atual = l->Primeiro;
    while (atual->prox != NULL && novo->evento->tempo > atual->prox->evento->tempo)
        atual = atual->prox;

    novo->prox = atual->prox;
    atual->prox = novo;

    return 1;
}

evento_t *obtem_primeiro_lef (lef_t *l) {
    nodo_lef_t *nodo_temp;
    evento_t *evento_temp;

    if (l->Primeiro == NULL)
        return NULL;

    nodo_temp = l->Primeiro;
    l->Primeiro = l->Primeiro->prox;
    nodo_temp->prox = NULL;

    evento_temp = nodo_temp->evento;
    free (nodo_temp);

    return evento_temp;
}
