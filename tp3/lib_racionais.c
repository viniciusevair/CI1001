#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib_racionais.h"

int aleat (int min, int max) {
    return rand() % (max - min + 1) + min;
}

racional *criar_r () {
    racional *r;
    if (! (r = malloc (sizeof (racional))))
        return NULL;

    return r;
}

racional *liberar_r (racional *r) {
    free (r);
    return NULL;
}

racional *sortear_r () {
    racional *r;
    r = criar_r ();
    r->num = aleat (-100, 100);
    r->den = aleat (-100, 100);

    simplifica_r (r);
    return r;
}

int ler_r (racional *r) {
    if (scanf ("%d %d", &r->num, &r->den) == 2 && valido_r (r))
        return 1;

    return 0;
}

void imprimir_r (racional *r) {
    if (r->den == 1)
        printf ("%d", r->num);
    else
        printf ("%d/%d", r->num, r->den);
}

int mdc (int a, int b) {
    if (b == 0)
        return a;

    return mdc (b, a % b);
}

int mmc (int a, int b) {
    return (a * b) / mdc (a, b);
}

int valido_r (racional *r) {
    return r->den != 0;
}

void simplifica_r (racional *r) {
    int m;
    if ((m = mdc (r->num, r-> den)) != 1) {
        r->num /= m;
        r->den /= m;
    }

    /* padroniza o sinal negativo no numerador para facilitar a visualizacao e
     * o calculo na funcao de comparar tamanho */
    if (r->den < 0) {
        r->num *= -1;
        r->den *= -1;
    }
}

int menor_r (racional *r1, racional *r2) {
    return r1->num * r2->den < r2->num * r1->den;
}

int iguais_r (racional *r1, racional *r2) {
    if (r1->den != r2->den || r1->num != r2->num)
        return 0;

    return 1;
}

racional *somar_r (racional *r1, racional *r2) {
    if (!valido_r (r2))
        return NULL;
    
    racional *rAux;
    rAux = criar_r ();

    rAux->den = mmc(r1->den, r2->den);
    rAux->num = r1->num * r2->den + r2->num * r1->den;
    simplifica_r (rAux);

    return rAux;
}

racional *subtrair_r (racional *r1, racional *r2) {
    if (!valido_r (r2))
        return NULL;

    racional *rAux;
    rAux = criar_r ();

    rAux->den = mmc(r1->den, r2->den);
    rAux->num = r1->num * r2->den - r2->num * r1->den;
    simplifica_r (rAux);

    return rAux;
}

racional *multiplicar_r (racional *r1, racional *r2) {
    if (!valido_r (r2))
        return NULL;

    racional *rAux;
    rAux = criar_r ();

    rAux->num = r1->num * r2->num;
    rAux->den = r1->den * r2->den;
    simplifica_r (rAux);

    return rAux;
}


racional *dividir_r (racional *r1, racional *r2) {
    if (r2->num == 0)
        return NULL;

    racional *rAux;
    rAux = criar_r ();

    rAux->num = r1->num * r2->den;
    rAux->den = r1->den * r2->num;
    simplifica_r (rAux);

    return rAux;
}
