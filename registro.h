#ifndef REGISTRO_H
#define REGISTRO_H

#define TAM_REGISTRO_DADOS 80
#define MODO_LEITURA_BINARIO "rb+"
#define MODO_ESCRITA_BINARIO "wb+"

#define TAM_REGISTRO_CABECALHO 17
#define STATUS_OK 'o'
#define STATUS_UNSTABLE 'u'

#define FILE_NOT_FOUND_ERROR -1
#define MALLOC_ERROR -2

typedef struct registro Registro;

/**
 * @brief Escreve um registro novo no arquivo
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param novo_registro resitro contendo as informações de uma estação
 */
void escrever_registros_csv(FILE* nome_arquivo_csv, FILE* nome_arquivo_binario);

/**
 * @brief Lê um registro guardado no arquivo binário baseado no RRN (Relative Record Number)
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param RRN Relative Record Number, indica qual "linha" dos registros vai ser lida
 * @return Um registro, caso encontre, ou NULL
 */
Registro* ler_registro_RRN(char* nome_arquivo, int RRN);


#endif