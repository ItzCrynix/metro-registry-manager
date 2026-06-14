#include "filtro.h"

Filtro* ler_filtros(int* qtd){
    scanf("%d ", qtd);
    Filtro *filtros = (Filtro*) malloc(sizeof(Filtro) * *qtd);

    for(int i = 0; i < *qtd; i++) {
        filtros[i].campo = calloc(100, sizeof(char));
        filtros[i].valor = calloc(100, sizeof(char));
        scanf("%s", filtros[i].campo);
        ScanQuoteString(filtros[i].valor);
    }

    return filtros;

}

// Libera memória alocada para filtros
void free_filtro(Filtro** filtro, int tam) {
    if (filtro == NULL || *filtro == NULL) return;

    for (int i = 0; i < tam; i++) {
        free((*filtro)[i].campo);
        free((*filtro)[i].valor);
        (*filtro)[i].campo = NULL;
        (*filtro)[i].valor = NULL;
    }

    free(*filtro);
    *filtro = NULL;
}
