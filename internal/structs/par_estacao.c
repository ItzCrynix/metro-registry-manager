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

int conta_estacao_e_pares(FILE* arquivo_binario, int qtd_registros, char** estacoes, ParEstacao* pares, int *qtd_estacoes, int *qtd_pares) {
    for (int rrn = 0; rrn < qtd_registros; rrn++) {
        Registro* temp = ler_registro_RRN(arquivo_binario, rrn);
        if (temp == NULL)
            continue;

        if (busca_estacao(estacoes, *qtd_estacoes, temp->nome_estacao) == NO_DATA_FOUND_ERROR) {
            estacoes[*qtd_estacoes] = strdup(temp->nome_estacao);
            (*qtd_estacoes)++;
        }

        if (temp->codigo_proxima_estacao != -1) {
            ParEstacao novo_par = {.estacao = temp->codigo_estacao, .proxima_estacao = temp->codigo_proxima_estacao};
            if (busca_par_estacao(pares, *qtd_pares, novo_par) == NO_DATA_FOUND_ERROR) {
                pares[*qtd_pares].estacao = novo_par.estacao;
                pares[*qtd_pares].proxima_estacao = novo_par.proxima_estacao;
                (*qtd_pares)++;
            }
        }

         free_registro(&temp);
    }

    return NO_ERROR;
}