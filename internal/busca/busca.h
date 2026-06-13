#ifndef BUSCA_H
#define BUSCA_H

#include "../utils/utils.h"
#include "../structs/cabecalho.h"
#include "../structs/registro.h"
#include "../structs/indice.h"   /* necessário para usar o tipo Indice */

typedef struct filtro {
    char* campo;
    char* valor;
} Filtro;

/* --- funções já existentes --- */

int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int RRN);

int buscar_registro_filtro(FILE* arquivo_binario, int qtd_buscas);

void free_filtro(Filtro** filtro, int tam);

int passou_no_filtro(Registro* registro, Filtro* filtro);

/* --- nova função para a funcionalidade [6] --- */

/**
 * @brief [6] Busca com apoio do índice primário.
 *
 * Se algum filtro for por "codEstacao", usa busca binária no índice.
 * Para os demais campos, faz busca sequencial (igual à [3]).
 *
 * @param arquivo_binario  Arquivo de dados (estacao.bin)
 * @param arquivo_indice   Arquivo de índice (indexaEstacao.bin)
 * @param qtd_buscas       Quantas buscas executar
 */
int buscar_registro_filtro_indexado(FILE* arquivo_binario,
                                    FILE* arquivo_indice,
                                    int   qtd_buscas);

#endif
