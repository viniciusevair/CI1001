#include <stdio.h>
#include "lib_racionais.h"

/* calcula o MDC pelo metodo de Euclides e o devolve como um valor inteiro */
int calculaMDC (int n, int m) {
    int resto = n%m;

    while (resto != 0) {
        n = m;
        m = resto;
        resto = n%m;
    }

    return m;
}

/* simplifica a fracao do racional, dividindo o numerador e o denominador pelo
 * MDC entre eles, devolve o racional simplificado */
tRacional simplificaRacional (tRacional n) {
    int mdc;

    if ((mdc = calculaMDC (n.num, n.den)) != 1) {
        n.num /= mdc;
        n.den /= mdc;
    }

    return n;
}

tRacional divRacionais (tRacional rac1, tRacional rac2) {
    tRacional n;

    n.num = rac1.num * rac2.den;
    n.den = rac1.den * rac2.num; 

    return simplificaRacional(n);
}

tRacional multRacionais (tRacional rac1, tRacional rac2) {
    tRacional n;

    n.num = rac1.num * rac2.num;
    n.den = rac1.den * rac2.den; 

    return simplificaRacional(n);
}

tRacional subtRacionais (tRacional rac1, tRacional rac2) {
    tRacional n;

    n.den = rac1.den * rac2.den;
    n.num = rac1.num * rac2.den - rac2.num * rac1.den;

    return simplificaRacional(n);
}

tRacional somaRacionais (tRacional rac1, tRacional rac2) {
    tRacional n;

    n.den = rac1.den * rac2.den;
    n.num = rac1.num * rac2.den + rac2.num * rac1.den;

    return simplificaRacional(n);
}

tRacional lerRacional () {
    tRacional n;
    scanf ("%d %d", &n.num, &n.den);

    return n;
}

void escreveRacional (tRacional n) {
    if (n.den == 1)
        printf ("%d\n", n.num);
    else
        printf ("%d/%d\n", n.num, n.den);
}

int verifRacional (tRacional n) {
    if (n.den == 0)
        return 0;

    return 1;
}
