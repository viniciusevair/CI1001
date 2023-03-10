#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "libpilha.h"

typedef double t_operador;

#define TAM_ENTRADA 256
#define TAM_PILHA 1024


/* Constantes com valores para identificar os operadores. O valor 
   antes do ponto flutuante difine a precedencia entre os operadores, 
   valores maiores tem maior precedencia. */
/* Apesar da subtracao e divisao terem a mesma precedencia que a soma e
   multiplicacao respectivamente, elas nao sao comutativas, e portanto,
   por conta da maneira que esta calculadora funciona, faz-se necessario
   que a precedencia seja maior que suas operacoes inversas, que por sua vez,
   sao comutativas. */
#define SOM 10.0
#define SUB 20.0
#define MUL 30.0
#define DIV 40.0
#define POT 50.0

/* Identificador de '(' para ser empilhado na pilha de operadores */
#define PAR 99.0


/* Converte os caracteres que representam os operadores na entrada
   para valores constantes que identificam os operadores. 
   Retorna 1 se o caracter c representa um operador valido e 0 caso
   contrario. */
int converte_operador(t_operador *op, char c) {
    if(c == '+')
        *op = SOM;
    else if(c == '-')
        *op = SUB;
    else if(c == '*')
        *op = MUL;
    else if(c == '/')
        *op = DIV;
    else if(c == '^')
        *op = POT;
    else
        return 0;
    return 1;
}


/* Retorna 1 se o operador op1 tem precedencia sobre o operador op2.
   Retorna 0 caso contrario. */
int precede(t_operador op1, t_operador op2) {
    if((op1 - op2) >= 1.0)
        return 1;
    return 0;
}

/* Desempilha os dois valores no topo da pilha de valores, aplica o
   operador sobre esses valores e empilha o resultado na pilha de 
   valores. */
int opera(t_operador op, t_pilha *valores) {
    double val_esq, val_dir;

    if(!desempilha(&val_dir, valores))
        return 0;
    if(!desempilha(&val_esq, valores))
        return 0;
    if(op == SOM)
        return empilha(val_esq + val_dir, valores);
    if(op == SUB)
        return empilha(val_esq - val_dir, valores);
    if(op == MUL)
        return empilha(val_esq * val_dir, valores);
    if(op == DIV && val_dir != 0.0)
        return empilha(val_esq / val_dir, valores);
    if(op == POT)
        return empilha(pow(val_esq, val_dir), valores);
    return 0;
}


/* Imprime na saida de erro (stderr) a mensagem de erro e a linha e 
   a coluna da entrada onde o erro foi detectado. */
void erro_aborta(char *msg, int col) {
    fprintf(stderr, "ERRO: %s (coluna %d)\n", msg, col);
}


/* Le da entrada padrao, usando fgets, um vetor de caracteres ate o \n.
   Se o tamanho da entrada for maior que o vetor de leitura, aumenta
   o tamanho do vetor e continua a leitura ate encontrar o \n.
   Retorna um ponteiro para o vetor lido ou NULL caso ocorra algum erro
   ne leitura ou ne alocação de memoria. */
/*
   char* le_entrada() {
   char *ent, *ret_realloc, *ret_fgets;
   int tam, pos;

   ent = NULL;
   tam = 0;
   pos = 0;

   do {
   tam += TAM_ENTRADA;
   ret_realloc = (char*) realloc(ent, sizeof(char) * tam);
   if(!ret_realloc) {
   free(ent);
   return NULL;
   }
   ent = ret_realloc;
   ent[tam - 1] = ' ';
   ret_fgets = fgets(ent + pos, tam - pos, stdin);
   pos = tam - 1;    
   } while(ret_fgets && ent[tam - 1] == '\0' && ent[tam - 2] != '\n');

   if(!ret_fgets && tam == TAM_ENTRADA) {
   free(ent);
   return NULL;
   }

   return ent;
   }
   */


/* Le da entrada padrao, usando fgets, um vetor de caracteres ate o \n.
   Retorna um ponteiro para o vetor lido ou NULL caso ocorra algum erro
   ne leitura ou na alocação de memoria. Se o tamanho da entrada for 
   maior que o vetor de leitura, retorna NULL. */
char* le_entrada() {
    char *ent, *ret_fgets;
    int tam;

    tam = TAM_ENTRADA;
    ent = (char*) malloc(sizeof(char) * tam);
    if(!ent)
        return NULL;
    ent[tam - 1] = ' ';
    ret_fgets = fgets(ent, tam, stdin);
    if(!ret_fgets || (ent[tam - 1] == '\0' && ent[tam - 2] != '\n')) {
        free(ent);
        return NULL;
    }
    return ent;
}


int main() {
    t_pilha *pilha_valores, *pilha_operadores;
    t_operador operador, op_topo;
    double operando, resultado, resultado_anterior;
    char *entrada, *c, *prox;
    int pediu_saida, erro_ocorreu;

    resultado_anterior = 0;
    pediu_saida = 0;
    while(!pediu_saida) {
        erro_ocorreu = 0;

        pilha_valores = cria_pilha(TAM_PILHA);
        if(!pilha_valores && !erro_ocorreu) {
            erro_aborta("erro ao criar pilha de valores", 0);
            erro_ocorreu++;
        }

        pilha_operadores = cria_pilha(TAM_PILHA);
        if(!pilha_operadores && !erro_ocorreu) {
            erro_aborta("erro ao criar pilha de operadores", 0);
            erro_ocorreu++;
        }

        entrada = le_entrada();
        if(!entrada && !erro_ocorreu) {
            erro_aborta("erro de leitura", 0);
            erro_ocorreu++;
        }

        c = entrada;
        while(*c != '\n') {
            /* Percorre o ponteiro c pela entrada ate o final de linha. */

            /* Caso 1: separadores */
            if(*c == ' ' || *c == '\t')
                /* Se for sepador, passa para o proximo caracter. */
                c++;

            /* Caso 2: operando */
            else if(isdigit(*c)) {
                /* Se for [1..9] le um valor e empilha na pilha de valores. */
                operando = strtod(c, &prox);
                if(c == prox && !erro_ocorreu) {
                    erro_aborta("operando incorreto", c - entrada + 1);
                    erro_ocorreu++;
                }
                if(!empilha(operando, pilha_valores) && !erro_ocorreu) {
                    erro_aborta("pilha de valores cheia", c - entrada + 1);
                    erro_ocorreu++;
                }
                c = prox;
            }

            /* Caso 3: abre parenteses */
            else if(*c == '(') {
                /* Se for abre parenteses, empilha PAR na pilha de operadores. */
                if(!empilha(PAR, pilha_operadores) && !erro_ocorreu) {
                    erro_aborta("pilha de operadores cheia", c - entrada + 1);
                    erro_ocorreu++;
                }
                c++;
            }

            /* Caso 4: fecha parenteses */
            else if(*c == ')') {
                /* Se for fecha parenteses, processa a pilha de operadores até 
                   encontar um PAR. */ 
                while(topo(&op_topo, pilha_operadores) && op_topo != PAR) {
                    desempilha(&op_topo, pilha_operadores);
                    if(!opera(op_topo, pilha_valores) && !erro_ocorreu) {
                        erro_aborta("formato incorreto", c - entrada + 1);
                        erro_ocorreu++;
                    }
                }
                if(pilha_vazia(pilha_operadores) ||
                        (desempilha(&op_topo, pilha_operadores) && op_topo != PAR)) {
                    erro_aborta("formato incorreto", c - entrada + 1);
                    erro_ocorreu++;
                }
                c++;
            }

            /* Caso 5: operador */
            else if(converte_operador(&operador, *c)) {
                /* Se for um operador valido, verifica a precedencia em relacao
                   ao topo da pilha de operadores. */
                while(topo(&op_topo, pilha_operadores) &&
                        op_topo != PAR &&
                        precede(op_topo, operador)) {
                    /* Enquando o topo da pilha tiver precedencia, desempilha e
                       processa o operador do topo da pilha. */
                    desempilha(&op_topo, pilha_operadores);
                    if(!opera(op_topo, pilha_valores) && !erro_ocorreu) {
                        erro_aborta("formato incorreto", c - entrada + 1);
                        erro_ocorreu++;
                    }
                }
                if(!empilha(operador, pilha_operadores) && !erro_ocorreu) {
                    /* Empilha o novo operador na pilha de operadores. */
                    erro_aborta("pilha de operadores cheia", c - entrada + 1);
                    erro_ocorreu++;
                }
                c++;
            }

            /* Caso 6: pedido de saida do programa na entrada */
            else if(*c == 'q' || *c == 'Q') {
                pediu_saida = 1;
                c++;
            }

            /* Caso 7: pedido de acesso ao resultado anterior*/
            else if(*c == 'm') {
                /* em caso de nao haver resultado anterior, usa-se o valor 0 */
                operando = resultado_anterior;
                if(!empilha(operando, pilha_valores) && !erro_ocorreu) {
                    erro_aborta("pilha de valores cheia", c - entrada + 1);
                    erro_ocorreu++;
                }
                c++;
            }

            /* Caso 7: caracter invalido na entrada */
            else {
                if (!erro_ocorreu) { 
                    erro_aborta("caracter desconhecido", c - entrada + 1);
                    erro_ocorreu++;
                }
                *c='\n';
            }
        }

        /* Nesse ponto o processamento da entrada terminou e pode ser o caso da 
           pilha de operadores nao estar vazia. */
        while(desempilha(&op_topo, pilha_operadores)) {
            /* Processa os operadores que restaram na pilha. */
            if(!opera(op_topo, pilha_valores) && !erro_ocorreu) {
                erro_aborta("formato incorreto", c - entrada + 1);
                erro_ocorreu++;
            }
        }

        /* Após o processamento, o resultado final da expressao esta no topo da 
           pilha de valores. */
        if(!pediu_saida) {
            if((!desempilha(&resultado, pilha_valores) || !pilha_vazia(pilha_valores)) 
                    && !erro_ocorreu) {
                erro_aborta("formato incorreto", c - entrada + 1);
                erro_ocorreu++;
            }
            if (!erro_ocorreu && pilha_vazia(pilha_valores))
                printf("%.16g\n", resultado);
            resultado_anterior = resultado;
        }

        destroi_pilha(pilha_valores);
        destroi_pilha(pilha_operadores);
        free(entrada);
    }

    return 0;
}
