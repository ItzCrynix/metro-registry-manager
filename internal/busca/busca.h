#ifndef BUSCA_H
#define BUSCA_H

#include "../utils/utils.h"
#include "../structs/cabecalho.h"
#include "../structs/registro.h"
#include "../structs/indice.h"  
#include "../filtro/filtro.h"
// Procura um registro pelo RRN
int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int RRN);

// Busca sequencial por filtros (lê filtros da entrada)
int buscar_registro_filtro(FILE* arquivo_binario, int qtd_buscas);

// Verifica se um registro atende a um filtro
int passou_no_filtro(Registro* registro, Filtro* filtro);

// Busca com uso do índice primário quando aplicável
int buscar_registro_filtro_indexado(FILE* arquivo_binario,
                                    FILE* arquivo_indice,
                                    int   qtd_buscas);

#endif
