#include "update.h"

int atualizar_registro(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_atualizacoes) {

    for (int i = 0; i < qtd_atualizacoes; i++) {

        int qtd_indices = 0;
        Indice* indices = carregar_indice(arquivo_indice, &qtd_indices);

        Cabecalho* cab = novo_cabecalho();
        ler_cabecalho_binario(arquivo_binario, cab);

        int qtd_encontrados = 0;
        int* codigos = retorna_todos_codigo_estacao_do_filtro(
            arquivo_binario, indices, qtd_indices,
            cab->proximo_rrn, &qtd_encontrados
        );

        int qtd_atualiza;
        Filtro* filtros_atualiza = ler_filtros(&qtd_atualiza);

        if (codigos == NULL) {
            free_filtro(&filtros_atualiza, qtd_atualiza);
            free(indices);
            free_cabecalho(&cab);
            continue;
        }

        for (int j = 0; j < qtd_encontrados; j++) {
            int pos = busca_binaria_indice(indices, qtd_indices, codigos[j]);
            int rrn = indices[pos].RRN;
            Registro* reg = ler_registro_RRN(arquivo_binario, rrn);
            aplicar_atualizacoes(reg, filtros_atualiza, qtd_atualiza);
            fseek(arquivo_binario, 17 + (80 * rrn), SEEK_SET);
            salvar_registro_binario(arquivo_binario, reg);
            free_registro(&reg);
        }

        free(codigos);
        free(indices);
        free_cabecalho(&cab);
        free_filtro(&filtros_atualiza, qtd_atualiza);
    }

    return NO_ERROR;
}

void aplicar_atualizacoes(Registro* reg, Filtro* filtros, int qtd_filtros) {
    for (int i = 0; i < qtd_filtros; i++) {
        if (strcmp(filtros[i].campo, "nomeEstacao") == 0) {
            free(reg->nome_estacao);
            reg->nome_estacao = strdup(filtros[i].valor);
            reg->tamanho_nome_estacao = strlen(filtros[i].valor);
        }
        if (strcmp(filtros[i].campo, "nomeLinha") == 0) {
            free(reg->nome_linha);
            reg->nome_linha = strdup(filtros[i].valor);
            reg->tamanho_nome_linha = strlen(filtros[i].valor);
        }
        if (strcmp(filtros[i].campo, "codEstacao") == 0)
            reg->codigo_estacao = atoi(filtros[i].valor);
        if (strcmp(filtros[i].campo, "codLinha") == 0)
            reg->codigo_linha = atoi(filtros[i].valor);
        if (strcmp(filtros[i].campo, "codProxEstacao") == 0)
            reg->codigo_proxima_estacao = atoi(filtros[i].valor);
        if (strcmp(filtros[i].campo, "distProxEstacao") == 0)
            reg->distancia_proxima_estacao = atoi(filtros[i].valor);
        if (strcmp(filtros[i].campo, "codLinhaIntegra") == 0)
            reg->codigo_linha_integracao = atoi(filtros[i].valor);
        if (strcmp(filtros[i].campo, "codEstIntegra") == 0)
            reg->codigo_estacao_integracao = atoi(filtros[i].valor);
    }
}