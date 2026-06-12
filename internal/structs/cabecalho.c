#include <stdlib.h>
#include <stdio.h>
#include "cabecalho.h"

Cabecalho* novo_cabecalho() {
    Cabecalho* novo = (Cabecalho*) malloc(sizeof(Cabecalho));
    if (!novo) {
        return NULL;
    }

    // escreve os valores padrões que o cabeçalho deve assumir em um arquivo vazio
    // O status precisa estar incosistent, pois é de se esperar que, ao criarmos um cabeçalho novo, iremos escrevê-lo ou ler em um arquivo
    novo->status = STATUS_INCONSISTENT;
    novo->topo_pilha = -1;
    novo->proximo_rrn = 0;
    novo->numero_estacoes = 0;
    novo->numero_pares_estacoes = 0;

    return novo;
}

void free_cabecalho(Cabecalho** cabecalho) {
    if (*cabecalho == NULL) return;

    free(*cabecalho);

    *cabecalho = NULL;
}

int salvar_cabecalho(FILE* arquivo_binario, Cabecalho* cabecalho_binario) {
    // volta o ponteiro para o inicio do arquivo
    rewind(arquivo_binario);

    size_t campos_salvos = 0;

    // Escreve os registros de cabeçalho
    campos_salvos += fwrite(&cabecalho_binario->status, sizeof(char), 1, arquivo_binario);
    campos_salvos += fwrite(&cabecalho_binario->topo_pilha, sizeof(int), 1, arquivo_binario);
    campos_salvos += fwrite(&cabecalho_binario->proximo_rrn, sizeof(int), 1, arquivo_binario);
    campos_salvos += fwrite(&cabecalho_binario->numero_estacoes, sizeof(int), 1, arquivo_binario);
    campos_salvos += fwrite(&cabecalho_binario->numero_pares_estacoes, sizeof(int), 1, arquivo_binario);

    return campos_salvos;
}

int ler_cabecalho_binario(FILE* arquivo_binario, Cabecalho* cabecalho_binario) {
    rewind(arquivo_binario);

    size_t campos_lidos = 0;

    campos_lidos += fread(&cabecalho_binario->status, sizeof(char), 1, arquivo_binario);
    campos_lidos += fread(&cabecalho_binario->topo_pilha, sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&cabecalho_binario->proximo_rrn, sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&cabecalho_binario->numero_estacoes, sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&cabecalho_binario->numero_pares_estacoes, sizeof(int), 1, arquivo_binario);

    return campos_lidos;
}