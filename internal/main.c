#include "./funcionalidades/binario.h"
#include "./funcionalidades/busca.h"
#include "./funcionalidades/remocao.h"
#include "./funcionalidades/update.h"
#include "./funcionalidades/insercao.h"

typedef enum opcoes {
    _,
    CONVERTER_CSV_BIN,
    LER_BINARIO,
    BUSCAR_REGISTROS,
    BUSCAR_REGISTRO_RRN,
    CRIAR_ARQUIVO_INDICE,
    BUSCA_INDEXADA,
    REMOVER_REGISTRO,
    INSERIR_REGISTRO,
    ATUALIZAR_REGISTRO
} Opcoes;

void buscar_registro_rrn();
void busca_filtrada();
void csv_para_binario();
void ler_arquivo_binario();
void criar_arquivo_indice();
void busca_indexada();
void remover_registro_indice();
void atualizar_registros();
void inserir_registro();

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
        case INSERIR_REGISTRO:
            inserir_registro();
            break;
        case ATUALIZAR_REGISTRO:
            atualizar_registros();
            break;
        default:
            printf("Operacao nao suportada!\n");
            break;
    }

    return 0;
}

void csv_para_binario() {
    char nome_arquivo_csv[100], nome_arquivo_binario[100];
    scanf("%s %s", nome_arquivo_csv, nome_arquivo_binario);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_ESCRITA_BINARIO);
    FILE* arquivo_csv = fopen(nome_arquivo_csv, MODO_SOMENTE_LEITURA);

    int erro = escrever_csv_para_binario(arquivo_csv, arquivo_binario);

    if (arquivo_csv != NULL) fclose(arquivo_csv);
    if (arquivo_binario != NULL) fclose(arquivo_binario);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_arquivo_binario);
}

void ler_arquivo_binario() {
    char nome_arquivo_binario[100];
    scanf("%s", nome_arquivo_binario);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);
    int erro = printar_arquivo_binario(arquivo_binario);

    if (arquivo_binario != NULL) fclose(arquivo_binario);

    if (erro == NO_DATA_FOUND_ERROR)
        printf("Registro inexistente.\n");
    else if (erro != NO_ERROR)
        printf("Falha no processamento do arquivo.\n");
}

void busca_filtrada() {
    char nome_arquivo_binario[100];
    int qtd_buscas = 0;
    scanf("%s %d", nome_arquivo_binario, &qtd_buscas);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);
    int erro = buscar_registro_filtro(arquivo_binario, qtd_buscas);

    if (arquivo_binario != NULL) fclose(arquivo_binario);

    if (erro != NO_ERROR)
        printf("Falha no processamento do arquivo.\n");
}

void buscar_registro_rrn() {
    char nome_arquivo_binario[100];
    int rrn = 0;
    scanf("%s %d", nome_arquivo_binario, &rrn);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);

    Registro* reg = NULL;
    int erro = procurar_registro_RRN(arquivo_binario, &reg, rrn);

    if (arquivo_binario != NULL) fclose(arquivo_binario);

    if (erro == FILE_NOT_FOUND_ERROR)
        printf("Falha no processamento do arquivo.\n");
    else if (erro == NO_DATA_FOUND_ERROR || erro == INVALID_RRN_ERROR)
        printf("Registro inexistente.\n");
    else
        print_registro(reg);

    free_registro(&reg);
}

void criar_arquivo_indice() {
    char nome_arquivo_binario[100], nome_arquivo_indice[100];
    scanf("%s %s", nome_arquivo_binario, nome_arquivo_indice);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);
    FILE* arquivo_indice = fopen(nome_arquivo_indice, MODO_SOMENTE_ESCRITA_BINARIO);

    int erro = gerar_arquivo_indice(arquivo_binario, arquivo_indice);

    if (arquivo_binario != NULL) fclose(arquivo_binario);
    if (arquivo_indice != NULL) fclose(arquivo_indice);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_arquivo_indice);
}

void busca_indexada() {
    char nome_arquivo_binario[100], nome_arquivo_indice[100];
    int qtd_buscas;
    scanf("%s %s %d", nome_arquivo_binario, nome_arquivo_indice, &qtd_buscas);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);
    FILE* arquivo_indice = fopen(nome_arquivo_indice, MODO_SOMENTE_LEITURA_BINARIO);

    int erro = buscar_registro_filtro_com_indice(arquivo_binario, arquivo_indice, qtd_buscas);

    if (arquivo_binario != NULL) fclose(arquivo_binario);
    if (arquivo_indice != NULL) fclose(arquivo_indice);

    if (erro != NO_ERROR)
        printf("Falha no processamento do arquivo.\n");
}

void remover_registro_indice() {
    char nome_arquivo_binario[100], nome_arquivo_indice[100];
    int qtd_remocoes;
    scanf("%s %s %d", nome_arquivo_binario, nome_arquivo_indice, &qtd_remocoes);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_LEITURA_EDICAO_BINARIO);
    FILE* arquivo_indice = fopen(nome_arquivo_indice, MODO_LEITURA_EDICAO_BINARIO);

    int erro = remover_registros_com_indice(arquivo_binario, arquivo_indice, qtd_remocoes);

    if (arquivo_binario != NULL) fclose(arquivo_binario);
    if (arquivo_indice != NULL) fclose(arquivo_indice);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_arquivo_binario);
    BinarioNaTela(nome_arquivo_indice);
}

void inserir_registro() {
    char nome_arquivo_binario[100], nome_arquivo_indice[100];
    int qtd_insercao;
    scanf("%s %s %d", nome_arquivo_binario, nome_arquivo_indice, &qtd_insercao);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_LEITURA_EDICAO_BINARIO);
    FILE* arquivo_indice = fopen(nome_arquivo_indice, MODO_LEITURA_EDICAO_BINARIO);

    int erro = inserir_novos_registros(arquivo_binario, arquivo_indice, qtd_insercao);
    
    if (arquivo_binario != NULL) fclose(arquivo_binario);
    if (arquivo_indice != NULL) fclose(arquivo_indice);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_arquivo_binario);
    BinarioNaTela(nome_arquivo_indice);
}

void atualizar_registros() {
    char nome_bin[100], nome_idx[100];
    int n;
    scanf("%s %s %d", nome_bin, nome_idx, &n);

    FILE* bin = fopen(nome_bin, MODO_LEITURA_EDICAO_BINARIO);
    FILE* idx = fopen(nome_idx, MODO_LEITURA_EDICAO_BINARIO);

    int erro = atualizar_registro(bin, idx, n);

    if (bin != NULL) fclose(bin);
    if (idx != NULL) fclose(idx);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_bin);
    BinarioNaTela(nome_idx);
}