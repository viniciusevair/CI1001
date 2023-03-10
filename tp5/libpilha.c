#include <stdio.h>
#include <stdlib.h>
#include "libpilha.h"

t_pilha* cria_pilha(int n) {
    t_pilha *p;

    if (! (p = malloc (sizeof(t_pilha))))
        return NULL;

    p->tam = n;
    p->topo = 0;

    if (! (p->v = calloc (n, sizeof(double)))) {
        free (p);
        return NULL;
    }
    
    return p;
}

t_pilha* destroi_pilha(t_pilha *p) {
    free (p->v);
    free (p);

    return NULL;
}

int pilha_vazia(t_pilha *p) {
    return p->topo == 0;
}

int empilha (double x, t_pilha *p) {
    if (p->tam == p->topo)
        return 0;

    (p->topo)++;
    p->v[p->topo - 1] = x;

    return 1;
}

int desempilha(double *t, t_pilha *p) {
    if (pilha_vazia(p))
        return 0;

    *t = p->v[p->topo - 1];
    (p->topo)--;

    return 1;
}

int topo(double *t, t_pilha *p) {
    if (pilha_vazia(p))
        return 0;

    *t = p->v[p->topo - 1];

    return 1;
}

