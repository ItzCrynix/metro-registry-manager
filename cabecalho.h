#ifndef CABECALHO_H
#define CABECALHO_H

#include <stdio.h>

#define TAM_REGISTRO_CABECALHO 17
#define STATUS_CONSISTENT '0'
#define STATUS_INCONSISTENT '1'

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

/**
 * @brief Salva as informações contidas no cabeçalho de um arquivo binário
 * @param arquivo_binario O arquivo que vai ser manipulado
 * @param cabecalho_binario O cabeçalho que contem as informações que vão ser salvas
 */
void salvar_cabecalho(FILE* arquivo_binario, Cabecalho* cabecalho_binario);

Cabecalho* ler_cabecalho_binario(FILE* arquivo_binario);

#endif