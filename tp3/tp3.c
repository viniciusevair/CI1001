#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib_racionais.h"
#define MAX 100

/* le um inteiro na faixa [0..MAX-1] */
int ler_tamanho () {
    int i;
    while ((scanf ("%d", &i)) != 1 || i >= MAX || i < 0)
        printf ("Valor invalido, tente novamente com um valor entre 0 e %d\n",
                MAX-1);

    return i;
}

/* imprime os racionais apontados pelo vetor de ponteiros para racionais */
void imprimir_vetor_racional (racional **v, int tam) {
    int i;

    for (i = 0; i < tam; i++) {
        imprimir_r (v[i]);
        printf (" ");
    }
    printf ("\n");
}

/* retorna um vetor de tam ponteiros para numeros racionais validos gerados aleatoriamente */
/* retorna NULL em caso de falha                                                           */
racional** aleatorio_vetor_racional (racional **v, int tam) {
    int i;

    for (i = 0; i < tam; i++) {
        v[i] = sortear_r ();
        if (! valido_r (v[i]))
            return NULL;
    }

    return v;
}

/* faz uma copia em w dos racionais apontados pelo vetor v */
void copia_vetor_racional (racional **w, racional**v, int tam) {
    int i;

    for (i = 0; i < tam; i++)
        w[i] = v[i];
}

/* retorna um vetor de tam ponteiros para numeros racionais que apontam em ordem crescente para os 
 * racionais apontados pelo vetor recebido no parametro. Defina outras funcoes para te ajudar caso
 * ache necessario */

/* algoritmo de ordenacao por insercao, adaptado do livro Algoritmos e 
 * Estrutura de Dados 1:
 * CASTILHO, Marcos Alexandre; SILVA, Fabiano; WEINGAERTNER, Daniel. 
 * Algoritmos e estruturas de dados I. Universidade Federal do Paraná. 
 * ISBN: 978-65-86233-62-9. Curitiba, 2020. p. 200. */
racional** ordenar_vetor_racional (racional **v, int tam) {
    int i, j;
    racional *aux;

    for (i = 1; i < tam; i++) {
        aux = v[i];
        j = i - 1;

        while (j >= 0 && menor_r(aux, v[j])) {
            v[j+1] = v[j];
            j--;
        }

        v[j+1] = aux;
    }

    return v;
}

racional** liberar_vetor (racional **v, int tam) {
    int i;

    for (i = 0; i < tam; i++)
        v[i] = liberar_r (v[i]);

    free (v);    
    return NULL;
}

int main ()
{
    /* inicializa semente randomica                            */
    /* o parametro eh 0 para facilitar os testes               */
    /* depois pode trocar para, por exemplo, srand (time (0)); */
    srand (time (NULL));

    racional **v, **w;
    /* v e w são vetores de ponteiros para racionais (racional *)
       alternativamente poderia ser declarado como racional *v[] */
    
    /* ler o tamanho do vetor de racionais */
    int tam = ler_tamanho ();

    v = calloc (tam, sizeof (racional*));
    w = calloc (tam, sizeof (racional*));

    /* aloca v com tam ponteiros para racional */
    aleatorio_vetor_racional (v, tam);
    /* lembre-se que a funcao acima retorna NULL em caso de falha */

    copia_vetor_racional (w, v, tam);
    
    /* chama a funcao para ordenar o vetor */
    w = ordenar_vetor_racional (w, tam);
    /*  lembre-se que a funcao acima retorna 0 em caso de falha */

    /* imprime o vetor ordenado */
    imprimir_vetor_racional (w, tam);

    /* libera toda memoria alocada dinamicamente        */
    /* nao esqueca de testar com valgrind para conferir */
    v = liberar_vetor (v, tam);
    free (w);
    w = NULL;

    return 0;
}
