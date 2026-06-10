#include "par_estacao.h"

int busca_par_estacao(ParEstacao* pares, int tam, ParEstacao procurado) {
    for (int i = 0; i < tam; i++) {
        if (pares[i].estacao == procurado.estacao && pares[i].proxima_estacao == procurado.proxima_estacao)
            return i;
    }

    return NO_DATA_FOUND_ERROR;
}

int busca_estacao(char** estacoes, int tam, char* procurado) {
    for (int i = 0; i < tam; i++) {
        if (estacoes[i] == NULL) break;

        if (strcmp(estacoes[i], procurado) == 0) {
            return i;
        }
    }

    return NO_DATA_FOUND_ERROR;
}