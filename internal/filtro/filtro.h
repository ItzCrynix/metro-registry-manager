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
int passou_no_filtro(Registro* registro, Filtro* filtro);
int encontrouReg(Registro* reg, Filtro* filtros, int qtd_filtros);
#endif