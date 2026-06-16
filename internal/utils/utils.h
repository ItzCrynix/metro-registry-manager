#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#define NO_ERROR 0
#define FILE_NOT_FOUND_ERROR -1
#define MALLOC_ERROR -2
#define NO_DATA_FOUND_ERROR -3
#define INVALID_RRN_ERROR -4
#define INCOSISTENT_FILE_ERROR -5
#define FILE_READ_ERROR -6
#define FILE_WRITE_ERROR -7

#define MODO_SOMENTE_LEITURA_BINARIO "rb"
#define MODO_SOMENTE_ESCRITA_BINARIO "wb"
#define MODO_LEITURA_EDICAO_BINARIO "rb+"
#define MODO_ESCRITA_EDICAO_BINARIO "wb+"

#define TAM_CABECALHO_REGISTRO 17
#define NUM_CAMPOS_CABECALHO 5
#define STATUS_CONSISTENT '1'
#define STATUS_INCONSISTENT '0'

#define TAM_REGISTRO_DADOS 80
#define STATUS_NOT_REMOVED '0'
#define STATUS_REMOVED '1'

/**
 * @brief Converte uma string em número
 * @param str A string que será convertida
 * @return O número convertido ou -1
 */
int integer_or_null(char* str);

const char* string_or_null(char* str);

void int_to_string(char* str, int number);

char* meu_strtok(char** buffer, const char* delimitador);

void ScanQuoteString(char *str);

void BinarioNaTela(char *arquivo);

void limpa_aspas(char *str);

#endif