#include "funcionalidades.h"
#include <ctype.h>

//isso o gemini fez pra mim, pq eu nunca pensaria nisso, até pq eu nunca usei direito



typedef struct parestacao {
    int estacao;
    int proxima_estacao;
} ParEstacao;

int busca_par_estacao(ParEstacao* pares, int tam, ParEstacao procurado) {
    for (int i = 0; i < tam; i++) {
        if (pares[i].estacao == procurado.estacao && pares[i].proxima_estacao == procurado.proxima_estacao)
            return i;
    }

    return -1;
}

int busca_estacao(char** estacoes, int tam, char* procurado) {
    for (int i = 0; i < tam; i++) {
        if (estacoes[i] == NULL) break;

        if (strcmp(estacoes[i], procurado) == 0) {
            return i;
        }
    }

    return -1;
}

int escrever_registros_csv(FILE* arquivo_csv, FILE* arquivo_binario) {
    if (arquivo_binario == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    if (arquivo_csv == NULL) {
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
        if (busca_estacao(estacoes, 1000, novo_registro->nome_estacao) == -1) {
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
            if (busca_par_estacao(pares, qtd_pares_estacoes, novo_par) == -1) {
                pares[qtd_pares_estacoes].estacao = novo_par.estacao;
                pares[qtd_pares_estacoes].proxima_estacao = novo_par.proxima_estacao;
                qtd_pares_estacoes++;
            }
        }

        // marca o registro como não removido e guarda o ultimo valor da pilha nele
        novo_registro->removido = STATUS_NOT_REMOVED;
        novo_registro->proximo_registro = cabecalho_binario->topo_pilha; // Obs: o topo sempre será -1, pois estamos apenas inserindo novos registros

        salvar_registro_binario(arquivo_binario, novo_registro);
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

    Cabecalho* cabecalho_binario = ler_cabecalho_binario(arquivo_binario);
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
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

        // formata a string do registro e printa
        char* registro_formatado = to_string(registro_atual);
        printf("%s\n", registro_formatado);

        free_registro(&registro_atual);
        free(registro_formatado);
    }

    free_cabecalho(&cabecalho_binario);

    return NO_ERROR;
}

int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int rrn) {
    if (arquivo_binario == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    Cabecalho* cabecalho_binario = ler_cabecalho_binario(arquivo_binario);
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
    }

 if (rrn < 0 || rrn >= cabecalho_binario->proximo_rrn) {
        free_cabecalho(&cabecalho_binario);
        return INVALID_RRN_ERROR;
    }

    *registro = ler_registro_RRN(arquivo_binario, rrn);
    if (*registro == NULL) {
        return NO_DATA_FOUND_ERROR;
    }

    return NO_ERROR;
}


int buscar_registro_Filtro(const char* nome_arquivo_binario, int quantidade_buscas, FILE* arquivo_binario, Cabecalho* cabecalho) {

    while (quantidade_buscas > 0) {
        int quantidade_campos = 0;
        scanf("%d", &quantidade_campos);

        char** campos = (char**) malloc(sizeof(char*) * quantidade_campos);
        char** valores = (char**) malloc(sizeof(char*) * quantidade_campos);
        if (campos == NULL || valores == NULL) {
            free(campos);
            free(valores);
            free_cabecalho(&cabecalho);
            fclose(arquivo_binario);
            return MALLOC_ERROR;
        }

        for (int i = 0; i < quantidade_campos; i++) {
            campos[i] = (char*) malloc(sizeof(char) * 100);
            valores[i] = (char*) malloc(sizeof(char) * 100);
            if (campos[i] == NULL || valores[i] == NULL) {
                for (int j = 0; j <= i; j++) {
                    free(campos[j]);
                    free(valores[j]);
                }
                free(campos);
                free(valores);
                free_cabecalho(&cabecalho);
                fclose(arquivo_binario);
                return MALLOC_ERROR;
            }
            scanf("%s", campos[i]);
            ScanQuoteString(valores[i]);
        }
        //feito

        int encontrou_pelo_menos_um = 0;
        for (int rrn = 0; rrn < cabecalho->proximo_rrn; rrn++) {
            Registro* reg = ler_registro_RRN(arquivo_binario, rrn);
            if (reg == NULL) {
                continue;
            }
            int passou_no_filtro = 1;
            passou_no_filtro = passou_no_Filtro(quantidade_campos, campos, valores, reg);
           

            if (passou_no_filtro) {
                char* texto = to_string(reg);
                if (texto != NULL) {
                    printf("%s\n", texto);
                    free(texto);
                }
                encontrou_pelo_menos_um = 1;
            }

            free_registro(&reg);
        }

        if (!encontrou_pelo_menos_um) {
            printf("Registro inexistente.\n");
        }

        for (int i = 0; i < quantidade_campos; i++) {
            free(campos[i]);
            free(valores[i]);
        }
        free(campos);
        free(valores);

        quantidade_buscas--;
    }

    free_cabecalho(&cabecalho);
    fclose(arquivo_binario);
    return NO_ERROR;
}