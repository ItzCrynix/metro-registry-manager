#include "./binario/binario.h"
#include "./structs/indice.h"
#include "./structs/registro.h"
#include "./busca/busca.h"

typedef enum opcoes {
    _,
    CONVERTER_CSV_BIN,
    LER_BINARIO,
    BUSCAR_REGISTROS,
    BUSCAR_REGISTRO_RRN,
    CRIAR_ARQUIVO_INDICE
} Opcoes;

void buscar_registro_rrn();
void busca_filtrada();
void csv_para_binario();
void ler_arquivo_binario();
void criar_arquivo_indice();

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
            busca_filtrada();
            break;
                
        case BUSCAR_REGISTRO_RRN:
            buscar_registro_rrn();
            break;
        
        case CRIAR_ARQUIVO_INDICE:
            criar_arquivo_indice();
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

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_ESCRITA_BINARIO);
    FILE* arquivo_csv = fopen(nome_arquivo_csv, "r");

    if (escrever_csv_para_binario(arquivo_csv, arquivo_binario) != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (arquivo_csv != NULL)
        fclose(arquivo_csv);

    if (arquivo_binario != NULL)
        fclose(arquivo_binario);

    BinarioNaTela(nome_arquivo_binario);
}

void ler_arquivo_binario() {
    char nome_arquivo_binario[100];
    scanf("%s", nome_arquivo_binario);
    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);

    int erro = printar_arquivo_binario(arquivo_binario);

    if (erro == NO_DATA_FOUND_ERROR) {
        printf("Registro inexistente.\n");
    } 
    else if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }

    if (arquivo_binario != NULL)
        fclose(arquivo_binario);
}

void busca_filtrada() {
    char nome_arquivo_binario[100];
    int quantidade_buscas;

    // 1. Lê os dados iniciais
    scanf("%s %d", nome_arquivo_binario, &quantidade_buscas);
    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);

    int erro = buscar_registro_filtro(arquivo_binario, quantidade_buscas);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }
    
    if (arquivo_binario != NULL)
        fclose(arquivo_binario);
}

void buscar_registro_rrn() {
    char nome_arquivo_binario[100];
    int rrn = 0;
    scanf("%s %d", nome_arquivo_binario, &rrn);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);
    
    Registro* registro = NULL; 
    int erro = procurar_registro_RRN(arquivo_binario, &registro, rrn);

    if (erro == FILE_NOT_FOUND_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }
    else if (erro == NO_DATA_FOUND_ERROR || erro == INVALID_RRN_ERROR) {
        printf("Registro inexistente.\n");
    } 
    else {
        print_registro(registro);
    }

    free_registro(&registro);

    if (arquivo_binario != NULL)
        fclose(arquivo_binario);
}

void criar_arquivo_indice() {
    char nome_arquivo_binario[100], nome_arquivo_indice[100];
    scanf("%s %s", nome_arquivo_binario, nome_arquivo_indice);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);
    FILE* arquivo_indice = fopen(nome_arquivo_indice, MODO_SOMENTE_ESCRITA_BINARIO);

    int erro = gerar_arquivo_indice(arquivo_binario, arquivo_indice);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }

    if (arquivo_binario != NULL)
        fclose(arquivo_binario);

    if (arquivo_indice != NULL)
        fclose(arquivo_indice);

    BinarioNaTela(nome_arquivo_indice);
}
