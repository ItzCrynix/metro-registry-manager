#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/**
 * @brief Converte uma string em número
 * @param str A string que será convertida
 * @return O número convertido ou -1
 */
int integer_or_null(char* str);

const char* string_or_null(char* str);

void ScanQuoteString(char *str);

void BinarioNaTela(char *arquivo);

#endif