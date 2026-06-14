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

    Cabecalho* cabecalho_binario = novo_cabecalho();
    if (cabecalho_binario == NULL)
        return MALLOC_ERROR;

    if (ler_cabecalho_binario(arquivo_binario, cabecalho_binario) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cabecalho_binario);
        return FILE_READ_ERROR;
    }

    if (cabecalho_binario->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cabecalho_binario);
        return INCOSISTENT_FILE_ERROR;
    }

    for (int busca = 0; busca < qtd_buscas; busca++) {
        int qtd_filtros;
        Filtro* filtros = ler_filtros(&qtd_filtros);

        int encontrou = 0;

        // varre todos os registros
        for (int rrn = 0; rrn < cabecalho_binario->proximo_rrn; rrn++) {
            Registro* registro_encontrado = ler_registro_RRN(arquivo_binario, rrn);
            if (registro_encontrado == NULL) continue;

            if (registro_passa_nos_filtros(registro_encontrado, filtros, qtd_filtros)) {
                encontrou = 1;
                print_registro(registro_encontrado);
            }
        }

        if (!encontrou)
            printf("Registro inexistente.\n");

        // problema de formatacao
        if (busca < qtd_buscas - 1)
            printf("\n");

        free_filtro(&filtros, qtd_filtros);
    }

    free_cabecalho(&cabecalho_binario);
    return NO_ERROR;
}

// Busca com apoio do índice primário (usa índice quando há filtro por codEstacao)
int buscar_registro_filtro_com_indice(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_buscas) {
    if (arquivo_binario == NULL || arquivo_indice == NULL)
        return FILE_NOT_FOUND_ERROR;

    Cabecalho* cabecalho_binario = novo_cabecalho();
    int qtd_indices = 0;
    Indice* indices = carregar_indice(arquivo_indice, &qtd_indices);
    if (cabecalho_binario == NULL || indices == NULL) {
        return MALLOC_ERROR;
    }

    if (ler_cabecalho_binario(arquivo_binario, cabecalho_binario) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cabecalho_binario);
        return FILE_READ_ERROR;
    }

    char status_indice;
    fread(&status_indice, sizeof(char), 1, arquivo_indice);
    if (cabecalho_binario->status == STATUS_INCONSISTENT || status_indice == STATUS_INCONSISTENT) {
        free_cabecalho(&cabecalho_binario);
        return INCOSISTENT_FILE_ERROR;
    }

    for (int busca = 0; busca < qtd_buscas; busca++) {
        int qtd_filtros = 0;
        Filtro* filtros = ler_filtros(&qtd_filtros);

        Registro* registro_encontrado;
        int encontrou = 0;
        int pos = -1;

        int pos_indice = tem_campo_indice(filtros, qtd_filtros);
        if (pos_indice != -1) {
            int valor_cod_estacao = atoi(filtros[pos_indice].valor);
            pos = busca_binaria_indice(indices, qtd_indices, valor_cod_estacao);
            if (pos == -1) continue;

            registro_encontrado = ler_registro_RRN(arquivo_binario, indices[pos].RRN);
            if (registro_encontrado == NULL) continue;

            if (registro_passa_nos_filtros(registro_encontrado, filtros, qtd_filtros)) {
                encontrou = 1;
                print_registro(registro_encontrado);
            }
        }
        else {
            // varre todos os registros
            for (int rrn = 0; rrn < cabecalho_binario->proximo_rrn; rrn++) {
                registro_encontrado = ler_registro_RRN(arquivo_binario, rrn);
                if (registro_encontrado == NULL) continue;

                if (registro_passa_nos_filtros(registro_encontrado, filtros, qtd_filtros)) {
                    encontrou = 1;
                    print_registro(registro_encontrado);
                }
            }
        }

        if (!encontrou)
            printf("Registro inexistente.\n");

        // problema de formatação
        if (busca < qtd_buscas - 1)
            printf("\n");

        free_registro(&registro_encontrado);
        free_filtro(&filtros, qtd_filtros);
    }

    free_indice(&indices);
    free_cabecalho(&cabecalho_binario);
    return NO_ERROR;
}
