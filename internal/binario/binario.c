#include "binario.h"

int escrever_csv_para_binario(FILE* arquivo_csv, FILE* arquivo_binario) {
    if (arquivo_binario == NULL || arquivo_csv == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    // Cria um arquivo para cuidar das mudanças do cabeçalho
    Cabecalho* cabecalho_binario = novo_cabecalho();
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
    }

    salvar_cabecalho(arquivo_binario, cabecalho_binario); // escreve o cabeçalho incial

    char buffer[200];
    fgets(buffer, sizeof(buffer), arquivo_csv);  // Descarta a primeira linha, que contem o nome das colunas

    int qtd_pares_estacoes = 0;
    int cap_par = 10;
    ParEstacao* pares = (ParEstacao*) malloc(sizeof(ParEstacao) * cap_par);

    int qtd_estacoes = 0;
    char** estacoes = (char**) calloc(1000, sizeof(char*));

    int qtd_registros = 0;
    // Lê o arquivo csv linha por linha
    while(fgets(buffer, sizeof(buffer), arquivo_csv)) {

        // tokeniza o registro e salva numa struct nova
        Registro* novo_registro = tokenizar_registro(buffer);
        if (novo_registro == NULL) {
            return MALLOC_ERROR;
        }

        // verifica se há uma estação na lista, e adiciona caso nao tenha
        if (busca_estacao(estacoes, 1000, novo_registro->nome_estacao) == NO_DATA_FOUND_ERROR) {
            estacoes[qtd_estacoes] = strdup(novo_registro->nome_estacao);
            qtd_estacoes++;
        }

        if (novo_registro->codigo_proxima_estacao != -1) {
            ParEstacao novo_par = {.estacao = novo_registro->codigo_estacao, .proxima_estacao = novo_registro->codigo_proxima_estacao};

            // aumenta o tamanho do array de pares de estação
            if (qtd_pares_estacoes >= cap_par) {
                cap_par += 10;
                pares = (ParEstacao*) realloc(pares, sizeof(ParEstacao) * cap_par);
            }

            // verifica se há um par na lista, e adiciona o novo par caso nao tenha
            if (busca_par_estacao(pares, qtd_pares_estacoes, novo_par) == NO_DATA_FOUND_ERROR) {
                pares[qtd_pares_estacoes].estacao = novo_par.estacao;
                pares[qtd_pares_estacoes].proxima_estacao = novo_par.proxima_estacao;
                qtd_pares_estacoes++;
            }
        }

        // marca o registro como não removido e guarda o ultimo valor da pilha nele
        novo_registro->removido = STATUS_NOT_REMOVED;
        novo_registro->proximo_registro = cabecalho_binario->topo_pilha; // Obs: o topo sempre será -1, pois estamos apenas inserindo novos registros

        int qtd_max_bytes = 10 + novo_registro->tamanho_nome_estacao + novo_registro->tamanho_nome_linha;
        if (salvar_registro_binario(arquivo_binario, novo_registro) < qtd_max_bytes) {
            return FILE_WRITE_ERROR;
        }
        qtd_registros++;

        free_registro(&novo_registro);
    }

    // Salva as ultimas alterações feitas do cabeçalho e libera a memória
    cabecalho_binario->status = STATUS_CONSISTENT;
    cabecalho_binario->proximo_rrn = qtd_registros;
    cabecalho_binario->numero_estacoes = qtd_estacoes;
    cabecalho_binario->numero_pares_estacoes = qtd_pares_estacoes;

    salvar_cabecalho(arquivo_binario, cabecalho_binario);
    
    free_cabecalho(&cabecalho_binario);
    free(pares);

    for (int i = 0; i < 1000; i++)
        free(estacoes[i]);
    free(estacoes);

    return NO_ERROR;
}

int printar_arquivo_binario(FILE* arquivo_binario) {
    if (arquivo_binario == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    Cabecalho* cabecalho_binario = novo_cabecalho();
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
    }

    if (ler_cabecalho_binario(arquivo_binario, cabecalho_binario) < NUM_CAMPOS_CABECALHO) {
        return FILE_READ_ERROR;
    }

    if (cabecalho_binario->status == STATUS_INCONSISTENT) {
        return INCOSISTENT_FILE_ERROR;
    }

    // Caso nenhum registro tenha sido escrito no arquivo ainda
    if (cabecalho_binario->proximo_rrn == 0) {
        return NO_DATA_FOUND_ERROR;
    }

    // itera sobre todos os registros disponíveis 
    int RRN_atual = 0;
    while (RRN_atual < cabecalho_binario->proximo_rrn) {
        Registro* registro_atual = ler_registro_RRN(arquivo_binario, RRN_atual++);

        // Caso tenha registro excluido
        if (registro_atual == NULL) {
            continue;
        }

        print_registro(registro_atual);

        free_registro(&registro_atual);
    }

    free_cabecalho(&cabecalho_binario);

    return NO_ERROR;
}