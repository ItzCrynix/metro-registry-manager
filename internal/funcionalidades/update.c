#include "update.h"

// aplica os campos do filtro de atualizacao no registro em memoria
// trata "NULO": campos inteiros viram -1, campos string viram vazios
void aplicar_atualizacoes(Registro* reg, Filtro* filtros, int qtd_filtros) {
    for (int i = 0; i < qtd_filtros; i++) {
        char* campo = filtros[i].campo;
        char* valor = filtros[i].valor;
        int eh_nulo = (strcmp(valor, "NULO") == 0);

        if (strcmp(campo, "nomeEstacao") == 0) {
            free(reg->nome_estacao);
            if (eh_nulo) {
                reg->nome_estacao = strdup("");
                reg->tamanho_nome_estacao = 0;
            } else {
                reg->nome_estacao = strdup(valor);
                reg->tamanho_nome_estacao = strlen(valor);
            }
        }
        if (strcmp(campo, "nomeLinha") == 0) {
            free(reg->nome_linha);
            if (eh_nulo) {
                reg->nome_linha = strdup("");
                reg->tamanho_nome_linha = 0;
            } else {
                reg->nome_linha = strdup(valor);
                reg->tamanho_nome_linha = strlen(valor);
            }
        }
        if (strcmp(campo, "codEstacao") == 0)
            reg->codigo_estacao = eh_nulo ? -1 : atoi(valor);
        if (strcmp(campo, "codLinha") == 0)
            reg->codigo_linha = eh_nulo ? -1 : atoi(valor);
        if (strcmp(campo, "codProxEstacao") == 0)
            reg->codigo_proxima_estacao = eh_nulo ? -1 : atoi(valor);
        if (strcmp(campo, "distProxEstacao") == 0)
            reg->distancia_proxima_estacao = eh_nulo ? -1 : atoi(valor);
        if (strcmp(campo, "codLinhaIntegra") == 0)
            reg->codigo_linha_integracao = eh_nulo ? -1 : atoi(valor);
        if (strcmp(campo, "codEstIntegra") == 0)
            reg->codigo_estacao_integracao = eh_nulo ? -1 : atoi(valor);
    }
}

int atualizar_registro(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_atualizacoes) {
    if (arquivo_binario == NULL || arquivo_indice == NULL)
        return FILE_NOT_FOUND_ERROR;

    Cabecalho* cab = novo_cabecalho();
    if (cab == NULL)
        return MALLOC_ERROR;

    if (ler_cabecalho_binario(arquivo_binario, cab) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cab);
        return FILE_READ_ERROR;
    }

    if (cab->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cab);
        return INCOSISTENT_FILE_ERROR;
    }

    // marca como inconsistente durante a edicao
    cab->status = STATUS_INCONSISTENT;
    salvar_cabecalho(arquivo_binario, cab);

    for (int i = 0; i < qtd_atualizacoes; i++) {
        int qtd_indices = 0;
        Indice* indices = carregar_indice(arquivo_indice, &qtd_indices);

        int qtd_encontrados = 0;
        int* codigos = retorna_todos_codigo_estacao_do_filtro(
            arquivo_binario, indices, qtd_indices,
            cab->proximo_rrn, &qtd_encontrados);

        int qtd_atualiza = 0;
        Filtro* filtros_atualiza = ler_filtros(&qtd_atualiza);

        if (codigos == NULL) {
            free_filtro(&filtros_atualiza, qtd_atualiza);
            free_indice(&indices);
            continue;
        }

        for (int j = 0; j < qtd_encontrados; j++) {
            int pos = busca_binaria_indice(indices, qtd_indices, codigos[j]);
            if (pos == -1)
                continue;

            int rrn = indices[pos].RRN;
            Registro* reg = ler_registro_RRN(arquivo_binario, rrn);
            if (reg == NULL)
                continue;

            aplicar_atualizacoes(reg, filtros_atualiza, qtd_atualiza);

            fseek(arquivo_binario, TAM_CABECALHO_REGISTRO + TAM_REGISTRO_DADOS * rrn, SEEK_SET);
            salvar_registro_binario(arquivo_binario, reg);

            free_registro(&reg);
        }

        free(codigos);
        free_indice(&indices);
        free_filtro(&filtros_atualiza, qtd_atualiza);
    }

    fflush(arquivo_binario);

    // reconstroi o indice do zero, pois o codEstacao pode ter sido alterado
    Indice* novo_indice = malloc(sizeof(Indice) * cab->proximo_rrn);
    int pos_novo = 0;

    for (int rrn = 0; rrn < cab->proximo_rrn; rrn++) {
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
        organiza_lista_indice(novo_indice, pos_novo);

        free_registro(&temp);
    }

    salvar_indices(arquivo_indice, novo_indice, pos_novo);
    fflush(arquivo_indice);
    ftruncate(fileno(arquivo_indice), TAM_CABECALHO_INDICE + TAM_REGISTRO_INDICE * pos_novo);
    free(novo_indice);

    cab->status = STATUS_CONSISTENT;
    if (salvar_cabecalho(arquivo_binario, cab) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cab);
        return FILE_WRITE_ERROR;
    }

    free_cabecalho(&cab);
    return NO_ERROR;
}
