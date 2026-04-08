#ifndef REGISTRO_H
#define REGISTRO_H

#include "utils.h"
#include "cabecalho.h"

#define TAM_REGISTRO_DADOS 80

#define STATUS_NOT_REMOVED '0'
#define STATUS_REMOVED '1'

typedef struct registro {
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
} Registro;

typedef struct filtro {
    char* campo;
    char* valor;
} Filtro;

/**
 * @brief Libera a memória de um registro
 * @param registro O endereço que será liberado
 */
void free_registro(Registro** registro);

void free_filtro(Filtro** filtro, int tam);

/**
 * @brief Transforma uma string em um novo registro
 * @param buffer A cadeia de caracteres que contem o novo registro
 * @return Um registro contendo todas as informações do buffer
 */
Registro* tokenizar_registro(char* buffer);

void print_registro(Registro* registro);

/**
 * @brief Salva as informações de um registro em um arquivo binário
 * @param arquivo_binario
 * @param novo_registro O registro que será salvo
 */
void salvar_registro_binario(FILE* arquivo_binario, Registro* novo_registro);

/**
 * @brief Lê um registro guardado no arquivo binário baseado no RRN (Relative Record Number)
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param RRN Relative Record Number, indica qual "linha" dos registros vai ser lida
 * @return Um registro, caso encontre, ou NULL
 */
Registro* ler_registro_RRN(FILE* nome_arquivo, int RRN);

int passou_no_filtro(Registro* registro, Filtro* filtro);

#endif