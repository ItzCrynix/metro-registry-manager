#ifndef FILTRO_H
#define FILTRO_H

#include "../utils/utils.h"


typedef struct filtro {
    char* campo;
    char* valor;
} Filtro;

Filtro* ler_filtros(int* qtd);

void free_filtro(Filtro** filtro, int tam);

#endif