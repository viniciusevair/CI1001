#include <stdio.h>

typedef struct {
    int num;
    int den;
} tRacional;

int calculaMDC (int n, int m) {
    int resto = n%m;

    while (resto != 0) {
        n = m;
        m = resto;
        resto = n%m;
    }

    return m;
}

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

int main () {
    tRacional rac1, rac2;

    do {
        rac1 = lerRacional ();

        if (verifRacional (rac1)) {
            rac2 = lerRacional ();

            if (verifRacional (rac2)) {
                    escreveRacional (somaRacionais (rac1, rac2));
                    escreveRacional (subtRacionais (rac1, rac2));
                    escreveRacional (multRacionais (rac1, rac2));
                    escreveRacional (divRacionais (rac1, rac2));

                    printf("\n");
            }
        }
    } while (verifRacional (rac1) && verifRacional (rac2));

    return 0;
}
