#ifndef INDICE_H
#define INDICE_H

#include "../utils/utils.h"
#include "cabecalho.h"

typedef struct indice {
    int id;
    int RRN;
} Indice;

/**
 * @brief Funciona como um insertion sort, organizando a lista em ordem crescente
 */
void organiza_lista_indice(Indice* lista, int tam);

int salvar_indices(FILE* arquivo_indice, Indice* indices, int tam);

int gerar_arquivo_indice(FILE* arquivo_binario, FILE* arquivo_indice);

#endif