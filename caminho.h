#ifndef CAMINHO_H
#define CAMINHO_H

#include "grafo.h"
#include "utils.h"

#define SEM_CAMINHO -100

/**
 * @brief [11] Determina e imprime o caminho mais curto entre duas estações
 * @param arquivo_binario arquivo binário já aberto para leitura
 * @param nome_origem nome da estação de origem
 * @param nome_destino nome da estação de destino
 */
int caminho_mais_curto(FILE* arquivo_binario, const char* nome_origem, const char* nome_destino);

#endif
