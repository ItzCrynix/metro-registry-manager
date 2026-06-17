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

    free_indice(&indices);

    int cap = cabecalho_binario->proximo_rrn + qtd_insercao;
    char** estacoes = calloc(cap, sizeof(char*));
    int n_nomes = 0;
    ParEstacao* pares = malloc(sizeof(ParEstacao) * cap);
    int n_pares = 0;

    for (int rrn = 0; rrn < cabecalho_binario->proximo_rrn; rrn++) {
        Registro* temp = ler_registro_RRN(arquivo_binario, rrn);
        if (temp == NULL) continue;

        if (busca_estacao(estacoes, n_nomes, temp->nome_estacao) == NO_DATA_FOUND_ERROR)
            estacoes[n_nomes++] = strdup(temp->nome_estacao);

        if (temp->codigo_proxima_estacao != -1) {
            ParEstacao par = {.estacao = temp->codigo_estacao, .proxima_estacao = temp->codigo_proxima_estacao};
            if (busca_par_estacao(pares, n_pares, par) == NO_DATA_FOUND_ERROR) {
                pares[n_pares].estacao = par.estacao;
                pares[n_pares].proxima_estacao = par.proxima_estacao;
                n_pares++;
            }
        }

        free_registro(&temp);
    }

    int qtd_estacoes = cabecalho_binario->numero_estacoes;
    int qtd_pares = cabecalho_binario->numero_pares_estacoes;

    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF);

    for (int insercao = 0; insercao < qtd_insercao; insercao++) {
        char linha[300];
        if (!fgets(linha, 300, stdin)) {
            break;
        }

        Registro* novo_registro = tokenizar_registro_com_aspas(linha);

        int byte_offset;
        if (cabecalho_binario->topo_pilha != -1) {
            byte_offset = TAM_CABECALHO_REGISTRO + cabecalho_binario->topo_pilha * TAM_REGISTRO_DADOS;

            int proximo_da_pilha;
            fseek(arquivo_binario, byte_offset + sizeof(char), SEEK_SET);
            fread(&proximo_da_pilha, sizeof(int), 1, arquivo_binario);

            cabecalho_binario->topo_pilha = proximo_da_pilha;

            fseek(arquivo_binario, byte_offset, SEEK_SET);
        }
        else {
            byte_offset = TAM_CABECALHO_REGISTRO + cabecalho_binario->proximo_rrn * TAM_REGISTRO_DADOS;
            fseek(arquivo_binario, byte_offset, SEEK_SET);

            cabecalho_binario->proximo_rrn++;
        }

        int qtd_max_bytes = 10 + novo_registro->tamanho_nome_estacao + novo_registro->tamanho_nome_linha;
        if (salvar_registro_binario(arquivo_binario, novo_registro) < qtd_max_bytes) {
            free_registro(&novo_registro);
            free_cabecalho(&cabecalho_binario);
            for (int i = 0; i < n_nomes; i++) free(estacoes[i]);
            free(estacoes);
            free(pares);
            return FILE_WRITE_ERROR;
        }

        // Atualiza os contadores apenas para estações/pares ainda não vistos
        if (busca_estacao(estacoes, n_nomes, novo_registro->nome_estacao) == NO_DATA_FOUND_ERROR) {
            estacoes[n_nomes++] = strdup(novo_registro->nome_estacao);
            qtd_estacoes++;
        }

        if (novo_registro->codigo_proxima_estacao != -1) {
            ParEstacao par = {.estacao = novo_registro->codigo_estacao, .proxima_estacao = novo_registro->codigo_proxima_estacao};
            if (busca_par_estacao(pares, n_pares, par) == NO_DATA_FOUND_ERROR) {
                pares[n_pares].estacao = par.estacao;
                pares[n_pares].proxima_estacao = par.proxima_estacao;
                n_pares++;
                qtd_pares++;
            }
        }

        free_registro(&novo_registro);
    }

    cabecalho_binario->numero_estacoes = qtd_estacoes;
    cabecalho_binario->numero_pares_estacoes = qtd_pares;
    salvar_cabecalho(arquivo_binario, cabecalho_binario);
    free_cabecalho(&cabecalho_binario);

    // salva as ultimas alterações no arquivo de indice
    gerar_arquivo_indice(arquivo_binario, arquivo_indice);

    for (int i = 0; i < n_nomes; i++)
        free(estacoes[i]);
    free(estacoes);
    free(pares);

    return NO_ERROR;
}