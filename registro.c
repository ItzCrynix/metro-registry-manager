#include "registro.h"

void free_registro(Registro* registro) {
    if (registro == NULL) return;

    free(registro->nome_estacao);
    free(registro->nome_linha);
    free(registro);

    registro = NULL;
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

char* int_to_string(int number) {
    char result[20] = "NULO";

    if (number != -1) {
        sprintf(result, "%d", number);
    }

    return result;
}

char* to_string(Registro* registro) {
    char* formatado = (char*) calloc(100, sizeof(char));

    char* cod_estacao_formatado = int_to_string(registro->codigo_estacao);
    char* cod_linha_formatado = int_to_string(registro->codigo_linha);
    char* cod_prox_estacao_formatado = int_to_string(registro->codigo_proxima_estacao);
    char* dist_prox_estacao_formatado = int_to_string(registro->distancia_proxima_estacao);
    char* cod_linha_integra_formatado = int_to_string(registro->codigo_linha_integracao);
    char* cod_estacao_integra_formatado = int_to_string(registro->codigo_estacao_integracao);

    string_or_null(registro->nome_estacao);
    string_or_null(registro->nome_linha);

    sprintf(formatado, "%s %s %s %s %s %s %s %s",
        cod_estacao_formatado,
        registro->nome_estacao,
        cod_linha_formatado,
        registro->nome_linha,
        cod_prox_estacao_formatado,
        dist_prox_estacao_formatado,
        cod_linha_integra_formatado,
        cod_estacao_integra_formatado
    );

    free(cod_estacao_formatado);
    free(cod_linha_formatado);
    free(cod_prox_estacao_formatado);
    free(dist_prox_estacao_formatado);
    free(cod_linha_integra_formatado);
    free(cod_estacao_integra_formatado);

    return formatado;
}