#ifndef CABECALHO_H
#define CABECALHO_H

#include "../utils/utils.h"

typedef struct cabecalho {
    char status;
    int topo_pilha;
    int proximo_rrn;
    int numero_estacoes;
    int numero_pares_estacoes;
} Cabecalho;

/**
 * @brief Aloca memória para um novo cabeçalho e define valores padrões pra ele
 * @return Ponteiro para um cabeçalho ou NULL
 */
Cabecalho* novo_cabecalho();

void free_cabecalho(Cabecalho** cabecalho);

/**
 * @brief Salva as informações contidas no cabeçalho de um arquivo binário
 * @param arquivo_binario O arquivo que vai ser manipulado
 * @param cabecalho_binario O cabeçalho que contem as informações que vão ser salvas
 */
int salvar_cabecalho(FILE* arquivo_binario, Cabecalho* cabecalho_binario);

int ler_cabecalho_binario(FILE* arquivo_binario, Cabecalho* cabecalho_binario);

#endif