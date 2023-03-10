#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

/* defines de parametros da simulacao */
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES*5
#define N_LOCAIS N_HEROIS/6
#define FIM_DO_MUNDO 34944
#define N_MISSOES FIM_DO_MUNDO/100

/* defines de eventos */
#define CHEGADA 1
#define SAIDA 2
#define MISSAO 3
#define APOCALIPSE 4

typedef struct coordenadas {
    int x;
    int y;
} coord_t;

typedef struct heroi {
    int id;
    int paciencia;
    int idade;
    int exp;
    conjunto_t *habilidades;
} heroi_t;

typedef struct local {
    int id;
    int capacidade;
    conjunto_t *ocupantes;
    fila_t *fila;
    coord_t localizacao;
} local_t;

typedef struct missao {
    int id;
    conjunto_t *requisitos;
} missao_t;

typedef struct mundo {
    lef_t *lef;
    int tempo;
    conjunto_t *habilidades;
    heroi_t **herois;
    local_t **locais;
    missao_t **missoes;
    int n_missoes;
    int n_herois;
    int n_locais;
    int n_tamanho_mundo;
} mundo_t;

int aleat(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/*
 * Calcula a distancia euclidiana entre os pontos a e b no plano.
 * O typecast para int faz o retorno ser o piso da distancia.
 */
int dist(local_t *a, local_t *b) {
    float x, y;
    x = (float)a->localizacao.x - b->localizacao.x;
    y = (float)a->localizacao.y - b->localizacao.y;

    return (int)sqrt(x * x + y * y);
}

int max(int a, int b) {
    if(a >= b)
        return a;
    return b;
}

/*
 * Retorna um ponteiro para um local novo, ou NULL caso falhe a alocacao da
 * memoria.
 *
 * A fila e conjunto internos do local tambem sao alocados dinamicamente e devem
 * ser liberados apos o uso.
 */
local_t* define_local(int id, int tamanho_maximo) {
    local_t *local_temp;
    if(! (local_temp = malloc(sizeof(local_t))))
        return NULL;
    
    local_temp->id = id;
    local_temp->capacidade = aleat(5, 30);
    local_temp->localizacao.x = aleat(0, tamanho_maximo - 1);
    local_temp->localizacao.y = aleat(0, tamanho_maximo - 1);
    local_temp->ocupantes = cria_cjt(local_temp->capacidade);
    local_temp->fila = cria_fila();

    return local_temp;
}

/*
 * Retorna um ponteiro para um heroi novo, ou NULL em caso de falha da alocacao
 * de memoria.
 *
 * O conjunto de habilidades do heroi tambem eh alocada dinamicamente e deve ser
 * liberada apos seu uso.
 */
heroi_t* define_heroi(int id, conjunto_t *habilidades_mundo) {
    heroi_t *heroi_temp;
    if(! (heroi_temp = malloc(sizeof(heroi_t))))
        return NULL;

    heroi_temp->id = id;
    heroi_temp->exp = 0;
    heroi_temp->paciencia = aleat(0, 100);
    heroi_temp->idade = aleat(18, 100);
    heroi_temp->habilidades = cria_subcjt_cjt(habilidades_mundo, aleat(2, 5));

    return heroi_temp;
}

/*
 * Retorna um ponteiro para uma missao nova, ou NULL em caso de falha da
 * alocacao de memoria.
 *
 * O conjunto de exigencias da missao tambem eh alocada dinamicamente e deve ser
 * liberada apos seu uso.
 */
missao_t* define_missao(int id, conjunto_t *habilidades_mundo) {
    missao_t *missao_temp;
    if(! (missao_temp = malloc(sizeof(missao_t))))
        return NULL;

    missao_temp->id = id;

    return missao_temp;
}

/*
 * Recebe uma serie de dados e os manda para a Lista de Eventos Futuros como um
 * unico evento. Os eventos sao adicionados ordenadamente de acordo com o tempo
 * de ocorrencia.
 *
 * O evento pode ser declarado localmente uma vez que a funcao da biblioteca lef
 * faz uma copia da memoria local. Por conta disso, a memoria da copia do evento
 * deve ser liberada apos seu uso.
 */
int adiciona_evento(lef_t *lef, int tempo, int dado1, int dado2, int tipo) {
    evento_t evento;

    evento.tipo = tipo;
    evento.tempo = tempo;
    evento.dado1 = dado1;
    evento.dado2 = dado2;
    if(! (adiciona_ordem_lef(lef, &evento)))
        return 0;

    return 1;
}

/*
 * Trata os dados do evento de entrada de um heroi num local.
 * A struct mundo eh passada apenas para uso da lef, tempo atual, e escolher
 * um novo local. O local e heroi relevantes ao evento sao recebidos 
 * separadamente pela funcao.
 */
int evento_chegada(mundo_t *terra1001, heroi_t *heroi, local_t *local) {
    int tpl, tdl;
    local_t *novo_destino;

    printf("%6d:CHEGA HEROI %2d Local %d (%2d/%2d)", terra1001->tempo, heroi->id, local->id, cardinalidade_cjt(local->ocupantes), local->capacidade);

    /*
     * Se o local tiver ocupado, verifica se 1/4 da paciencia do heroi eh maior
     * que o tamanho atual da fila. Se for, entra na fila, senao ele desiste e
     * vai para outro lugar.
     */
    if(cardinalidade_cjt(local->ocupantes) >= local->capacidade) {
        if(heroi->paciencia>>2 > tamanho_fila(local->fila)) {
            insere_fila(local->fila, heroi->id);
            printf(", FILA %d\n", local->fila->tamanho);
        } else {
            /*
             * novo destino e tdl calculados dentro da funcao de chegada para
             * evitar a necessidade de um evento de saida instantaneo como
             * intermediario.
             */
            novo_destino = terra1001->locais[aleat(0, terra1001->n_locais - 1)];
            tdl = dist(local, novo_destino) / (100 - max(0, heroi->idade - 40));
            adiciona_evento(terra1001->lef, terra1001->tempo + tdl/15, heroi->id, novo_destino->id, CHEGADA);
            printf(", DESISTE\n");
        }

        return 1;
    }

    /* Gera o tempo de saida do heroi */
    tpl = max(1, heroi->paciencia/10 + aleat(-2, 6));
    adiciona_evento(terra1001->lef, terra1001->tempo + tpl, heroi->id, local->id, SAIDA);
    printf(", ENTRA\n");
    insere_cjt(local->ocupantes, heroi->id);

    return 1;
}

/*
 * Trata os dados do evento de saida de um heroi num local.
 * A struct mundo eh passada apenas para uso da lef, tempo atual, e escolher
 * um novo local. O local e heroi relevantes ao evento sao recebidos 
 * separadamente pela funcao.
 */
int evento_saida(mundo_t *terra1001, heroi_t *heroi, local_t *local) {
    int tdl, id_heroi_esperando;
    local_t *novo_destino;
    evento_t *evento;

    printf("%6d:SAIDA HEROI %2d Local %d (%2d/%2d)", terra1001->tempo, heroi->id, local->id, cardinalidade_cjt(local->ocupantes), local->capacidade);
    retira_cjt(local->ocupantes, heroi->id);

    if(! (evento = malloc(sizeof(evento_t))))
        return 0;

    /*
     * O caso a seguir nao utiliza a funcao de adicionar eventos pois eh uma
     * excessao que precisa adiconar um evento no inicio, para evitar que outro
     * heroi que vai entrar no mesmo tempo fure a fila.
     */
    if(! vazia_fila(local->fila)) {
        retira_fila(local->fila, &id_heroi_esperando);
        evento->tipo = CHEGADA;
        evento->tempo = terra1001->tempo;
        evento->dado1 = id_heroi_esperando;
        evento->dado2 = local->id;
        if(! (adiciona_inicio_lef(terra1001->lef, evento)))
            return 0;
        printf(", REMOVE FILA HEROI %2d", id_heroi_esperando);
    }

    novo_destino = terra1001->locais[aleat(0, terra1001->n_locais - 1)];
    tdl = dist(local, novo_destino) / (100 - max(0, heroi->idade - 40));
    if(! (adiciona_evento(terra1001->lef, terra1001->tempo + tdl/15, heroi->id, novo_destino->id, CHEGADA)))
        return 0;

    printf("\n");

    free(evento);

    return 1;
}

/*
 * Funcao auxiliar da funcao "evento_missao", verifica se existe uma equipe que
 * satisfaz os requisitos da missao, e caso exista mais de uma, seleciona a
 * equipe com a menor quantidade de herois.
 *
 * Retorna o id do local com a equipe que resolve a missao em caso de sucesso, 
 * -1 em caso de falha na alocacao de memoria e
 * -2 no caso de nenhuma equipe preencher os requisitos.
 */
int escolhe_equipe(mundo_t *terra1001, missao_t *missao) {
    int i, j, distintivo, eqs;
    conjunto_t *solucao, *solucao_anterior, *uniao, *uniao_anterior;

    if(! (solucao = cria_cjt(10)))
        return -1;

    /* 
     * Loop externo, caminha por todos os locais.
     */
    i = 0;
    while(i < terra1001->n_locais) {
        if(! (uniao = cria_cjt(10))) {
            solucao = destroi_cjt(solucao);
            return -1;
        }

        inicia_iterador_cjt(terra1001->locais[i]->ocupantes);

        /* 
         * Loop interno, constroi a uniao de habilidades 
         * da equipe de herois presentes no local "i".
         */
        j = 0;
        while(j < cardinalidade_cjt(terra1001->locais[i]->ocupantes)) {
            uniao_anterior = uniao;

            /* Distintivo representa o numero de identificacao do heroi. */
            incrementa_iterador_cjt(terra1001->locais[i]->ocupantes, &distintivo);
            if(! (uniao = uniao_cjt(uniao, terra1001->herois[distintivo]->habilidades))) {
                solucao = destroi_cjt(solucao);
                uniao_anterior = destroi_cjt(uniao_anterior);

                return -1;
            }
            uniao_anterior = destroi_cjt(uniao_anterior);
            j++;
        }

        printf("%6d:MISSAO %3d HAB_EQL %d: ", terra1001->tempo, missao->id, i);
        imprime_cjt(uniao);

        /* 
         * Se a equipe tem as habilidades necessarias, e eh menor que uma equipe
         * anteriormente elegivel para solucao, atribui a nova equipe para a
         * solucao. 
         */
        if(contido_cjt(missao->requisitos, uniao))
            if(vazio_cjt(solucao) || cardinalidade_cjt(terra1001->locais[i]->ocupantes) < cardinalidade_cjt(solucao)) {
                solucao_anterior = solucao;
                if(! (solucao = copia_cjt(terra1001->locais[i]->ocupantes))) {
                    uniao = destroi_cjt(uniao);
                    solucao = destroi_cjt(solucao);
                    return -1;
                }
                eqs = i;
                solucao_anterior = destroi_cjt(solucao_anterior);
            }

        uniao = destroi_cjt(uniao);
        i++;
    }

    if(vazio_cjt(solucao)) {
        printf("%6d:MISSAO %3d IMPOSSIVEL\n", terra1001->tempo, missao->id);
        solucao = destroi_cjt(solucao);
        return -2;
    }

    printf("%6d:MISSAO %3d HER_EQS %d: ", terra1001->tempo, missao->id, eqs);
    imprime_cjt(solucao);
    solucao = destroi_cjt(solucao);

    return eqs;
}

/*
 * Trata os dados do evento de uma missao.
 * A liberacao da memoria que guarda a missao
 * eh feita durante o evento do apocalipse.
 */
int evento_missao(mundo_t *terra1001, missao_t *missao) {
    int i, eqs, distintivo;

    missao->requisitos = cria_subcjt_cjt(terra1001->habilidades, aleat(3, 6));
    printf("%6d:MISSAO %3d HAB_REQ ", terra1001->tempo, missao->id);
    imprime_cjt(missao->requisitos);

    eqs = escolhe_equipe(terra1001, missao);

    /*
     * Caso nenhum local tenha uma equipe apta a resolver a missao,
     * o evento da missao eh recriado para acontecer mais uma vez no futuro.
     */
    if(eqs == -2) {
        missao->requisitos = destroi_cjt(missao->requisitos);
        adiciona_evento(terra1001->lef, aleat(terra1001->tempo, FIM_DO_MUNDO), missao->id, 0, MISSAO);
        return 1;
    }
    if(eqs == -1)
        return 0;

    /*
     * Em caso de sucesso, incrementa a xp de todos
     * os herois presentes no local.
     */
    i = 0;
    inicia_iterador_cjt(terra1001->locais[eqs]->ocupantes);
    while(i < cardinalidade_cjt(terra1001->locais[eqs]->ocupantes)) {
        incrementa_iterador_cjt(terra1001->locais[eqs]->ocupantes, &distintivo);
        (terra1001->herois[distintivo]->exp)++;
        i++;
    }

    missao->requisitos = destroi_cjt(missao->requisitos);

    return 1;
}

/*
 * Trata os dados do evento de fim de mundo.
 * Exibe a experiencia final de cada heroi e em seguida
 * libera toda a memoria utilizada durante o programa,
 * com excessao das memorias reservadas para a lef e mundo, que sao liberadas
 * durante a finalizacao na main.
 */
int evento_apocalipse(mundo_t *terra1001) {
    int i;

    printf("%6d:FIM\n", terra1001->tempo);

    /* liberando herois */
    i = 0;
    while(i < terra1001->n_herois) {
        printf("HEROI %2d EXPERIENCIA %d\n", terra1001->herois[i]->id, terra1001->herois[i]->exp);
        terra1001->herois[i]->habilidades = destroi_cjt(terra1001->herois[i]->habilidades);
        free(terra1001->herois[i]);

        i++;
    }
    free(terra1001->herois);
    terra1001->herois = NULL;

    /* liberando locais */
    i = 0;
    while(i < terra1001->n_locais) {
        terra1001->locais[i]->ocupantes = destroi_cjt(terra1001->locais[i]->ocupantes);
        terra1001->locais[i]->fila = destroi_fila(terra1001->locais[i]->fila);
        free(terra1001->locais[i]);

        i++;
    }
    free(terra1001->locais);
    terra1001->locais = NULL;

    /* liberando missoes */
    i = 0;
    while(i < terra1001->n_missoes) {
        free(terra1001->missoes[i]);

        i++;
    }
    free(terra1001->missoes);
    terra1001->missoes = NULL;

    /* liberando catalogo de habilidades */
    terra1001->habilidades = destroi_cjt(terra1001->habilidades);

    /* Destroi todos eventos marcados para acontecer apos o apocalipse. */
    terra1001->lef = destroi_lef(terra1001->lef);

    return 1;
}

mundo_t* cria_mundos() {
    mundo_t *terra1001;
    int i;

    if(! (terra1001 = malloc(sizeof(mundo_t))))
        return NULL;
    terra1001->lef = cria_lef();
        
    terra1001->tempo = 0;
    terra1001->n_tamanho_mundo = N_TAMANHO_MUNDO;
    terra1001->n_herois = N_HEROIS;
    terra1001->n_locais = N_LOCAIS;
    terra1001->n_missoes = N_MISSOES;

    /* habilidades */
    if(! (terra1001->habilidades = cria_cjt(N_HABILIDADES)))
        return NULL;
    i = 0;
    while(i < N_HABILIDADES) {
        insere_cjt(terra1001->habilidades, i);        
        i++;
    }

    /* locais */
    if(! (terra1001->locais = calloc(terra1001->n_locais, sizeof(local_t*))))
        return NULL;
    i = 0;
    while(i < terra1001->n_locais) {
        if(!(terra1001->locais[i] = define_local(i, terra1001->n_tamanho_mundo)))
            return NULL;
        
        i++;
    }

    /* herois */
    if(! (terra1001->herois = calloc(terra1001->n_herois, sizeof(heroi_t*))))
        return NULL;
    i = 0;
    while(i < terra1001->n_herois) {
        if(!(terra1001->herois[i] = define_heroi(i, terra1001->habilidades)))
            return NULL;
        
        adiciona_evento(terra1001->lef, aleat(0, 96 * 7), terra1001->herois[i]->id, terra1001->locais[aleat(0, terra1001->n_locais - 1)]->id, CHEGADA);
        i++;
    }

    /* missoes */
    if(! (terra1001->missoes = calloc(terra1001->n_missoes, sizeof(missao_t*))))
        return NULL;

    i = 0;
    while(i < terra1001->n_missoes) {
        if(!(terra1001->missoes[i] = define_missao(i, terra1001->habilidades)))
            return NULL;
        
        adiciona_evento(terra1001->lef, aleat(0, FIM_DO_MUNDO), terra1001->missoes[i]->id, 0, MISSAO);
        i++;
    }

    /* apocalipse */
    adiciona_evento(terra1001->lef, FIM_DO_MUNDO, 4, 7, APOCALIPSE);

    return terra1001;
}

int main() {
    srand(time(NULL));
    int mundo_existe;
    mundo_t *terra1001;
    evento_t *evento_atual;

    if(! (terra1001 = cria_mundos()))
        return 1;

    /*
     * Bloco principal do programa. Caminha pela LEF executando os eventos ao
     * longo do tempo.
     *
     * O criterio de parada do loop eh o momento em que o apocalipse acontece,
     * caso contrario, os eventos retroalimentam a lef indefinidamente.
     */
    mundo_existe = 1;
    while(mundo_existe && (evento_atual = obtem_primeiro_lef(terra1001->lef))) {
        terra1001->tempo = evento_atual->tempo;
        switch(evento_atual->tipo) {
            case CHEGADA:
                if(! (evento_chegada(terra1001, terra1001->herois[evento_atual->dado1], terra1001->locais[evento_atual->dado2])))
                    return 1;
                break;
            case SAIDA:
                if(! (evento_saida(terra1001, terra1001->herois[evento_atual->dado1], terra1001->locais[evento_atual->dado2])))
                    return 1;
                break;
            case MISSAO:
                if(! (evento_missao(terra1001, terra1001->missoes[evento_atual->dado1])))
                    return 1;
                break;
            case APOCALIPSE:
                evento_apocalipse(terra1001);
                mundo_existe = 0;
                break;
        }    

        free(evento_atual);
    }

    free(terra1001);

    return 0;
}
