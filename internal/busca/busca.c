#include "busca.h"

int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int rrn) {
    if (arquivo_binario == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    Cabecalho* cabecalho_binario = ler_cabecalho_binario(arquivo_binario);
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
    }

    if (cabecalho_binario->status == STATUS_INCONSISTENT) {
        return INCOSISTENT_FILE_ERROR;
    }

    if (rrn < 0 || rrn >= cabecalho_binario->proximo_rrn) {
        free_cabecalho(&cabecalho_binario);
        return INVALID_RRN_ERROR;
    }

    free_cabecalho(&cabecalho_binario);

    *registro = ler_registro_RRN(arquivo_binario, rrn);
    if (*registro == NULL) {
        return NO_DATA_FOUND_ERROR;
    }

    return NO_ERROR;
}


int buscar_registro_filtro(FILE* arquivo_binario, int qtd_buscas) {
    if (arquivo_binario == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    Cabecalho* cabecalho = ler_cabecalho_binario(arquivo_binario);
    if (cabecalho == NULL) {
        return MALLOC_ERROR;
    }

    if (cabecalho->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cabecalho);
        return INCOSISTENT_FILE_ERROR;
    }

    while (qtd_buscas > 0) {
        int qtd_campos;
        scanf("%d ", &qtd_campos);

        Filtro* filtros = (Filtro*) malloc(sizeof(Filtro) * qtd_campos);

        // Pega todos os campos que serão pesquisados
        for (int i = 0; i < qtd_campos; i++) {
            filtros[i].campo = calloc(100, sizeof(char));
            filtros[i].valor = calloc(100, sizeof(char));

            scanf("%s", filtros[i].campo);
            ScanQuoteString(filtros[i].valor);
        }

        int encontrou_um = 0;

        // passa registro por registro procurando um correspondente
        for (int RRN_atual = 0; RRN_atual < cabecalho->proximo_rrn; RRN_atual++) {
            Registro* registro_atual = ler_registro_RRN(arquivo_binario, RRN_atual);
            if (registro_atual == NULL) {
                continue;
            }
           
            int qtd_correspondencias = 0;
            for (int i = 0; i < qtd_campos; i++) {
                if (passou_no_filtro(registro_atual, (filtros + i))) {
                    qtd_correspondencias++;
                }
            }

            if (qtd_correspondencias == qtd_campos) {
                print_registro(registro_atual);
                encontrou_um = 1;
            }

            free_registro(&registro_atual);
        }

        if (encontrou_um == 0) {
            printf("Registro inexistente.");
        }

        qtd_buscas--;
        
        if (qtd_buscas > 0) {
            printf("\n");
        }

        free_filtro(&filtros, qtd_campos);
    }

    free_cabecalho(&cabecalho);

    return NO_ERROR;
}

int passou_no_filtro(Registro* registro, Filtro* filtro) {
        if (strcmp(filtro->campo, "codEstacao") == 0) {
            return registro->codigo_estacao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "codLinha") == 0) {
            return registro->codigo_linha == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "codProxEstacao") == 0) {
            return registro->codigo_proxima_estacao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "distProxEstacao") == 0) {
            return registro->distancia_proxima_estacao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "codLinhaIntegra") == 0) {
            return registro->codigo_linha_integracao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "codEstIntegra") == 0) {
            return registro->codigo_estacao_integracao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "nomeEstacao") == 0) {
            return strcmp(registro->nome_estacao, filtro->valor) == 0;
        }
        if (strcmp(filtro->campo, "nomeLinha") == 0) {
            return strcmp(registro->nome_linha, filtro->valor) == 0;
        }
        
        return 0;
}

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