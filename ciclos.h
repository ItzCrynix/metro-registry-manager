#ifndef CICLOS_H
#define CICLOS_H

#include "grafo.h"
#include "utils.h"

/**
 * @brief [13] Conta a quantidade de ciclos simples a partir de uma estação de origem
 * @param arquivo_binario arquivo binário já aberto para leitura
 * @param nome_origem nome da estação de origem
 * @param qtd_ciclos ponteiro onde a quantidade de ciclos encontrados será armazenada
 */
int contar_ciclos(FILE* arquivo_binario, const char* nome_origem, int* qtd_ciclos);

#endif
