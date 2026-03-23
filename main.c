#include<stdio.h>
#include<string.h>
#include"registro.h"

typedef enum OPCOES {
    SAIR_PROGRAMA,
    INSERIR_CSV,
    LER_BINARIO,
    BUSCAR_REGISTROS,
    BUSCAR_REGISTROS_RRN
};

int main() {
    while (1) {
        int opcao;
        scanf("%d", &opcao);

        switch (opcao) {
            case INSERIR_CSV:
                char nome_arquivo_csv[100], nome_arquivo_binario[100];
                scanf("%s %s", nome_arquivo_csv, nome_arquivo_binario);

                if (escrever_registro_csv(nome_arquivo_csv, nome_arquivo_binario) == -1) {
                    printf("Falha no processamento do arquivo.\n");
                    break;
                }

                // caso passe no teste, é pra rodar o binarioNaTela (ainda não foi disponibilizado no edisciplinas)

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