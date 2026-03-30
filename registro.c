#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"
#include "utils.h"
#include "cabecalho.h"

//
// Funções auxiliares do escrever_registro_csv()
//

Registro* tokenizar_registro(char* buffer) {
    Registro* registro_temporario = (Registro*) malloc(sizeof(Registro));
    if (registro_temporario == NULL) {
        return NULL;
    }

    // strtok basicamente retorna uma string do buffer cada vez subsequente que é chamado com o NULL como primeiro argumento
    char* token = strtok(buffer, ",");
    registro_temporario->codigo_estacao = integer_or_null(token);

    token = strtok(NULL, ",");
    registro_temporario->nome_estacao = strdup(token);
    registro_temporario->tamanho_nome_estacao = strlen(token);

    token = strtok(NULL, ",");
    registro_temporario->codigo_linha = integer_or_null(token);

    token = strtok(NULL, ",");
    registro_temporario->nome_linha = strdup(token);
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

    int registros_realizados = 0;
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

        registros_realizados++;
        free(novo_registro);
    }

    // Salva as ultimas alterações feitas do cabeçalho
    cabecalho_binario->proximo_rrn = registros_realizados;
    salvar_cabecalho(arquivo_binario, cabecalho_binario);

    free(cabecalho_binario);
    return 0;
}

Registro* ler_registro_RRN(FILE* arquivo_binario, int rrn) {
    // Posiciona o ponteiro do arquivo no dado
    fseek(arquivo_binario, 5, 0);

    int prox_rrn = 0;
    fread(&prox_rrn,sizeof(int), 1, arquivo_binario);

    // O RRN não está dentro do range permitido
    if(rrn < 0 || rrn >= prox_rrn){
        return NULL;
    }

    int byte_offset = (TAM_REGISTRO_CABECALHO + (TAM_REGISTRO_DADOS * rrn));

    fseek(arquivo_binario,byte_offset, 0);

    // 
    char removido;
    fread(&(removido),sizeof(char), 1, arquivo_binario);
    if(removido == RRN_REMOVED){
        return NULL;
    }
    
    Registro* registro_encontrado = (Registro*) malloc(sizeof(Registro));
    if (registro_encontrado == NULL) {
        return NULL;
    }

    registro_encontrado->removido = removido;

    // Leirura dos campos fixos
    fread(&(registro_encontrado->proximo_registro), sizeof(int), 1, arquivo_binario);
    fread(&(registro_encontrado->codigo_estacao), sizeof(int), 1, arquivo_binario);
    fread(&(registro_encontrado->codigo_linha), sizeof(int), 1, arquivo_binario);
    fread(&(registro_encontrado->codigo_proxima_estacao), sizeof(int), 1, arquivo_binario);
    fread(&(registro_encontrado->distancia_proxima_estacao), sizeof(int), 1, arquivo_binario);
    fread(&(registro_encontrado->codigo_linha_integracao), sizeof(int), 1, arquivo_binario);
    fread(&(registro_encontrado->codigo_estacao_integracao), sizeof(int), 1, arquivo_binario);

    // Leitura dos campos variáveis
    fread(&(registro_encontrado->tamanho_nome_estacao), sizeof(int), 1, arquivo_binario);

    registro_encontrado->nome_estacao = (char*) calloc(registro_encontrado->tamanho_nome_estacao, sizeof(char));
    fread(registro_encontrado->nome_estacao, sizeof(char), registro_encontrado->tamanho_nome_estacao, arquivo_binario);
    
    fread(&(registro_encontrado->tamanho_nome_linha), sizeof(int), 1, arquivo_binario);

    registro_encontrado->nome_linha = (char*) calloc(registro_encontrado->tamanho_nome_linha, sizeof(char));
    fread(registro_encontrado->nome_linha, sizeof(char), registro_encontrado->tamanho_nome_linha, arquivo_binario);

    return registro_encontrado;
}

int buscar_registro_RRN(FILE* arquivo_binario, Registro** registro, int rrn) {
    if (arquivo_binario == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    *registro = ler_registro_RRN(arquivo_binario, rrn);
    if (registro == NULL) {
        return RRN_NOT_FOUND;
    }

    return 0;
}