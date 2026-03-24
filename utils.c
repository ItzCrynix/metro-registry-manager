#include <stdlib.h>
#include <string.h>
#include "utils.h"

// verifica se é uma string válida antes de converter
int integer_or_null(char* str) {
    return str == NULL || strcspn(str, "\n\r") == 0 ? -1 : atoi(str);
}