#include "registro.h"

void free_registro(Registro** registro) {
    if (registro == NULL || *registro == NULL) return;

    // As strings são alocadas separadamente, então é necessário desalocar
    free((*registro)->nome_estacao);
    (*registro)->nome_estacao = NULL;

    free((*registro)->nome_linha);
    (*registro)->nome_linha = NULL;

    free(*registro);

    *registro = NULL;
}

/**
 * @brief Transforma uma string em um novo registro
 * @param buffer A cadeia de caracteres que contem o novo registro
 * @return Um registro contendo todas as informações do buffer
 */
Registro* tokenizar_registro(char* buffer) {
    Registro* registro_temporario = (Registro*) malloc(sizeof(Registro));
    if (registro_temporario == NULL) {
        return NULL;
    }

    // strtok basicamente retorna uma string do buffer cada vez subsequente que é chamado com o NULL como primeiro argumento
    char* token = meu_strtok(&buffer, ",");
    registro_temporario->codigo_estacao = integer_or_null(token);

    token = meu_strtok(&buffer, ",");
    registro_temporario->nome_estacao = strdup(token);
    registro_temporario->tamanho_nome_estacao = strlen(token);

    token = meu_strtok(&buffer, ",");
    registro_temporario->codigo_linha = integer_or_null(token);

    token = meu_strtok(&buffer, ",");
    registro_temporario->nome_linha = strdup(token);
    registro_temporario->tamanho_nome_linha = strlen(token);

    token = meu_strtok(&buffer, ",");
    registro_temporario->codigo_proxima_estacao = integer_or_null(token);

    token = meu_strtok(&buffer, ",");
    registro_temporario->distancia_proxima_estacao = integer_or_null(token);

    token = meu_strtok(&buffer, ",");
    registro_temporario->codigo_linha_integracao = integer_or_null(token);

    token = meu_strtok(&buffer, ",");
    registro_temporario->codigo_estacao_integracao = integer_or_null(token);

    return registro_temporario;
}

void print_registro(Registro* registro) {
    // instancia todos os filtro->campos com filtro->valor padrão
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

    printf("%s %s %s %s %s %s %s %s\n",
        cod_estacao_formatado,
        string_or_null(registro->nome_estacao),
        cod_linha_formatado,
        string_or_null(registro->nome_linha),
        cod_prox_estacao_formatado,
        dist_prox_estacao_formatado,
        cod_linha_integra_formatado,
        cod_estacao_integra_formatado
    );
}

int salvar_campos_fixos(FILE* arquivo_binario, Registro* novo_registro) {
    size_t campos_salvos = 0;
    
    // status e informações do próximo registro na pilha
    campos_salvos += fwrite(&novo_registro->removido, sizeof(char), 1, arquivo_binario);
    campos_salvos += fwrite(&novo_registro->proximo_registro, sizeof(int), 1, arquivo_binario);

    // informações das estações
    campos_salvos += fwrite(&novo_registro->codigo_estacao, sizeof(int), 1, arquivo_binario);
    campos_salvos += fwrite(&novo_registro->codigo_linha, sizeof(int), 1, arquivo_binario);
    campos_salvos += fwrite(&novo_registro->codigo_proxima_estacao, sizeof(int), 1, arquivo_binario);
    campos_salvos += fwrite(&novo_registro->distancia_proxima_estacao, sizeof(int), 1, arquivo_binario);
    campos_salvos += fwrite(&novo_registro->codigo_linha_integracao, sizeof(int), 1, arquivo_binario);
    campos_salvos += fwrite(&novo_registro->codigo_estacao_integracao, sizeof(int), 1, arquivo_binario);

    return campos_salvos;
}

int salvar_campos_variaveis(FILE* arquivo_binario, Registro* novo_registro) {
    size_t campos_salvos = 0;

    campos_salvos += fwrite(&novo_registro->tamanho_nome_estacao, sizeof(int), 1, arquivo_binario);
    if (novo_registro->tamanho_nome_estacao > 0) {
        campos_salvos += fwrite(novo_registro->nome_estacao, sizeof(char), novo_registro->tamanho_nome_estacao, arquivo_binario);
    }

    campos_salvos += fwrite(&novo_registro->tamanho_nome_linha, sizeof(int), 1, arquivo_binario);
    if (novo_registro->tamanho_nome_linha > 0) {
        campos_salvos += fwrite(novo_registro->nome_linha, sizeof(char), novo_registro->tamanho_nome_linha, arquivo_binario);
    }

    return campos_salvos;
}

int salvar_registro_binario(FILE* arquivo_binario, Registro* novo_registro) {
    long inicio_registro = ftell(arquivo_binario);

    size_t campos_salvos = 0;
    campos_salvos += salvar_campos_fixos(arquivo_binario, novo_registro);
    campos_salvos += salvar_campos_variaveis(arquivo_binario, novo_registro);

    long final_registro = ftell(arquivo_binario);

    // Seria possível calcular os bytes usando sizeof(...) hardcoded, mas o ftell deixa mais intuitivo
    int bytes_remanescentes = TAM_REGISTRO_DADOS - (final_registro - inicio_registro);

    // escreve o final com $, caso sobre bytes no registro
    if (bytes_remanescentes > 0) {
        char placeholder = '$';
        for (int i = 0; i < bytes_remanescentes; i++)
            fwrite(&placeholder, sizeof(char), 1, arquivo_binario);
    }

    return campos_salvos;
}

Registro* ler_registro_RRN(FILE* arquivo_binario, int rrn) {
    int byte_offset = (TAM_REGISTRO_CABECALHO + (TAM_REGISTRO_DADOS * rrn));
    fseek(arquivo_binario, byte_offset, SEEK_SET);

    char removido = STATUS_REMOVED;
    if(fread(&(removido), sizeof(char), 1, arquivo_binario) < 1 || removido == STATUS_REMOVED) {
        return NULL;
    }
    
    Registro* registro_encontrado = (Registro*) malloc(sizeof(Registro));
    if (registro_encontrado == NULL) {
        return NULL;
    }

    registro_encontrado->removido = removido;

    size_t campos_lidos = 0;
    campos_lidos += fread(&(registro_encontrado->proximo_registro), sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&(registro_encontrado->codigo_estacao), sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&(registro_encontrado->codigo_linha), sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&(registro_encontrado->codigo_proxima_estacao), sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&(registro_encontrado->distancia_proxima_estacao), sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&(registro_encontrado->codigo_linha_integracao), sizeof(int), 1, arquivo_binario);
    campos_lidos += fread(&(registro_encontrado->codigo_estacao_integracao), sizeof(int), 1, arquivo_binario);

    // Leitura dos campos variáveis
    campos_lidos += fread(&(registro_encontrado->tamanho_nome_estacao), sizeof(int), 1, arquivo_binario);
    if (registro_encontrado->tamanho_nome_estacao > 0) {
        registro_encontrado->nome_estacao = (char*) calloc(registro_encontrado->tamanho_nome_estacao, sizeof(char));
    }
    campos_lidos += fread(registro_encontrado->nome_estacao, sizeof(char), registro_encontrado->tamanho_nome_estacao, arquivo_binario);
    
    campos_lidos += fread(&(registro_encontrado->tamanho_nome_linha), sizeof(int), 1, arquivo_binario);
    if (registro_encontrado->tamanho_nome_linha > 0) {
        registro_encontrado->nome_linha = (char*) calloc(registro_encontrado->tamanho_nome_linha + 1, sizeof(char));
    }
    campos_lidos += fread(registro_encontrado->nome_linha, sizeof(char), registro_encontrado->tamanho_nome_linha, arquivo_binario);

    int qtd_max_bytes_lidos = 10 + registro_encontrado->tamanho_nome_estacao + registro_encontrado->tamanho_nome_linha;

    if (campos_lidos < qtd_max_bytes_lidos) {
        NULL;
    }

    return registro_encontrado;
}