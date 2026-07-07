#ifndef AGM_H
#define AGM_H

#include "grafo.h"
#include "utils.h"

/**
 * @brief [12] Constrói a árvore geradora mínima (considerando as linhas de ida
 *        e volta) e imprime seu percurso em profundidade a partir da estação
 *        de origem informada
 * @param arquivo_binario arquivo binário já aberto para leitura
 * @param nome_origem nome da estação de origem
 */
int arvore_geradora_minima(FILE* arquivo_binario, const char* nome_origem);

#endif
