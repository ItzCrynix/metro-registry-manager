#ifndef REGISTRO_H
#define REGISTRO_H

#define TAM_REG_REGISTRO 80
#define MODO_LEITURA_BINARIO "rb+"
#define MODO_ESCRITA_BINARIO "wb+"

#define FILE_NOT_FOUND_ERROR -1

typedef struct registro Registro;

/**
 * @brief Escreve um registro novo no arquivo
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param novo_registro resitro contendo as informações de uma estação
 */
int escrever_registro_csv(char* nome_arquivo_csv, char* nome_arquivo_binario);

/**
 * @brief Lê um registro guardado no arquivo binário baseado no RRN (Relative Record Number)
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param RRN Relative Record Number, indica qual "linha" dos registros vai ser lida
 */
Registro* ler_registro_RRN(char* nome_arquivo, int RRN);


#endif