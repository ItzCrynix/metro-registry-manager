#ifndef UPDATE_H
#define UPDATE_H

#include "utils.h"
#include "cabecalho.h"
#include "registro.h"
#include "indice.h"
#include "filtro.h"

int atualizar_registro(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_atualizacoes);
void aplicar_atualizacoes(Registro* reg, Filtro* filtros, int qtd_filtros);

#endif
