#include "remocao.h"

int remover_registros_com_indice(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_remocoes) {
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

    for (int remocao = 0; remocao < qtd_remocoes; remocao++) {
        int qtd_encontrados = 0;
        int* cod_para_remover = retorna_todos_codigo_estacao_do_filtro(arquivo_binario, indices, qtd_indices, cabecalho_binario->proximo_rrn, &qtd_encontrados);
        if (cod_para_remover == NULL || qtd_encontrados == 0) {
            return NO_DATA_FOUND_ERROR;
        }

        for (int i = 0; i < qtd_encontrados; i++) {
            int pos_indice = busca_binaria_indice(indices, qtd_indices, cod_para_remover[i]);

            int byte_offset = TAM_CABECALHO_REGISTRO + TAM_REGISTRO_DADOS * indices[pos_indice].RRN;
            fseek(arquivo_binario, byte_offset, SEEK_SET);

            char removido;
            fread(&removido, sizeof(char), 1, arquivo_binario);
            if (removido == STATUS_REMOVED) continue;

            fseek(arquivo_binario, byte_offset, SEEK_SET);

            removido = STATUS_REMOVED;
            fwrite(&removido, sizeof(char), 1, arquivo_binario);
            fwrite(&cabecalho_binario->topo_pilha, sizeof(int), 1, arquivo_binario);

            cabecalho_binario->topo_pilha = indices[pos_indice].RRN;
        }

        // Nesta parte ele atualiza os indices pra tirar os removidos
        Indice* novo_indice = malloc(sizeof(Indice) * qtd_indices);

        int pos_aux_codigos = 0;
        int pos_novo_indice = 0;
        for (int i = 0; i < qtd_indices; i++) {
            // verifica se o codigo atual é igual ao do indice
            if (busca_binaria_indice(indices, qtd_indices, cod_para_remover[pos_aux_codigos]) != -1) {
                pos_aux_codigos++;
                continue;
            }

            // copia somente os indices que resta
            novo_indice[pos_novo_indice].id = indices[i].id;
            novo_indice[pos_novo_indice].RRN = indices[i].RRN;
            pos_novo_indice++;
        }

        free_indice(&indices);
        indices = novo_indice;
        qtd_indices = pos_novo_indice;

        free(cod_para_remover);
    }

    salvar_indices(arquivo_indice, indices, qtd_indices);
    free_indice(&indices);

    long tamanho_atual = TAM_CABECALHO_INDICE + TAM_REGISTRO_INDICE * qtd_indices;

    // limpa o que sobrar no final do arquivo
    fflush(arquivo_indice);
    ftruncate(fileno(arquivo_indice), tamanho_atual);

    if (salvar_cabecalho(arquivo_binario, cabecalho_binario) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cabecalho_binario);
        return FILE_WRITE_ERROR;
    }

    free_cabecalho(&cabecalho_binario);
    return NO_ERROR;
}