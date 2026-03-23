#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"

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

void escrever_registros_csv(FILE* arquivo_csv, FILE* arquivo_binario) {
    // Precisa fazer:
    // - criar os cabeçalhos
    // - adicionar cada linha tokenizando os campos
    // - lembrar de atualizar tanto o RRN quando o topo da pilha

    int topo_pilha = -1;
    int proximo_RRN = 0;
    int numero_estacoes = 0;
    int numero_pares_estacoes = 0;

    // Escreve os registros de cabeçalho
    fwrite(STATUS_OK, sizeof(char), 1, arquivo_binario);
    fwrite(topo_pilha, sizeof(int), 1, arquivo_binario);
    fwrite(proximo_RRN, sizeof(int), 1, arquivo_binario);
    fwrite(numero_estacoes, sizeof(int), 1, arquivo_binario);
    fwrite(numero_pares_estacoes, sizeof(int), 1, arquivo_binario);

    char buffer[200];
    fgets(buffer, sizeof(buffer), arquivo_csv); // Lê a primeira linha, que no caso contem apenas os nomes das colunas, que é irrelevante

    // Lê o arquivo linha por linha
    while(fgets(buffer, sizeof(buffer), arquivo_csv)) {
        char* token = strtok(buffer, ",");
    }
}

Registro* ler_registro_RRN(char* nome_arquivo, int RRN) {
    // Abre o arquivo no modo escrita/leitura binária
    FILE* arquivo = fopen(nome_arquivo, MODO_LEITURA_BINARIO);

    // faz alguma coisa

    fclose(arquivo);
    return 0;
}