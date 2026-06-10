#ifndef ESTACAO_H
#define ESTACAO_H

#include "../utils.h"

typedef struct parestacao {
    int estacao;
    int proxima_estacao;
} ParEstacao;

int busca_par_estacao(ParEstacao* pares, int tam, ParEstacao procurado);

int busca_estacao(char** estacoes, int tam, char* procurado);

#endif