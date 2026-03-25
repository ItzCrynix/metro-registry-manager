#ifndef REGISTRO_H
#define REGISTRO_H

#define TAM_REGISTRO_DADOS 80
#define MODO_LEITURA_BINARIO "rb+"
#define MODO_ESCRITA_BINARIO "wb+"

#define FILE_NOT_FOUND_ERROR -1
#define MALLOC_ERROR -2

typedef struct registro {
    char removido;
    int proximo_registro;
    int codigo_estacao;
    int codigo_linha;
    int codigo_proxima_estacao;
    int distancia_proxima_estacao;
    int codigo_linha_integracao;
    int codigo_estacao_integracao;

    int tamanho_nome_estacao;
    char* nome_estacao;

    int tamanho_nome_linha;
    char* nome_linha;
} Registro;

/**
 * @brief Escreve um registro novo no arquivo
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param novo_registro resitro contendo as informações de uma estação
 */
int escrever_registros_csv(FILE* nome_arquivo_csv, FILE* nome_arquivo_binario);

/**
 * @brief Lê um registro guardado no arquivo binário baseado no RRN (Relative Record Number)
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param RRN Relative Record Number, indica qual "linha" dos registros vai ser lida
 * @return Um registro, caso encontre, ou NULL
 */
Registro* ler_registro_RRN(char* nome_arquivo, int RRN);

#endif