#include <stdio.h>
#include <stdlib.h>
#include "lib_conjunto.h"
#define MAX 100

/* Faz a liberacao da memoria alocada para cada indice do vetor de conjuntos.
 * Deixa a liberacao da memoria do vetor para a main. */
void liberar_vetor_conjuntos (conjunto_t **v, int tam) {
    int i;

    for (i = 0; i < tam; i++)
        destroi_cjt (v[i]);
}

/* Cria um conjunto vazio com no maximo "tam" espacos e em seguida faz a
 * leitura de elementos para o conjunto. Retorna o conjunto lido em
 * caso de sucesso e retorna NULL em caso de falha na criacao do conjunto. */
conjunto_t *ler_conjunto (int tam) {
    conjunto_t *c;
    int elemento;

    if (! (c = cria_cjt (tam)))
        return NULL;

    while (scanf("%d", &elemento) == 1 && elemento != 0)
        if (! insere_cjt (c, elemento)) {
            printf ("Limite de elementos alcancado, ");
            printf ("termine a leitura do conjunto.\n");
        }

    return c;
}

/* Faz a leitura de uma sequencia de conjuntos. Retorna 1 em caso de sucesso,
 * retorna 0 em caso de falha da criacao de algum conjunto. */
int ler_vetor_conjuntos (conjunto_t **v, int tam) {
    int i;
    /* Inicializa o indice em -1 por conta da estrutura 
     * do loop logo a seguir. */
    i = -1;

    do {
        i++;
        if (! (v[i] = ler_conjunto (tam)))
            return -1;
    } while (! vazio_cjt (v[i]));

    /* Chama a funcao para destruir o conjunto vazio criado na iteracao de
     * saida do loop */
    destroi_cjt (v[i]);
    return i;
}

int main () {
    conjunto_t **herois, **equipes;
    int herois_tam, equipes_tam, i, j, distintivo;
    conjunto_t *missao, *uniao_anterior, *uniao, *solucao_anterior, *solucao;

    /* ler os herois e suas habilidades */
    if (! (herois = calloc (MAX, sizeof (conjunto_t*))))
        return 0;

    while ((herois_tam = ler_vetor_conjuntos (herois, 10)) < 0) {
        liberar_vetor_conjuntos (herois, herois_tam);
        printf ("Houve algum problema com a leitura, ");
        printf ("recomece a entrada de dados dos herois.\n");
    }

    /* ler a missao */
    missao = ler_conjunto (MAX);

    /* ler as equipes de herois */
    if (! (equipes = calloc (MAX, sizeof (conjunto_t*))))
        return 0;

    while ((equipes_tam = ler_vetor_conjuntos (equipes, herois_tam)) == 0) {
        liberar_vetor_conjuntos (equipes, equipes_tam);
        printf ("Houve algum problema com a leitura, ");
        printf ("recomece a entrada de dados das equipes.\n");
    }

    /* a solucao eh encontrada se a missao esta contido na uniao das 
     * habilidades de uma equipe, mas tomando-se aquela de menor tamanho. */

    if (! (solucao = cria_cjt (herois_tam)))
        return 0;

    /* Loop externo, caminha pelas equipes */
    for (i = 0; i < equipes_tam; i++) {
        if (! (uniao = cria_cjt (MAX)))
            return 0;

        inicia_iterador_cjt (equipes[i]);
        /* Loop interno, constroi a uniao de habilidades da equipe */
        for (j = 0; j < cardinalidade_cjt (equipes[i]); j++) {
            uniao_anterior = uniao;

            /* Distintivo representa o numero de identificacao do heroi. */
            /* Subtrai-se 1 do valor, por conta de vetores iniciarem 
             * no indice 0 em C. */
            incrementa_iterador_cjt (equipes[i], &distintivo);
            uniao = uniao_cjt (uniao, herois[distintivo - 1]);
            uniao_anterior = destroi_cjt (uniao_anterior);
        }

        /* Se a equipe tem as habilidades necessarias, e eh menor que uma equipe
         * anteriormente elegivel para solucao, atribui a nova equipe para a
         * solucao. */
        if (contido_cjt (missao, uniao))
            if (vazio_cjt (solucao) || cardinalidade_cjt (equipes[i]) < cardinalidade_cjt (solucao)) {
                solucao_anterior = solucao;
                solucao = copia_cjt (equipes[i]);
                solucao_anterior = destroi_cjt (solucao_anterior);
            }

        uniao = destroi_cjt (uniao);
    }

    if (vazio_cjt (solucao))
        printf ("NENHUMA\n");
    else
        imprime_cjt (solucao);

    /* libera toda a memoria alocada dinamicamente */
    liberar_vetor_conjuntos (herois, herois_tam);
    free (herois);
    herois = NULL;
    destroi_cjt (missao);
    missao = NULL;
    liberar_vetor_conjuntos (equipes, equipes_tam);
    free (equipes);
    equipes = NULL;
    destroi_cjt (solucao);

    return 0;
}
