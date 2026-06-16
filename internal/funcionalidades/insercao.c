#include "insercao.h"

int inserir_novos_registros(FILE* arquivo_binario, FILE* arquivo_indice, int qtd_insercao) {
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

    int qtd_estacoes = 0;
    char** estacoes = malloc(sizeof(char*) * cabecalho_binario->proximo_rrn);

    int qtd_pares = 0;
    ParEstacao* pares = malloc(sizeof(ParEstacao) * cabecalho_binario->proximo_rrn);

    // pega todas as estações e pares de estações para uso posterior
    conta_estacao_e_pares(arquivo_binario, cabecalho_binario->proximo_rrn, estacoes, pares, &qtd_estacoes, &qtd_pares);

    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF);

    for (int insercao = 0; insercao < qtd_insercao; insercao++) {
        char linha[300];
        if (!fgets(linha, 300, stdin)) {
            printf("Fail\n");
            break;
        }

        Registro* novo_registro = tokenizar_registro_com_aspas(linha);

        if (cabecalho_binario->topo_pilha != -1) {
            Registro* ultimo_removido = ler_registro_RRN(arquivo_binario, cabecalho_binario->topo_pilha);
            cabecalho_binario->topo_pilha = ultimo_removido->proximo_registro;
            free_registro(&ultimo_removido);

            // Volta o registro que acabamos de ler
            fseek(arquivo_binario, -TAM_REGISTRO_DADOS, SEEK_CUR);
        }
        else {
            int byte_offset = TAM_CABECALHO_REGISTRO + cabecalho_binario->proximo_rrn * TAM_REGISTRO_DADOS;
            fseek(arquivo_binario, byte_offset, SEEK_SET);
        }

        int qtd_max_bytes = 10 + novo_registro->tamanho_nome_estacao + novo_registro->tamanho_nome_linha;
        if (salvar_registro_binario(arquivo_binario, novo_registro) < qtd_max_bytes) {
            free_registro(&novo_registro);
            return FILE_WRITE_ERROR;
        }

        free_registro(&novo_registro);
    }

    conta_estacao_e_pares(arquivo_binario, cabecalho_binario->proximo_rrn, estacoes, pares, &qtd_estacoes, &qtd_pares);

    // salva os numeros de estações e pares
    cabecalho_binario->numero_estacoes = qtd_estacoes;
    cabecalho_binario->numero_pares_estacoes = qtd_pares;
    salvar_cabecalho(arquivo_binario, cabecalho_binario);
    free_cabecalho(&cabecalho_binario);

    // salva as ultimas alterações no arquivo de indice
    gerar_arquivo_indice(arquivo_binario, arquivo_indice);

    for (int i = 0; i < qtd_estacoes; i++)
        free(estacoes[i]);
    free(estacoes);
    free(pares);

    return NO_ERROR;
}