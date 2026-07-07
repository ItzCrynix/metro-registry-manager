#ifndef REMOCAO_H
#define REMOCAO_H

#include "utils.h"
#include "cabecalho.h"
#include "registro.h"
#include "indice.h"
#include "filtro.h"
#include "par_estacao.h"

int remover_registros_com_indice(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_remocoes);

#endif