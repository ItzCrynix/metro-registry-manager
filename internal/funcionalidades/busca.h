#ifndef BUSCA_H
#define BUSCA_H

#include "utils.h"
#include "cabecalho.h"
#include "registro.h"
#include "indice.h"  
#include "filtro.h"

// Procura um registro pelo RRN
int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int RRN);

// Busca sequencial por filtros (lê filtros da entrada)
int buscar_registro_filtro(FILE* arquivo_binario, int qtd_buscas);


// Busca com uso do índice primário quando aplicável
int buscar_registro_filtro_com_indice(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_buscas);

#endif
