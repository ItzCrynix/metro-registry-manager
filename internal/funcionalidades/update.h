#ifndef UPDATE_H 
#define UPDATE_H    

#include "../utils/utils.h"
#include "../structs/cabecalho.h"
#include "../structs/registro.h"
#include "../structs/indice.h"
#include "../structs/filtro.h"

int atualizar_registro(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_atualizacoes);
void aplicar_atualizacoes(Registro* reg, Filtro* filtros, int qtd_filtros);

#endif             