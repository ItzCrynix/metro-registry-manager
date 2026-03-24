#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

// verifica se é uma string válida antes de converter
int integer_or_null(char* str) {
    return str == NULL || strcspn(str, "\n\r") == 0 ? -1 : atoi(str);
}

void binarioNaTela(FILE *arquivo_binario) {
    if (arquivo_binario == NULL) return;

    // Retrocede ao início caso o ponteiro já tenha se movido
    rewind(arquivo_binario);

    int caracter;
    while ((caracter = fgetc(arquivo_binario)) != EOF) {
        putchar(caracter);
    }
}