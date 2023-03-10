/*
 * TAD fila
 * Autores:
 *    Marcos Alexandre Castilho
 *    Luis Carlos Erpen de Bona
 *
 * Primeira versão em 18/10/2019
 * Versao 1.0.0 de 10/11/2021
 * Versao 1.0.1 de 08/2022
 * Versao 2.0.0 de 14/12/2022
 */

struct nodo_f {
    int elem;
    struct nodo_f *prox;
};
typedef struct nodo_f nodo_f_t;

struct fila {
    nodo_f_t *ini;
    nodo_f_t *fim;
    int tamanho; /* numero de elementos na fila */
};
typedef struct fila fila_t;

/* Cria uma fila vazia e a retorna, se falhar retorna NULL. */
fila_t *cria_fila ();

/* Remove todos os elementos da fila, libera espaco e devolve NULL. */
fila_t *destroi_fila (fila_t *f);

/* Retorna 1 se a fila esta vazia e 0 caso contrario. */
int vazia_fila (fila_t *f);

/* Retorna o numero de elementos da fila. */
int tamanho_fila (fila_t *f);

/* Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario. */
int insere_fila (fila_t *f, int elemento);

/* Remove o elemento do inicio da fila (politica FIFO) e o retorna
 * no parametro *elemento. Retorna 1 se a operacao foi bem sucedida
 * e 0 caso contrario. */
int retira_fila (fila_t *f, int *elemento);

/* para depuracao */
void imprime_fila (fila_t *f);

