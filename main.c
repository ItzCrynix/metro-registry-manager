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
void csv_para_binario();
void ler_arquivo_binario();

int main() {
    while (1) {
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
    }
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
    }

    fclose(arquivo_csv);
    fclose(arquivo_binario);
}

void buscar_registro_filtro() {
    char nome_arquivo_binario[100];
    int quantidade_buscas, quantidade_campos;

    // 1. Lê os dados iniciais
    scanf("%s %d", nome_arquivo_binario, &quantidade_buscas);

    while (quantidade_buscas > 0) {
        scanf("%d", &quantidade_campos);
        
        // Aloca os filtros que o usuário quer usar
        char **campos = malloc(sizeof(char *) * quantidade_campos);
        char **valores = malloc(sizeof(char *) * quantidade_campos);
        for (int i = 0; i < quantidade_campos; i++) {
            campos[i] = malloc(100);
            valores[i] = malloc(100);
            scanf("%s %s", campos[i], valores[i]); 
        }

      
        FILE *fp = fopen(nome_arquivo_binario, MODO_LEITURA_BINARIO );
        fseek(fp, 77, 0); // Pula o cabeçalho

        int encontrou_pelo_menos_um = 0;
        Registro *reg = NULL;

        
        while (ler_proximo_registro(fp, &reg)) { 
            
            int passou_no_filtro = 1; // Começa achando que o registro serve

        
            for (int i = 0; i < quantidade_campos; i++) {
                if (strcmp(campos[i], "CodEstacao") == 0) {
                    if (reg->codigo_estacao != atoi(valores[i])) passou_no_filtro = 0;
                } 
                else if (strcmp(campos[i], "NomeEstacao") == 0) {
                    if (strcmp(reg->nome_estacao, valores[i]) != 0) passou_no_filtro = 0;
                }

                 else if (strcmp(campos[i], "") == 0) {
                    if (strcmp(reg->nome_estacao, valores[i]) != 0) passou_no_filtro = 0;
                }
                 else if (strcmp(campos[i], "NomeLinha") == 0) {
                    if (strcmp(reg->nome_linha, valores[i]) != 0) passou_no_filtro = 0;
                }
                 else if (strcmp(campos[i], "CodLinha") == 0) {
                    if (strcmp(reg->codigo_linha, valores[i]) != 0) passou_no_filtro = 0;
                }
                   else if (strcmp(campos[i], "CodProxEst") == 0) {
                    if (strcmp(reg->codigo_proxima_estacao, valores[i]) != 0) passou_no_filtro = 0;
                }
                   else if (strcmp(campos[i], "DistanciaProxEst") == 0) {
                    if (strcmp(reg->distancia_proxima_estacao, valores[i]) != 0) passou_no_filtro = 0;
                }
                 else if (strcmp(campos[i], "CodLinhaInteg") == 0) {
                    if (strcmp(reg->codigo_linha_integracao, valores[i]) != 0) passou_no_filtro = 0;
                }
                 else if (strcmp(campos[i], "CodEstacaoInteg") == 0) {
                    if (strcmp(reg->codigo_estacao_integracao, valores[i]) != 0) passou_no_filtro = 0;
                }
            }

            if (passou_no_filtro && reg->removido == '0') {
                printf("%s\n", to_string(reg));
                encontrou_pelo_menos_um = 1;
            }
            
            free_registro(&reg);
        }

        if (!encontrou_pelo_menos_um) printf("Registro inexistente.\n");

        for(int i=0; i<quantidade_campos; i++) { free(campos[i]); free(valores[i]); }
        free(campos); free(valores);
        fclose(fp);

        quantidade_buscas--;
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

    fclose(arquivo_binario);
}   
