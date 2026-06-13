#include "./binario/binario.h"
#include "./structs/indice.h"
#include "./structs/registro.h"
#include "./busca/busca.h"

/*
 * Cada número corresponde a uma funcionalidade do trabalho.
 * O enum garante que BUSCA_INDEXADA = 6 automaticamente,
 * sem precisar escrever o número na mão.
 */
typedef enum opcoes {
    _,                      /* 0 — não usado */
    CONVERTER_CSV_BIN,      /* 1 */
    LER_BINARIO,            /* 2 */
    BUSCAR_REGISTROS,       /* 3 */
    BUSCAR_REGISTRO_RRN,    /* 4 */
    CRIAR_ARQUIVO_INDICE,   /* 5 */
    BUSCA_INDEXADA          /* 6 — novo */
} Opcoes;

/* Declarações das funções locais (implementadas no final deste arquivo) */
void buscar_registro_rrn();
void busca_filtrada();
void csv_para_binario();
void ler_arquivo_binario();
void criar_arquivo_indice();
void busca_indexada();          /* nova */

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

        default:
            printf("Operacao nao suportada!\n");
            break;
    }

    return 0;
}

/* ============================================================
 * Funções auxiliares da main
 * Cada uma só lê os parâmetros do stdin e chama a função certa.
 * ============================================================ */

void csv_para_binario() {
    char nome_arquivo_csv[100], nome_arquivo_binario[100];
    scanf("%s %s", nome_arquivo_csv, nome_arquivo_binario);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_ESCRITA_BINARIO);
    FILE* arquivo_csv     = fopen(nome_arquivo_csv, "r");

    int erro = escrever_csv_para_binario(arquivo_csv, arquivo_binario);

    if (arquivo_csv     != NULL) fclose(arquivo_csv);
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

    if (erro == NO_DATA_FOUND_ERROR) {
        printf("Registro inexistente.\n");
    } else if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }
}

void busca_filtrada() {
    char nome_arquivo_binario[100];
    int quantidade_buscas;

    scanf("%s %d", nome_arquivo_binario, &quantidade_buscas);
    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);

    int erro = buscar_registro_filtro(arquivo_binario, quantidade_buscas);

    if (arquivo_binario != NULL) fclose(arquivo_binario);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }
}

void buscar_registro_rrn() {
    char nome_arquivo_binario[100];
    int rrn = 0;
    scanf("%s %d", nome_arquivo_binario, &rrn);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);

    Registro* registro = NULL;
    int erro = procurar_registro_RRN(arquivo_binario, &registro, rrn);

    free_registro(&registro);

    if (arquivo_binario != NULL) fclose(arquivo_binario);

    if (erro == FILE_NOT_FOUND_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    } else if (erro == NO_DATA_FOUND_ERROR || erro == INVALID_RRN_ERROR) {
        printf("Registro inexistente.\n");
    } else {
        print_registro(registro);
    }
}

void criar_arquivo_indice() {
    char nome_arquivo_binario[100], nome_arquivo_indice[100];
    scanf("%s %s", nome_arquivo_binario, nome_arquivo_indice);

    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);
    FILE* arquivo_indice  = fopen(nome_arquivo_indice,  MODO_SOMENTE_ESCRITA_BINARIO);

    int erro = gerar_arquivo_indice(arquivo_binario, arquivo_indice);

    if (arquivo_binario != NULL) fclose(arquivo_binario);
    if (arquivo_indice  != NULL) fclose(arquivo_indice);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    BinarioNaTela(nome_arquivo_indice);
}

/*
 * busca_indexada — case 6
 *
 * Lê: nome_dados nome_indice quantidade_buscas
 * Abre os dois arquivos em modo leitura (só lê, não modifica nada).
 * Delega toda a lógica para buscar_registro_filtro_indexado().
 */
void busca_indexada() {
    char nome_arquivo_binario[100], nome_arquivo_indice[100];
    int qtd_buscas;

    scanf("%s %s %d", nome_arquivo_binario, nome_arquivo_indice, &qtd_buscas);

    /*
     * Abrimos os dois arquivos em "rb" (somente leitura).
     * A [6] nunca modifica nada — só consulta.
     */
    FILE* arquivo_binario = fopen(nome_arquivo_binario, MODO_SOMENTE_LEITURA_BINARIO);
    FILE* arquivo_indice  = fopen(nome_arquivo_indice,  MODO_SOMENTE_LEITURA_BINARIO);

    int erro = buscar_registro_filtro_indexado(arquivo_binario,
                                               arquivo_indice,
                                               qtd_buscas);

    if (arquivo_binario != NULL) fclose(arquivo_binario);
    if (arquivo_indice  != NULL) fclose(arquivo_indice);

    if (erro != NO_ERROR) {
        printf("Falha no processamento do arquivo.\n");
    }
}
