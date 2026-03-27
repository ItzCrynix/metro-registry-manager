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

void int_to_string(char* str, int number) {
    if (number != -1) 
        str = "NULO";
    else  
        sprintf(str, "%d", number);
}

char* to_string(Registro* registro) {
    char* formatado = (char*) calloc(150, sizeof(char));
    if (formatado == NULL) {
        return NULL;
    }

    char cod_estacao_formatado[20];
    char cod_linha_formatado[20];
    char cod_prox_estacao_formatado[20];
    char dist_prox_estacao_formatado[20];
    char cod_linha_integra_formatado[20];
    char cod_estacao_integra_formatado[20];

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