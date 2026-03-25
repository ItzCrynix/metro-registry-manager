#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"
#include "utils.h"

typedef enum opcoes {
    SAIR_PROGRAMA,
    CONVERTER_CSV_BIN,
    LER_BINARIO,
    BUSCAR_REGISTROS,
    BUSCAR_REGISTROS_RRN
} Opcoes;

void csv_para_binario(char* nome_arquivo_csv, char* nome_arquivo_binario);

int main() {
    while (1) {
        int opcao;
        scanf("%d", &opcao);

        switch (opcao) {
            case CONVERTER_CSV_BIN:
                char nome_arquivo_csv[100], nome_arquivo_binario[100];
                scanf("%s %s", nome_arquivo_csv, nome_arquivo_binario);

                csv_para_binario(nome_arquivo_csv, nome_arquivo_binario);
                break;
            case LER_BINARIO:
                break;
            case BUSCAR_REGISTROS:
                break;
            case BUSCAR_REGISTROS_RRN:
                break;
            case SAIR_PROGRAMA:
                exit(0);
                break;
            default:
                printf("Operacao nao suportada!\n");
                break;
        }
    }
}

void csv_para_binario(char* nome_arquivo_csv, char* nome_arquivo_binario) {
    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_ESCRITA_BINARIO);
    FILE* arquivo_csv = fopen(nome_arquivo_csv, "r");

    if (escrever_registros_csv(arquivo_csv, arquivo_binario) == FILE_NOT_FOUND_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }

    fclose(arquivo_csv);
    fclose(arquivo_binario);
}