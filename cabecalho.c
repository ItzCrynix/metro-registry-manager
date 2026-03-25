#include <stdlib.h>
#include <stdio.h>
#include "cabecalho.h"

Cabecalho* novo_cabecalho() {
    Cabecalho* novo = (Cabecalho*) malloc(sizeof(Cabecalho));
    if (novo == NULL) {
        return NULL;
    }

    // escreve os valores padrões que o cabeçalho deve assumir em um arquivo vazio
    novo->status = STATUS_CONSISTENT;
    novo->topo_pilha = -1;
    novo->proximo_rrn = 0;
    novo->numero_estacoes = 0;
    novo->numero_pares_estacoes = 0;

    return novo;
}

void salvar_cabecalho(FILE* arquivo_binario, Cabecalho* cabecalho_binario) {
    // volta o ponteiro para o inicio do arquivo
    rewind(arquivo_binario);

    // Escreve os registros de cabeçalho
    fwrite(&cabecalho_binario->status, sizeof(char), 1, arquivo_binario);
    fwrite(&cabecalho_binario->topo_pilha, sizeof(int), 1, arquivo_binario);
    fwrite(&cabecalho_binario->proximo_rrn, sizeof(int), 1, arquivo_binario);
    fwrite(&cabecalho_binario->numero_estacoes, sizeof(int), 1, arquivo_binario);
    fwrite(&cabecalho_binario->numero_pares_estacoes, sizeof(int), 1, arquivo_binario);
}

Cabecalho* ler_cabecalho_binario(FILE* arquivo_binario) {
    Cabecalho* cabecalho_binario = novo_cabecalho();
    if (cabecalho_binario == NULL) {
        return NULL;
    }

    fread(&cabecalho_binario->status, sizeof(char), 1, arquivo_binario);
    fread(&cabecalho_binario->topo_pilha, sizeof(int), 1, arquivo_binario);
    fread(&cabecalho_binario->proximo_rrn, sizeof(int), 1, arquivo_binario);
    fread(&cabecalho_binario->numero_estacoes, sizeof(int), 1, arquivo_binario);
    fread(&cabecalho_binario->numero_pares_estacoes, sizeof(int), 1, arquivo_binario);

    return cabecalho_binario;
}