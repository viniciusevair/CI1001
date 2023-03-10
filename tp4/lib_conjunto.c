/* Biblioteca para a TAD "Conjunto", implementada por Vinicius Evair da Silva,
 * a partir do header disponibilizado pelos professores da disciplina de 
 * Programacao 1 (CI1001), adaptado do livro Algoritmos e Estrutura de Dados 1:
 * CASTILHO, Marcos Alexandre; SILVA, Fabiano; WEINGAERTNER, Daniel. 
 * Algoritmos e estruturas de dados I. Universidade Federal do Paraná. 
 * ISBN: 978-65-86233-62-9. Curitiba, 2020. p. 291. */

#include <stdio.h>
#include <stdlib.h>
#include "lib_conjunto.h"

conjunto_t *cria_cjt (int max) {
   conjunto_t *c; 

   if (! (c = malloc (sizeof (conjunto_t))))
       return NULL;

   c->max = max;
   c->card = 0;

   if (! (c->v = calloc (c->max, sizeof (int))))
       return NULL;

   return c;
}

conjunto_t *destroi_cjt (conjunto_t *c) {
    while (! vazio_cjt (c)) {
        retira_um_elemento_cjt (c);
    }

    free (c->v);
    free (c);    
    return NULL;
}

int vazio_cjt (conjunto_t *c) {
    return cardinalidade_cjt (c) == 0;
}

int cardinalidade_cjt (conjunto_t *c) {
    return c->card;
}

/* funcao interna a ser usada nas funcoes "pertence_cjt" e
 * "retira_cjt" */
int busca_binaria (conjunto_t *c, int elemento, int ini, int fim) {
    if (fim < ini)
        return -1;
    
    int meio = (ini + fim) / 2;
    if (elemento == c->v[meio])
        return meio;

    if (elemento < c->v[meio])
        return busca_binaria (c, elemento, ini, meio - 1);
    return busca_binaria (c, elemento, meio + 1, fim);
}

int insere_cjt (conjunto_t *c, int elemento) {
    int i;
    i = cardinalidade_cjt (c) - 1;

    if (pertence_cjt (c, elemento)) {
        return 1;
    }

    if (cardinalidade_cjt (c) == c->max)
        return 0;
    
    while (i >= 0 && elemento < c->v[i]) {
        c->v[i+1] = c->v[i];
        i--;
    }

    c->v[i+1] = elemento;
    (c->card)++;

    return 1;
}

int retira_cjt (conjunto_t *c, int elemento) {
    int i;

    if (! pertence_cjt (c, elemento)) {
        return 0;
    }
    
    for (i = busca_binaria (c, elemento, 0, cardinalidade_cjt (c) - 1); i < cardinalidade_cjt (c) - 1; i++) {
        c->v[i] = c->v[i+1];
    }
    (c->card)--;

    return 1;
}

int pertence_cjt (conjunto_t *c, int elemento) {
    int indice;
    
    if ((indice = busca_binaria (c, elemento, 0, cardinalidade_cjt (c) - 1)) < 0)
        return 0;

    if (c->v[indice] == elemento) {

        return 1;
    }

    return 0;
}

int contido_cjt (conjunto_t *c1, conjunto_t *c2) {
    int i;
    int j;

    if (cardinalidade_cjt (c1) > cardinalidade_cjt (c2))
        return 0;

    i = 0;
    j = 0;    
    while (i < cardinalidade_cjt (c1) && j < cardinalidade_cjt (c2)) {
        if (c1->v[i] < c2->v[j])
            return 0;

        if (c1->v[i] > c2->v[j])        
            j++;
        else {
            i++;
            j++;
        }
    }

    return i == cardinalidade_cjt (c1);
}

int sao_iguais_cjt (conjunto_t *c1, conjunto_t *c2) {
    int i;

    if (cardinalidade_cjt (c1) != cardinalidade_cjt (c2))
        return 0;

    i = 0;
    while (i < cardinalidade_cjt (c1) && c1->v[i] == c2->v[i])
        i++;

    return i == cardinalidade_cjt (c1);
}

conjunto_t *diferenca_cjt (conjunto_t *c1, conjunto_t *c2) {
    int i, j, k;
    conjunto_t *dif;

    i = 0;
    j = 0;
    k = -1;    
    dif = cria_cjt (cardinalidade_cjt (c1) + cardinalidade_cjt (c2));
    
    while (i < cardinalidade_cjt (c1) && j < cardinalidade_cjt (c2))
        if (c1->v[i] < c2->v[j]) {
            k++;
            dif->v[k] = c1->v[i];
            i++;
        }
        else {
            if (c1->v[i] < c2->v[j])
                j++;
            else {
                i++;
                j++;
            }
        }

    for (j = i; j < cardinalidade_cjt (c1); j++) {
        k++;
        dif->v[k] = c1->v[j];
    }

    dif->card = k + 1;

    return dif;
}

conjunto_t *interseccao_cjt (conjunto_t *c1, conjunto_t *c2) {
    int i, j, k;
    conjunto_t *intersec;

    i = 0;
    j = 0;
    k = 0;
    intersec = cria_cjt (cardinalidade_cjt (c1) + cardinalidade_cjt (c2));

    while (i < cardinalidade_cjt (c1) && j < cardinalidade_cjt (c2))
        if (c1->v[i] < c2->v[j])
            i++;
        else {
            if (c1->v[i] > c2->v[j])
                j++;
            else {
                intersec->v[k] = c1->v[i];
                k++;
                i++;
                j++;
            }
        }

    intersec->card = k;
    return intersec;
}

conjunto_t *uniao_cjt (conjunto_t *c1, conjunto_t *c2) {
    int i, j, k, l;
    conjunto_t *u;

    i = 0;
    j = 0;
    k = -1;
    u = cria_cjt (cardinalidade_cjt (c1) + cardinalidade_cjt (c2));
    /* Enquanto ambos os conjuntos possuirem elementos que ainda nao foram
     * passados para o conjunto uniao, o loop abaixo escolhe sempre o menor
     * elemento disponivel, mantendo a ordenacao. O loop para quando o menor
     * dos conjuntos nao possuir mais elementos para copiar. */
    while (i < cardinalidade_cjt (c1) && j < cardinalidade_cjt (c2)) {
        k++;

        /* Se o proximo menor elemento ainda nao copiado pertencia ao c1, 
         * avanca apenas o indice i. */
        if (c1->v[i] < c2->v[j]) {
            u->v[k] = c1->v[i];
            i++;
        }
        else {
            /* Se o proximo menor elemento ainda nao copiado pertencia ao c2,
             * avanca apenas o indice j. */
            if (c1->v[i] > c2->v[j]) {
                u->v[k] = c2->v[j];
                j++;
            }
            /* Caso o proximo menor elemento pertenca a ambos os conjuntos
             * basta copia-lo para o conjunto uniao e avancar ambos
             * os indices i e j. */
            else {
                u->v[k] = c1->v[i];
                i++;
                j++;
            }
        }
    }

    /* Os loops abaixo garantem que o conjunto de maior cardinalidade copiara
     * ao conjunto uniao o restante de seus elementos. Apenas um dos loops
     * abaixo acontecerao. */
    for (l = i; l < cardinalidade_cjt (c1); l++) {
        k++;
        u->v[k] = c1->v[l];
    }
    for (l = j; l < cardinalidade_cjt (c2); l++) {
        k++;
        u->v[k] = c2->v[l];
    }

    u->card = k + 1;

    return u;
}

conjunto_t *copia_cjt (conjunto_t *c) {
    int i; 
    conjunto_t *c2;

    c2 = cria_cjt (c->max);
    c2->card = cardinalidade_cjt (c);

    for (i = 0; i < cardinalidade_cjt (c); i++)
        c2->v[i] = c->v[i];

    return c2; /* Devolve NULL caso a criacao tenha falhado */
}

/* Funcao interna da biblioteca, retorna um inteiro aleatorio dentro de um
 * intervalo */
int aleat (int min, int max) {
    return rand() % (max - min + 1) + min;
}

/* Funcao interna da biblioteca, ordena o subconjunto criado com indices
 * aleatorios */
conjunto_t *ordenar_conjunto (conjunto_t *c) {
    int i, j, aux;

    for (i = 1; i < cardinalidade_cjt (c); i++) {
        aux = c->v[i];
        j = i - 1;

        while (j >= 0 && c->v[j] > aux) {
            c->v[j+1] = c->v[j];
            j--;
        }

        c->v[j+1] = aux;
    }

    return c;
}

/* Refazer depois, fazendo uma copia do conjunto original e randomicamente
 * removendo valores de dentro da copia até que a copia tenha 
 * o tamanho desejado */
conjunto_t *cria_subcjt_cjt (conjunto_t *c, int n) {
    conjunto_t *subc;

    if (vazio_cjt (c)) {
        return cria_cjt (c->max);
    }

    if (n >= cardinalidade_cjt (c))
        return copia_cjt (c);

    subc = copia_cjt (c);

    while (cardinalidade_cjt (subc) > n) {
        subc->v[aleat (0, cardinalidade_cjt (subc))] = subc->v[cardinalidade_cjt (subc) - 1];
        (subc->card)--;
    }
   
    return ordenar_conjunto (subc);
}

void imprime_cjt (conjunto_t *c) {
    int i, elemento;

    if (vazio_cjt (c)) {
        printf ("conjunto vazio\n");
        return;
    }

    inicia_iterador_cjt (c);
    for (i = 0; i < cardinalidade_cjt (c) - 1; i++) {
        incrementa_iterador_cjt (c, &elemento);
        printf ("%d ", elemento);
    }
    incrementa_iterador_cjt (c, &elemento);
    printf ("%d\n", elemento);
}

void inicia_iterador_cjt (conjunto_t *c) {
    c->ptr = 0;
}

int incrementa_iterador_cjt (conjunto_t *c, int *ret_iterador) {
    if (c->ptr >= cardinalidade_cjt (c))
        return 0;

    *ret_iterador = c->v[c->ptr];
    (c->ptr)++;

    return 1;
}

int retira_um_elemento_cjt (conjunto_t *c) {
    (c->card)--;
    return c->v[cardinalidade_cjt (c)];
}
