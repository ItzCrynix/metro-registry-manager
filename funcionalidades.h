#ifndef FUNCS_H
#define FUNCS_H

#define MODO_LEITURA_BINARIO "rb+"
#define MODO_ESCRITA_BINARIO "wb+"

#define NO_ERROR 0
#define FILE_NOT_FOUND_ERROR -1
#define MALLOC_ERROR -2
#define NO_DATA_FOUND_ERROR -3
#define RRN_NOT_FOUND -3
#define RRN_REMOVED '1'

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

int printar_arquivo_binario(FILE* arquivo_binario);

#endif