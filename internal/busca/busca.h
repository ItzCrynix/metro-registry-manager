#ifndef BUSCA_H
#define BUSCA_H

#include "../utils/utils.h"
#include "../structs/cabecalho.h"
#include "../structs/registro.h"

typedef struct filtro {
    char* campo;
    char* valor;
} Filtro;

/**
 * @param arquivo_binario
 * @param registro
 * @param rrn
 */
int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int RRN);

int buscar_registro_filtro(FILE* arquivo_binario, int qtd_buscas);

void free_filtro(Filtro** filtro, int tam);

int passou_no_filtro(Registro* registro, Filtro* filtro);

#endif