#include "utils.h"

// verifica se é uma string válida antes de converter
int integer_or_null(char* str) {
    return str == NULL || strcspn(str, "\n\r") == 0 ? -1 : atoi(str);
}

const char* string_or_null(char* str) {
    return str == NULL || strcspn(str, "\n\r") == 0 ? "NULO" : str;
}

// o strtok normal pula os espaços com vários ',' seguidos...isto contorna este problema
char* meu_strtok(char** buffer, const char* delimitador) {
    if (buffer == NULL || *buffer == NULL)
        return NULL;

    char *start = *buffer;
    char* posicao_delimitador;

    if ((posicao_delimitador = strpbrk(start, delimitador)) != NULL) {
        *posicao_delimitador = '\0';
        *buffer = posicao_delimitador + 1;
    }
    else
        *buffer = NULL;

    return start;
}

//
// Funcções disponibilizadas no runcodes
//

void ScanQuoteString(char *str) {
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n') { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, "-1"); // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
            strcpy(str, "-1");
        }
        getchar();         // ignorar aspas fechando
    } else if (R != EOF) { // vc tá tentando ler uma string que não tá entre
                           // aspas! Fazer leitura normal %s então, pois deve
                           // ser algum inteiro ou algo assim...
        int i = 0;
        str[i++] = R;
        while ((R = getchar()) != EOF && !isspace(R))
            str[i++] = R;
    } else { // EOF
        strcpy(str, "-1");
    }
}

void BinarioNaTela(char *arquivo) {
    FILE *fs;
    if (arquivo == NULL || !(fs = fopen(arquivo, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }

    fseek(fs, 0, SEEK_END);
    size_t fl = ftell(fs);

    fseek(fs, 0, SEEK_SET);
    unsigned char *mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    unsigned long cs = 0;
    for (unsigned long i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }

    printf("%lf\n", (cs / (double)100));

    free(mb);
    fclose(fs);
}