#include "busca.h"

// Procura registro pelo RRN
int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int rrn) {
    if (arquivo_binario == NULL)
        return FILE_NOT_FOUND_ERROR;

    Cabecalho* cab = novo_cabecalho();
    if (cab == NULL)
        return MALLOC_ERROR;

    if (ler_cabecalho_binario(arquivo_binario, cab) < NUM_CAMPOS_CABECALHO)
        return FILE_READ_ERROR;

    if (cab->status == STATUS_INCONSISTENT)
        return INCOSISTENT_FILE_ERROR;

    if (rrn < 0 || rrn >= cab->proximo_rrn) {
        free_cabecalho(&cab);
        return INVALID_RRN_ERROR;
    }

    free_cabecalho(&cab);

    *registro = ler_registro_RRN(arquivo_binario, rrn);
    if (*registro == NULL)
        return NO_DATA_FOUND_ERROR;

    return NO_ERROR;
}

// Busca sequencial por filtros (lidos da entrada)
int buscar_registro_filtro(FILE* arquivo_binario, int qtd_buscas) {
    if (arquivo_binario == NULL)
        return FILE_NOT_FOUND_ERROR;

    Cabecalho* cab = novo_cabecalho();
    if (cab == NULL)
        return MALLOC_ERROR;

    if (ler_cabecalho_binario(arquivo_binario, cab) < NUM_CAMPOS_CABECALHO)
        return FILE_READ_ERROR;

    if (cab->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cab);
        return INCOSISTENT_FILE_ERROR;
    }

    while (qtd_buscas > 0) {
       int qtd_campos;
        Filtro* filtros = ler_filtros(&qtd_campos);

        int encontrou = 0;

        // varre todos os registros
        for (int rrn = 0; rrn < cab->proximo_rrn; rrn++) {
            Registro* reg = ler_registro_RRN(arquivo_binario, rrn);
            if (reg == NULL)
                continue;

         encontrou += encontrouReg(reg, filtros, qtd_campos);
        }

        if (encontrou == 0)
            printf("Registro inexistente.");

        qtd_buscas--;

        if (qtd_buscas > 0)
            printf("\n");

        free_filtro(&filtros, qtd_campos);
    }

    free_cabecalho(&cab);
    return NO_ERROR;
}

// Verifica se o registro atende ao filtro



// Busca com apoio do índice primário (usa índice quando há filtro por codEstacao)
int buscar_registro_filtro_indexado(FILE* arquivo_binario,
                                    FILE* arquivo_indice,
                                    int   qtd_buscas) {

    if (arquivo_binario == NULL || arquivo_indice == NULL)
        return FILE_NOT_FOUND_ERROR;

    Cabecalho* cab = novo_cabecalho();
    if (cab == NULL)
        return MALLOC_ERROR;

    // lê cabeçalho do arquivo de dados
    if (ler_cabecalho_binario(arquivo_binario, cab) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cab);
        return FILE_READ_ERROR;
    }

    if (cab->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cab);
        return INCOSISTENT_FILE_ERROR;
    }

    for (int busca = 0; busca < qtd_buscas; busca++) {
        int qtd_filtros;
        Filtro* filtros = ler_filtros(&qtd_filtros);

        int tem_cod_estacao   = 0;
        int valor_cod_estacao = -1;

for (int i = 0; i < qtd_filtros; i++) {
    if (strcmp(filtros[i].campo, "codEstacao") == 0) {
        tem_cod_estacao   = 1;
        valor_cod_estacao = atoi(filtros[i].valor);
    }
}

        int encontrou = 0;

        if (tem_cod_estacao) {
            // usa índice: carrega e faz busca binária
            int qtd_indices = 0;
            Indice* indices = carregar_indice(arquivo_indice, &qtd_indices);

            if (indices != NULL) {
                int pos = busca_binaria_indice(indices, qtd_indices, valor_cod_estacao);

                if (pos != -1) {
                    Registro* reg = ler_registro_RRN(arquivo_binario, indices[pos].RRN);

                    if (reg != NULL) {
                        encontrou += encontrouReg(reg, filtros, qtd_filtros);
                    }
                }
                free(indices);
            }

        } else {
            // sem filtro por codEstacao: busca sequencial
            for (int rrn = 0; rrn < cab->proximo_rrn; rrn++) {
                Registro* reg = ler_registro_RRN(arquivo_binario, rrn);
                if (reg == NULL)
                    continue;
encontrou += encontrouReg(reg, filtros, qtd_filtros);
            }
        }

        if (!encontrou)
            printf("Registro inexistente.");

        if (busca < qtd_buscas - 1)
            printf("\n");

        free_filtro(&filtros, qtd_filtros);
    }

    free_cabecalho(&cab);
    return NO_ERROR;
}
