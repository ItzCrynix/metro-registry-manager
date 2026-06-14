#include "./funcionalidades/binario.h"
#include "./funcionalidades/busca.h"
#include "./funcionalidades/remocao.h"

typedef enum opcoes {
    _,
    CONVERTER_CSV_BIN,
    LER_BINARIO,
    BUSCAR_REGISTROS,
    BUSCAR_REGISTRO_RRN,
    CRIAR_ARQUIVO_INDICE,
    BUSCA_INDEXADA,
    REMOVER_REGISTRO
} Opcoes;

void buscar_registro_rrn();
void busca_filtrada();
void csv_para_binario();
void ler_arquivo_binario();
void criar_arquivo_indice();
void busca_indexada();
void remover_registro_indice();

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
        case BUSCA_INDEXADA:
            busca_indexada();
            break;
        case REMOVER_REGISTRO:
            remover_registro_indice();
            break;
        default:
            printf("Operacao nao suportada!\n");
            break;
    }

    return 0;
}

void csv_para_binario() {
    char nome_csv[100], nome_bin[100];
    scanf("%s %s", nome_csv, nome_bin);

    FILE* bin = fopen(nome_bin, MODO_SOMENTE_ESCRITA_BINARIO);
    FILE* csv = fopen(nome_csv, "r");

    int erro = escrever_csv_para_binario(csv, bin);

    if (csv != NULL) fclose(csv);
    if (bin != NULL) fclose(bin);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_bin);
}

void ler_arquivo_binario() {
    char nome_bin[100];
    scanf("%s", nome_bin);

    FILE* bin = fopen(nome_bin, MODO_SOMENTE_LEITURA_BINARIO);
    int erro = printar_arquivo_binario(bin);

    if (bin != NULL) fclose(bin);

    if (erro == NO_DATA_FOUND_ERROR)
        printf("Registro inexistente.\n");
    else if (erro != NO_ERROR)
        printf("Falha no processamento do arquivo.\n");
}

void busca_filtrada() {
    char nome_bin[100];
    int n;
    scanf("%s %d", nome_bin, &n);

    FILE* bin = fopen(nome_bin, MODO_SOMENTE_LEITURA_BINARIO);
    int erro = buscar_registro_filtro(bin, n);

    if (bin != NULL) fclose(bin);

    if (erro != NO_ERROR)
        printf("Falha no processamento do arquivo.\n");
}

void buscar_registro_rrn() {
    char nome_bin[100];
    int rrn = 0;
    scanf("%s %d", nome_bin, &rrn);

    FILE* bin = fopen(nome_bin, MODO_SOMENTE_LEITURA_BINARIO);

    Registro* reg = NULL;
    int erro = procurar_registro_RRN(bin, &reg, rrn);

    free_registro(&reg);
    if (bin != NULL) fclose(bin);

    if (erro == FILE_NOT_FOUND_ERROR)
        printf("Falha no processamento do arquivo.\n");
    else if (erro == NO_DATA_FOUND_ERROR || erro == INVALID_RRN_ERROR)
        printf("Registro inexistente.\n");
    else
        print_registro(reg);
}

void criar_arquivo_indice() {
    char nome_bin[100], nome_idx[100];
    scanf("%s %s", nome_bin, nome_idx);

    FILE* bin = fopen(nome_bin, MODO_SOMENTE_LEITURA_BINARIO);
    FILE* idx = fopen(nome_idx, MODO_SOMENTE_ESCRITA_BINARIO);

    int erro = gerar_arquivo_indice(bin, idx);

    if (bin != NULL) fclose(bin);
    if (idx != NULL) fclose(idx);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_idx);
}

void busca_indexada() {
    char nome_bin[100], nome_idx[100];
    int n;
    scanf("%s %s %d", nome_bin, nome_idx, &n);

    FILE* bin = fopen(nome_bin, MODO_SOMENTE_LEITURA_BINARIO);
    FILE* idx = fopen(nome_idx, MODO_SOMENTE_LEITURA_BINARIO);

    int erro = buscar_registro_filtro_com_indice(bin, idx, n);

    if (bin != NULL) fclose(bin);
    if (idx != NULL) fclose(idx);

    if (erro != NO_ERROR)
        printf("Falha no processamento do arquivo.\n");
}

void remover_registro_indice() {
    char nome_bin[100], nome_idx[100];
    int qtd_remocoes;
    scanf("%s %s %d", nome_bin, nome_idx, &qtd_remocoes);

    FILE* bin = fopen(nome_bin, MODO_LEITURA_EDICAO_BINARIO);
    FILE* idx = fopen(nome_idx, MODO_LEITURA_EDICAO_BINARIO);

    int erro = remover_registros_com_indice(bin, idx, qtd_remocoes);

    if (bin != NULL) fclose(bin);
    if (idx != NULL) fclose(idx);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_bin);
    BinarioNaTela(nome_idx);
}
