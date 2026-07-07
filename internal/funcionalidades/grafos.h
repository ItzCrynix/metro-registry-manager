#ifndef GRAFOS_H
#define GRAFOS_H

#include "../utils/utils.h"
#include "../structs/grafo.h"

#define SEM_CAMINHO -100

typedef struct {
    int destino;
    int peso;
} ArestaND; // aresta não-direcionada (ida e volta)

/**
 * @brief [10] Constrói e imprime a lista de adjacências do grafo de estações
 * @param arquivo_binario arquivo binário já aberto para leitura
 */
int imprimir_lista_adjacencia(FILE* arquivo_binario);

/**
 * @brief [11] Determina e imprime o caminho mais curto entre duas estações
 * @param arquivo_binario arquivo binário já aberto para leitura
 * @param nome_origem nome da estação de origem
 * @param nome_destino nome da estação de destino
 */
int caminho_mais_curto(FILE* arquivo_binario, const char* nome_origem, const char* nome_destino);

/**
 * @brief [12] Constrói a árvore geradora mínima (considerando as linhas de ida
 *        e volta) e imprime seu percurso em profundidade a partir da estação
 *        de origem informada
 * @param arquivo_binario arquivo binário já aberto para leitura
 * @param nome_origem nome da estação de origem
 */
int arvore_geradora_minima(FILE* arquivo_binario, const char* nome_origem);

/**
 * @brief [13] Conta a quantidade de ciclos simples a partir de uma estação de origem
 * @param arquivo_binario arquivo binário já aberto para leitura
 * @param nome_origem nome da estação de origem
 * @param qtd_ciclos ponteiro onde a quantidade de ciclos encontrados será armazenada
 */
int contar_ciclos(FILE* arquivo_binario, const char* nome_origem, int* qtd_ciclos);

#endif