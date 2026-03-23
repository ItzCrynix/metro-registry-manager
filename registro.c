#include<stdio.h>
#include<string.h>
#include"registro.h"

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

int escrever_registro_csv(char* nome_arquivo_csv, char* nome_arquivo_binario) {
    // Abre o arquivo binario no modo escrita/leitura binária
    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_ESCRITA_BINARIO);
    if (arquivo_binario == NULL) return FILE_NOT_FOUND_ERROR;

    FILE* arquivo_csv = fopen(nome_arquivo_csv, "r");
    if (arquivo_csv == NULL) return FILE_NOT_FOUND_ERROR;

    // Precisa fazer:
    // - criar os cabeçalhos
    // - adicionar cada linha tokenizando os campos
    // - lembrar de atualizar tanto o RRN quando o topo da pilha

    fclose(arquivo_binario);
    return 0;
}

Registro* ler_registro_RRN(char* nome_arquivo, int RRN) {
    // Abre o arquivo no modo escrita/leitura binária
    FILE* arquivo = fopen(nome_arquivo, MODO_LEITURA_BINARIO);

    // faz alguma coisa

    fclose(arquivo);
    return 0;
}