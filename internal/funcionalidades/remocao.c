#include "remocao.h"

int remover_registros_com_indice(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_remocoes) {
    if (arquivo_binario == NULL || arquivo_indice == NULL)
        return FILE_NOT_FOUND_ERROR;

    Cabecalho* cabecalho_binario = novo_cabecalho();
    int qtd_indices = 0;
    Indice* indices = carregar_indice(arquivo_indice, &qtd_indices);
    if (cabecalho_binario == NULL || indices == NULL)
        return MALLOC_ERROR;

    if (ler_cabecalho_binario(arquivo_binario, cabecalho_binario) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cabecalho_binario);
        return FILE_READ_ERROR;
    }

    if (cabecalho_binario->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cabecalho_binario);
        return INCOSISTENT_FILE_ERROR;
    }

    cabecalho_binario->status = STATUS_INCONSISTENT;
    salvar_cabecalho(arquivo_binario, cabecalho_binario);

    int algo_removido = 0;

    for (int remocao = 0; remocao < qtd_remocoes; remocao++) {
        int qtd_encontrados = 0;
        int* cod_para_remover = retorna_todos_codigo_estacao_do_filtro(arquivo_binario, indices, qtd_indices, cabecalho_binario->proximo_rrn, &qtd_encontrados);

        if (cod_para_remover == NULL)
            continue;

        for (int i = 0; i < qtd_encontrados; i++) {
            int pos_indice = busca_binaria_indice(indices, qtd_indices, cod_para_remover[i]);
            if (pos_indice == -1)
                continue;

            int rrn = indices[pos_indice].RRN;
            int byte_offset = TAM_CABECALHO_REGISTRO + TAM_REGISTRO_DADOS * rrn;

            fseek(arquivo_binario, byte_offset, SEEK_SET);
            char removido;
            fread(&removido, sizeof(char), 1, arquivo_binario);
            if (removido == STATUS_REMOVED)
                continue;

            fseek(arquivo_binario, byte_offset, SEEK_SET);
            removido = STATUS_REMOVED;
            fwrite(&removido, sizeof(char), 1, arquivo_binario);
            fwrite(&cabecalho_binario->topo_pilha, sizeof(int), 1, arquivo_binario);

            cabecalho_binario->topo_pilha = rrn;
            algo_removido = 1;
        }

        free(cod_para_remover);
    }

    free_indice(&indices);
    fflush(arquivo_binario);

    // reconstroi o indice do zero varrendo o binario
    Indice* novo_indice = malloc(sizeof(Indice) * cabecalho_binario->proximo_rrn);
    int pos_novo = 0;

    for (int rrn = 0; rrn < cabecalho_binario->proximo_rrn; rrn++) {
        int byte_offset = TAM_CABECALHO_REGISTRO + TAM_REGISTRO_DADOS * rrn;
        fseek(arquivo_binario, byte_offset, SEEK_SET);

        char removido;
        fread(&removido, sizeof(char), 1, arquivo_binario);
        if (removido == STATUS_REMOVED)
            continue;

        Registro* temp = ler_registro_RRN(arquivo_binario, rrn);
        if (temp == NULL)
            continue;

        novo_indice[pos_novo].id  = temp->codigo_estacao;
        novo_indice[pos_novo].RRN = rrn;
        pos_novo++;

        free_registro(&temp);
    }

    organiza_lista_indice(novo_indice, pos_novo);

    salvar_indices(arquivo_indice, novo_indice, pos_novo);
    long tamanho_indice = TAM_CABECALHO_INDICE + TAM_REGISTRO_INDICE * pos_novo;
    fflush(arquivo_indice);
    ftruncate(fileno(arquivo_indice), tamanho_indice);
    free(novo_indice);

    // so recalcula os contadores se algo foi de fato removido
    if (algo_removido) {
        int qtd_estacoes = 0;
        char** estacoes = malloc(sizeof(char*) * (pos_novo > 0 ? pos_novo : 1));

        int qtd_pares = 0;
        ParEstacao* pares = malloc(sizeof(ParEstacao) * (pos_novo > 0 ? pos_novo : 1));

        for (int rrn = 0; rrn < cabecalho_binario->proximo_rrn; rrn++) {
            Registro* temp = ler_registro_RRN(arquivo_binario, rrn);
            if (temp == NULL)
                continue;

            if (busca_estacao(estacoes, qtd_estacoes, temp->nome_estacao) == NO_DATA_FOUND_ERROR) {
                estacoes[qtd_estacoes] = strdup(temp->nome_estacao);
                qtd_estacoes++;
            }

            if (temp->codigo_proxima_estacao != -1) {
                ParEstacao novo_par = {.estacao = temp->codigo_estacao,
                                       .proxima_estacao = temp->codigo_proxima_estacao};
                if (busca_par_estacao(pares, qtd_pares, novo_par) == NO_DATA_FOUND_ERROR) {
                    pares[qtd_pares].estacao = novo_par.estacao;
                    pares[qtd_pares].proxima_estacao = novo_par.proxima_estacao;
                    qtd_pares++;
                }
            }

            free_registro(&temp);
        }

        for (int i = 0; i < qtd_estacoes; i++)
            free(estacoes[i]);
        free(estacoes);
        free(pares);

        cabecalho_binario->numero_estacoes = qtd_estacoes;
        cabecalho_binario->numero_pares_estacoes = qtd_pares;
    }

    cabecalho_binario->status = STATUS_CONSISTENT;
    if (salvar_cabecalho(arquivo_binario, cabecalho_binario) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cabecalho_binario);
        return FILE_WRITE_ERROR;
    }

    free_cabecalho(&cabecalho_binario);
    return NO_ERROR;
}
