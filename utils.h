#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Converte uma string em número
 * @param str A string que será convertida
 * @return O número convertido ou -1
 */
int integer_or_null(char* str);

/**
 * @brief Printa na terminal todos os bytes de um arquivo binário
 * @param arquivo_binario O arquivo que será printado
 */
void binarioNaTela(FILE *arquivo_binario);

#endif