#ifndef FILTRO_H
#define FILTRO_H

#include "../utils/utils.h"
#include "../structs/registro.h"


typedef struct filtro {
    char* campo;
    char* valor;
} Filtro;

Filtro* ler_filtros(int* qtd);

void free_filtro(Filtro** filtro, int tam);

int registro_passa_nos_filtros(Registro* reg, Filtro* filtros, int qtd_filtros);

int checar_filtros(Registro* registro, Filtro* filtro);

int tem_campo_indice(Filtro* filtros, int qtd_filtros);

#endif