/* 
 * Arquivo de headers para manipular ponteiros para racionais.
 * feito em 09/11/2022 para o tp3 da disciplina prog1.
*/

/*********** NAO ALTERE ESTE ARQUIVO *********************/

typedef struct racional {
    int num;
    int den;
} racional;

/* retorna um numero aleatorio entre min e max, inclusive. */
int aleat (int min, int max);

/* cria um racional sem valores atribuidos, so aloca o espaco */
racional *criar_r ();

/* libera a memoria de um racional alocado */
racional *liberar_r (racional *r);

/* aloca espaço para um ponteiro para um racional contendo
 * numeros aleatorios no numerador e denominador.
 * Retorna o ponteiro para o racional simplificado. */
racional *sortear_r ();

/* le um racional, retorna 1 em caso de sucesso ou 0 em caso de falha 
 * a qual pode ocorrer por uma entrada de racional invalido ou falha no scanf */
int ler_r (racional *r);

/* imprime um racional sem espaco em branco apos o numero e sem mudar de linha */
void imprimir_r (racional *r);
 
/* calcula o mdc pelo metodo de Euclides */
int mdc (int a, int b);

/* mmc = (a * b) / mdc (a, b) */
int mmc (int a, int b);

/* retorna 1 se o racional r eh valido ou 0 caso contrario */
int valido_r (racional *r);

/* simplifica um racional para o menor denominador possivel   */
/* caso o racional seja da forma n/n, simplifica ele para 1/1 */
void simplifica_r (racional *r);

/* retorna 1 caso *r1 seja menor do que *r2 */
int menor_r (racional *r1, racional *r2);

/* retorna 1 caso os racionais *r1 e *r2 sejam iguais ou 0 caso contrario */
int iguais_r (racional *r1, racional *r2);

/* soma dois racionais e devolve um ponteiro para o resultado       */
/* retorna NULL em qualquer caso de falha, por consistencia com dividir */
racional *somar_r (racional *r1, racional *r2);

/* subtrai dois racionais e devolve um ponteiro para o resultado    */
/* retorna NULL em qualquer caso de falha, por consistencia com dividir */
racional *subtrair_r (racional *r1, racional *r2);

/* multiplica dois racionais e devolve um ponteiro o resultado      */
/* retorna NULL em qualquer caso de falha, por consistencia com dividir */
racional *multiplicar_r (racional *r1, racional *r2);

/* divide dois racionais e devolve um ponteiro para resultado */
/* retorna NULL em qualquer caso de falha                     */
racional *dividir_r (racional *r1, racional *r2);
