#ifndef ESTACAO_H
#define ESTACAO_H

#include "utils.h"

typedef struct parestacao {
    int estacao;
    int proxima_estacao;
} ParEstacao;

/**
 * @brief Busca um par de dados de estação específico
 * @param pares indica a lista que será buscada
 * @param tam O tamanho da lista
 * @param procurado Os dados que você quer buscar
 * @return O indice do primeiro elemento que bate com o procurado, se não retorna -1
 */
int busca_par_estacao(ParEstacao* pares, int tam, ParEstacao procurado);

/**
 * @brief Busca um nome de estação específico
 * @param estacoes indica a lista que será buscada
 * @param tam O tamanho da lista
 * @param procurado Os dados que você quer buscar
 * @return O indice do primeiro elemento que bate com o procurado, se não retorna -1
 */
int busca_estacao(char** estacoes, int tam, char* procurado);

#endif