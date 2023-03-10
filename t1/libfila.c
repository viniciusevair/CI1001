#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila_t *cria_fila () {
    fila_t *f;
    if (! (f = malloc (sizeof (fila_t))))
        return NULL;

    f->ini = NULL;
    f->fim = NULL;
    f->tamanho = 0;

    return f;
}

fila_t *destroi_fila (fila_t *f) {
    int elem;

    while (! vazia_fila(f)) {
        retira_fila(f, &elem);
    }

    free (f);
    return NULL;
}

int vazia_fila (fila_t *f) {
    return f->ini == NULL;
}

int tamanho_fila (fila_t *f) {
    return f->tamanho;
}

int insere_fila (fila_t *f, int elemento) {
    nodo_f_t *novo; 

    if (! (novo = malloc (sizeof (nodo_f_t))))
        return 0;

    novo->elem = elemento;
    novo->prox = NULL;

    if (vazia_fila(f)) {
        f->ini = novo;
        f->fim = novo;
        (f->tamanho)++;

        return 1;
    }

    f->fim->prox = novo;
    f->fim = novo;
    (f->tamanho)++;

    return 1;
}

int retira_fila (fila_t *f, int *elemento) {
    nodo_f_t *aux;

    if (vazia_fila(f))
        return 0;

    *elemento = f->ini->elem;
    aux = f->ini;
    f->ini = f->ini->prox;

    (f->tamanho)--;
    free (aux);

    return 1;
}

void imprime_fila (fila_t *f) {
    nodo_f_t *imp;
    
    if (vazia_fila(f)) {
        printf ("Fila vazia\n");
        return;
    }

    imp = f->ini;
    imp->prox = f->ini->prox;

    while (imp != NULL) {
        printf ("%d ", imp->elem);
        imp = imp->prox;
    }
    printf("\n");
}
