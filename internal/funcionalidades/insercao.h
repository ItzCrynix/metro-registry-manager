#ifndef INSERCAO_H
#define INSERCAO_H

#include "../utils/utils.h"
#include "../structs/cabecalho.h"
#include "../structs/registro.h"
#include "../structs/indice.h"
#include "../structs/filtro.h"
#include "../structs/par_estacao.h"

int inserir_novos_registros(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_insercao);

#endif