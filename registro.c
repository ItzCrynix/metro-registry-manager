#include "registro.h"

void free_registro(Registro** registro) {
    if (*registro == NULL) return;

    // As strings são alocadas separadamente, então é necessário desalocar
    if ((*registro)->nome_estacao != NULL) {
        free((*registro)->nome_estacao);
        (*registro)->nome_estacao = NULL;
    }

    if ((*registro)->nome_linha != NULL) {
        free((*registro)->nome_linha);
        (*registro)->nome_linha = NULL;
    }

    free(*registro);

    *registro = NULL;
}

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

void int_to_string(char* str, int number) {
    if (number != -1) 
        sprintf(str, "%d", number);
}

char* to_string(Registro* registro) {
    char* formatado = (char*) calloc(150, sizeof(char));
    if (formatado == NULL) {
        return NULL;
    }

    // instancia todos os campos com valor padrão
    char cod_estacao_formatado[20] = "NULO";
    char cod_linha_formatado[20] = "NULO";
    char cod_prox_estacao_formatado[20] = "NULO";
    char dist_prox_estacao_formatado[20] = "NULO";
    char cod_linha_integra_formatado[20] = "NULO";
    char cod_estacao_integra_formatado[20] = "NULO";

    int_to_string(cod_estacao_formatado, registro->codigo_estacao);
    int_to_string(cod_linha_formatado, registro->codigo_linha);
    int_to_string(cod_prox_estacao_formatado, registro->codigo_proxima_estacao);
    int_to_string(dist_prox_estacao_formatado, registro->distancia_proxima_estacao);
    int_to_string(cod_linha_integra_formatado, registro->codigo_linha_integracao);
    int_to_string(cod_estacao_integra_formatado, registro->codigo_estacao_integracao);

    sprintf(formatado, "%s %s %s %s %s %s %s %s",
        cod_estacao_formatado,
        string_or_null(registro->nome_estacao),
        cod_linha_formatado,
        string_or_null(registro->nome_linha),
        cod_prox_estacao_formatado,
        dist_prox_estacao_formatado,
        cod_linha_integra_formatado,
        cod_estacao_integra_formatado
    );

    return formatado;
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

    // Salva os dois campos de tamanho variável
    fwrite(&novo_registro->tamanho_nome_estacao, sizeof(int), 1, arquivo_binario);
    if (novo_registro->tamanho_nome_estacao > 0) {
        fwrite(novo_registro->nome_estacao, sizeof(char), novo_registro->tamanho_nome_estacao, arquivo_binario);
    }

    fwrite(&novo_registro->tamanho_nome_linha, sizeof(int), 1, arquivo_binario);
    if (novo_registro->tamanho_nome_linha > 0) {
        fwrite(novo_registro->nome_linha, sizeof(char), novo_registro->tamanho_nome_linha, arquivo_binario);
    }

    long final_registro = ftell(arquivo_binario);

    // Seria possível calcular os bytes usando sizeof(...) hardcoded, mas o ftell deixa mais intuitivo
    int bytes_remanescentes = TAM_REGISTRO_DADOS - (final_registro - inicio_registro);

    // escreve o final com $, caso sobre bytes no registro
    if (bytes_remanescentes > 0) {
        char placeholder = '$';
        for (int i = 0; i < bytes_remanescentes; i++)
            fwrite(&placeholder, sizeof(char), 1, arquivo_binario);
    }
}