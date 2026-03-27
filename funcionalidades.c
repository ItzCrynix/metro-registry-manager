#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"
#include "registro.h"
#include "utils.h"
#include "cabecalho.h"

//
// Funções auxiliares do escrever_registro_csv()
//

void salvar_campos_fixos(FILE* arquivo_binario, Registro* novo_registro) {
    // status e informações do próximo registro na pilha
    fwrite(&novo_registro->removido, sizeof(char), 1, arquivo_binario);
    fwrite(&novo_registro->proximo_registro, sizeof(int), 1, arquivo_binario);

    // informações das estações
    fwrite(&novo_registro->codigo_estacao, sizeof(int), 1, arquivo_binario);
    fwrite(&novo_registro->codigo_linha, sizeof(int), 1, arquivo_binario);
    fwrite(&novo_registro->codigo_proxima_estacao, sizeof(int), 1, arquivo_binario);
    fwrite(&novo_registro->distancia_proxima_estacao, sizeof(int), 1, arquivo_binario);
    fwrite(&novo_registro->codigo_linha_integracao, sizeof(int), 1, arquivo_binario);
    fwrite(&novo_registro->codigo_estacao_integracao, sizeof(int), 1, arquivo_binario);
}

void salvar_registro_binario(FILE* arquivo_binario, Registro* novo_registro) {
    long inicio_registro = ftell(arquivo_binario);

    salvar_campos_fixos(arquivo_binario, novo_registro);

    fwrite(&novo_registro->tamanho_nome_estacao, sizeof(int), 1, arquivo_binario);
    if (novo_registro->tamanho_nome_estacao > 0) {
        fwrite(novo_registro->nome_estacao, sizeof(char), novo_registro->tamanho_nome_estacao, arquivo_binario);
    }

    fwrite(&novo_registro->tamanho_nome_linha, sizeof(int), 1, arquivo_binario);
    if (novo_registro->tamanho_nome_linha > 0) {
        fwrite(novo_registro->nome_linha, sizeof(char), novo_registro->tamanho_nome_linha, arquivo_binario);
    }

    long final_registro = ftell(arquivo_binario);

    // escreve o final com $, caso sobre espaço
    int bytes_remanescentes = TAM_REGISTRO_DADOS - (final_registro - inicio_registro);
    if (bytes_remanescentes > 0) {
        char placeholder = '$';
        for (int i = 0; i < bytes_remanescentes; i++)
            fwrite(&placeholder, sizeof(char), 1, arquivo_binario);
    }
}


int escrever_registros_csv(FILE* arquivo_csv, FILE* arquivo_binario) {
    if (arquivo_binario == NULL || arquivo_csv == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    // Cria um arquivo para cuidar das mudanças do cabeçalho, para salvar depois
    Cabecalho* cabecalho_binario = novo_cabecalho();
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
    }

    // escreve o cabeçalho incial
    salvar_cabecalho(arquivo_binario, cabecalho_binario);

    char buffer[200];
    // Descarta a primeira linha, que contem o nome das colunas
    fgets(buffer, sizeof(buffer), arquivo_csv);

    // Lê o arquivo csv linha por linha
    while(fgets(buffer, sizeof(buffer), arquivo_csv)) {
        Registro* novo_registro = tokenizar_registro(buffer);
        if (novo_registro == NULL) {
            return MALLOC_ERROR;
        }

        // marca o registro como não removido e salva o ultimo valor da pilha nele
        novo_registro->removido = '0';
        novo_registro->proximo_registro = cabecalho_binario->topo_pilha;

        salvar_registro_binario(arquivo_binario, novo_registro);

        free(novo_registro->nome_estacao);
        free(novo_registro->nome_linha);
        free(novo_registro);
    }

    // Salva as ultimas alterações feitas do cabeçalho e libera a memória
    cabecalho_binario->status = STATUS_CONSISTENT;
    salvar_cabecalho(arquivo_binario, cabecalho_binario);
    free(cabecalho_binario);

    return NO_ERROR;
}

Registro* ler_registro_RRN(FILE* nome_arquivo, int RRN) {
    // Abre o arquivo no modo escrita/leitura binária
    FILE* arquivo = fopen(nome_arquivo, MODO_LEITURA_BINARIO);

    // faz alguma coisa

    fclose(arquivo);
    return 0;
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

    int RRN_atual = 0;
    int registros_printados = 0;

    while (RRN_atual < cabecalho_binario->proximo_rrn) {
        Registro* registro_atual = ler_registro_RRN(arquivo_binario, RRN_atual++);

        // Caso tenha registro excluido
        if (registro_atual == NULL) {
            continue;
        }

        char* registro_formatado = to_string(registro_atual);
        printf("%s\n", registro_formatado);
        registros_printados++;
    }

    free(cabecalho_binario);

    return NO_ERROR;
}