#include <stdio.h>
#include "lib_racionais.h"

int main () {
    tRacional rac1, rac2;

    while (verifRacional (rac1 = lerRacional ()) && 
            verifRacional (rac2 = lerRacional ())) {
        escreveRacional (somaRacionais (rac1, rac2));
        escreveRacional (subtRacionais (rac1, rac2));
        escreveRacional (multRacionais (rac1, rac2));
        if (rac2.num != 0)
            escreveRacional (divRacionais (rac1, rac2));
        else
            printf ("Divisao invalida!\n");

        printf("\n");
    } 

    return 0;
}
