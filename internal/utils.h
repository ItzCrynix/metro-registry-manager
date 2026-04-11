#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define NO_ERROR 0
#define FILE_NOT_FOUND_ERROR -1
#define MALLOC_ERROR -2
#define NO_DATA_FOUND_ERROR -3
#define INVALID_RRN_ERROR -4


/**
 * @brief Converte uma string em número
 * @param str A string que será convertida
 * @return O número convertido ou -1
 */
int integer_or_null(char* str);

const char* string_or_null(char* str);

char* meu_strtok(char** buffer, const char* delimitador);

void ScanQuoteString(char *str);

void BinarioNaTela(char *arquivo);

#endif