#include<stdio.h>
#include<string.h>
#include"registro.h"

typedef enum OPCOES {
    INSERIR_CSV,
    LER_BINARIO,
    BUSCAR_REGISTROS,
    BUSCAR_REGISTROS_RRN,
    SAIR_PROGRAMA
};

int main() {
    while (1) {
        int opcao;
        scanf("%d", &opcao);

        switch (opcao) {
            case INSERIR_CSV:
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