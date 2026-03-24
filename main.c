#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"

typedef enum opcoes {
    SAIR_PROGRAMA,
    INSERIR_CSV,
    LER_BINARIO,
    BUSCAR_REGISTROS,
    BUSCAR_REGISTROS_RRN
} Opcoes;

int csv_para_binario(char* nome_arquivo_csv, char* nome_arquivo_binario);

int main() {
    while (1) {
        int opcao;
        scanf("%d", &opcao);

        switch (opcao) {
            case INSERIR_CSV:
                char nome_arquivo_csv[100], nome_arquivo_binario[100];
                scanf("%s %s", nome_arquivo_csv, nome_arquivo_binario);

                if (csv_para_binario(nome_arquivo_csv, nome_arquivo_binario) == -1) {
                    printf("Falha no processamento do arquivo.\n");
                    break;
                }

                // binarioNaTela()
                
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

int csv_para_binario(char* nome_arquivo_csv, char* nome_arquivo_binario) {
    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_ESCRITA_BINARIO);
    if (arquivo_binario == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    FILE* arquivo_csv = fopen(nome_arquivo_csv, "r");
    if (arquivo_csv == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    int resultado = escrever_registros_csv(arquivo_csv, arquivo_binario);

    fclose(arquivo_csv);
    fclose(arquivo_binario);
    return resultado;
}