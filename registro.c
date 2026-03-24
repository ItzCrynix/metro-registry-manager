#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"
#include "utils.h"

struct registro {
    char removido;
    int proximo_registro;
    int codigo_estacao;
    int codigo_linha;
    int codigo_proxima_estacao;
    int distancia_proxima_estacao;
    int codigo_linha_integracao;
    int codigo_estacao_integracao;

    int tamanho_nome_estacao;
    char* nome_estacao;

    int tamanho_nome_linha;
    char* nome_linha;
};

typedef struct cabecalho {
    char status;
    int topo_pilha;
    int proximo_rrn;
    int numero_estacoes;
    int numero_pares_estacoes;
} Cabecalho;

Cabecalho* novo_cabecalho() {
    Cabecalho* novo = (Cabecalho*) malloc(sizeof(Cabecalho));
    if (novo == NULL) {
        return NULL;
    }

    novo->status = STATUS_OK;
    novo->topo_pilha = -1;
    novo->proximo_rrn = 0;
    novo->numero_estacoes = 0;
    novo->numero_pares_estacoes = 0;

    return novo;
}

void escrever_cabecalho(FILE* arquivo_binario, Cabecalho* cabecalho_binario) {
    fseek(arquivo_binario, 0, 0);
    // Escreve os registros de cabeçalho
    fwrite(&cabecalho_binario->status, sizeof(char), 1, arquivo_binario);
    fwrite(&cabecalho_binario->topo_pilha, sizeof(int), 1, arquivo_binario);
    fwrite(&cabecalho_binario->proximo_rrn, sizeof(int), 1, arquivo_binario);
    fwrite(&cabecalho_binario->numero_estacoes, sizeof(int), 1, arquivo_binario);
    fwrite(&cabecalho_binario->numero_pares_estacoes, sizeof(int), 1, arquivo_binario);
}

Registro* tokenizar_registro(char* buffer) {
    Registro* registro_temporario = (Registro*) malloc(sizeof(Registro));
    if (registro_temporario == NULL) {
        return NULL;
    }

    char* token = strtok(buffer, ",");
    registro_temporario->codigo_estacao = integer_or_null(token);

    token = strtok(NULL, ",");
    registro_temporario->nome_estacao = token;
    registro_temporario->tamanho_nome_estacao = strlen(token);

    token = strtok(NULL, ",");
    registro_temporario->codigo_linha = integer_or_null(token);

    token = strtok(NULL, ",");
    registro_temporario->nome_linha = token;
    registro_temporario->tamanho_nome_linha = strlen(token);

    token = strtok(NULL, ",");
    registro_temporario->codigo_proxima_estacao = integer_or_null(token);

    token = strtok(NULL, ",");
    registro_temporario->distancia_proxima_estacao = integer_or_null(token);

    token = strtok(NULL, ",");
    registro_temporario->codigo_linha_integracao = integer_or_null(token);

    token = strtok(NULL, ",");
    registro_temporario->codigo_estacao_integracao = integer_or_null(token);

    return registro_temporario;
}

int escrever_registros_csv(FILE* arquivo_csv, FILE* arquivo_binario) {
    // Cria um arquivo para cuidar das mudanças do cabeçalho, para salvar depois
    Cabecalho* cabecalho_binario = novo_cabecalho();
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
    }

    // escreve o cabeçalho incial
    escrever_cabecalho(arquivo_binario, cabecalho_binario);

    char buffer[200];
    fgets(buffer, sizeof(buffer), arquivo_csv); // Lê a primeira linha, que no caso contem apenas os nomes das colunas, que é irrelevante

    // Lê o arquivo linha por linha
    while(fgets(buffer, sizeof(buffer), arquivo_csv)) {
        Registro* novo_registro = tokenizar_registro(buffer);
        if (novo_registro == NULL) {
            return MALLOC_ERROR;
        }

        // marca o registro como não removido e salva o ultimo valor da pilha nele
        novo_registro->removido = 'n';
        novo_registro->proximo_registro = cabecalho_binario->topo_pilha;

        // salva o valor do rrn atual no topo da pilha e incrementa o rrn
        cabecalho_binario->topo_pilha = cabecalho_binario->proximo_rrn++;

        fwrite(&novo_registro->removido, sizeof(char), 1, arquivo_binario);

        int bytes_remanescentes = TAM_REGISTRO_DADOS - (sizeof(int) * 9) - (sizeof(char) * (1 + novo_registro->tamanho_nome_estacao + novo_registro->tamanho_nome_linha));

        free(novo_registro);
    }

    // Salva as ultimas alterações feitas do cabeçalho
    escrever_cabecalho(arquivo_binario, cabecalho_binario);

    free(cabecalho_binario);
    return 0;
}

Registro* ler_registro_RRN(char* nome_arquivo, int RRN) {
    // Abre o arquivo no modo escrita/leitura binária
    FILE* arquivo = fopen(nome_arquivo, MODO_LEITURA_BINARIO);

    // faz alguma coisa

    fclose(arquivo);
    return 0;
}