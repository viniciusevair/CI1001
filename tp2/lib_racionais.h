    /* cria o tipo racional a partir de dois inteiros, um numerador
 * e um denominador */
typedef struct {
    int num;
    int den;
} tRacional;

/* le dois inteiros e devolve um tipo racional */
tRacional lerRacional ();

/* verifica se o denominador eh valido (diferente de zero), se for valido,
 * devolve 1, e se nao for valido devolve 0 */
int verifRacional (tRacional n);

/* escreve na tela o racional recebido */
void escreveRacional (tRacional n);

/* recebe dois racionais, realiza a soma e simplifica o resultado, devolve o
 * resultado */
tRacional somaRacionais (tRacional rac1, tRacional rac2);

/* recebe dois racionais, realiza a subtracao e simplifica o resultado, devolve
 * o resultado */
tRacional subtRacionais (tRacional rac1, tRacional rac2);

/* recebe dois racionais, realiza a multiplicacao e simplifica o resultado,
 * devolve o resultado */
tRacional multRacionais (tRacional rac1, tRacional rac2);

/* recebe dois racionais, realiza a divisao e simplifica o resultado, devolve o
 * resultado */
tRacional divRacionais (tRacional rac1, tRacional rac2);
