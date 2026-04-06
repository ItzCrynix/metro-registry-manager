#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"
#include "utils.h"

typedef enum opcoes {
    SAIR_PROGRAMA,
    CONVERTER_CSV_BIN,
    LER_BINARIO,
    BUSCAR_REGISTROS,
    BUSCAR_REGISTRO_RRN
} Opcoes;

void buscar_registro_rrn();
void buscar_registro_filtro();
void csv_para_binario();
void ler_arquivo_binario();

int main() {
    int opcao;
    scanf("%d", &opcao);

    switch (opcao) {
        case CONVERTER_CSV_BIN:
            csv_para_binario();
            break;

        case LER_BINARIO:
            ler_arquivo_binario();
            break;

        case BUSCAR_REGISTROS:
            buscar_registro_filtro();
            break;
                
        case BUSCAR_REGISTRO_RRN:
            buscar_registro_rrn();
            break;

        case SAIR_PROGRAMA:
            exit(EXIT_SUCCESS);
            break;

        default:
            printf("Operacao nao suportada!\n");
            break;
    }

    return 0;
}

//
// Funções puramente para deixar a estrutura da main mais limpa
//

void csv_para_binario() {
    char nome_arquivo_csv[100], nome_arquivo_binario[100];
    scanf("%s %s", nome_arquivo_csv, nome_arquivo_binario);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_ESCRITA_BINARIO);
    FILE* arquivo_csv = fopen(nome_arquivo_csv, "r");

    if (escrever_registros_csv(arquivo_csv, arquivo_binario) == FILE_NOT_FOUND_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (arquivo_csv != NULL)
        fclose(arquivo_csv);

    if (arquivo_binario != NULL)
        fclose(arquivo_binario);

    BinarioNaTela(nome_arquivo_binario);
}

void buscar_registro_filtro() {
    char nome_arquivo_binario[100];
    int quantidade_buscas;

    // 1. Lê os dados iniciais
    scanf("%s %d", nome_arquivo_binario, &quantidade_buscas);
    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_LEITURA_BINARIO);
    Cabecalho* cabecalho = ler_cabecalho_binario(arquivo_binario);
    if (cabecalho == NULL) {
        fclose(arquivo_binario);
        printf("Falha no processamento do arquivo.\n");
    }

    int erro = buscar_registro_Filtro(nome_arquivo_binario, quantidade_buscas, arquivo_binario,cabecalho);
    
    
    if (erro == FILE_NOT_FOUND_ERROR || erro == MALLOC_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivo_binario);
    }
}




void buscar_registro_rrn(){
    char nome_arquivo_binario[100];
    int rrn = 0;
    scanf("%s %d", nome_arquivo_binario, &rrn);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_LEITURA_BINARIO);
    
    Registro* registro = NULL; 
    int erro = procurar_registro_RRN(arquivo_binario, &registro, rrn);

    if (erro == FILE_NOT_FOUND_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }
    else if (erro == NO_DATA_FOUND_ERROR || erro == INVALID_RRN_ERROR) {
        printf("Registro inexistente.\n");
    } else {
        char* registro_formatado = to_string(registro);
        printf("%s\n", registro_formatado);
         free(registro_formatado);
        free_registro(&registro);
    }

    if (arquivo_binario != NULL)
        fclose(arquivo_binario);
}

void ler_arquivo_binario() {
    char nome_arquivo_binario[100];
    scanf("%s", nome_arquivo_binario);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_LEITURA_BINARIO);

    int erro = printar_arquivo_binario(arquivo_binario);

    if (erro == NO_DATA_FOUND_ERROR) {
        printf("Registro inexistente.\n");
    } else if (erro == FILE_NOT_FOUND_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }

    if (arquivo_binario != NULL)
        fclose(arquivo_binario);
}   
