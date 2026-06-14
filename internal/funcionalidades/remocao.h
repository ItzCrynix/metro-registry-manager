#ifndef REMOCAO_H
#define REMOCAO_H

#include "../utils/utils.h"
#include "../structs/cabecalho.h"
#include "../structs/registro.h"
#include "../structs/indice.h"
#include "../structs/filtro.h"
#include "../structs/par_estacao.h"

int remover_registros_com_indice(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_remocoes);

#endif