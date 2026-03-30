#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"
#include "utils.h"
#include "cabecalho.h"

#define MODO_LEITURA_BINARIO "rb+"
#define MODO_ESCRITA_BINARIO "wb+"

#define NO_ERROR 0
#define FILE_NOT_FOUND_ERROR -1
#define MALLOC_ERROR -2
#define NO_DATA_FOUND_ERROR -3

/**
 * @brief Escreve um registro novo no arquivo
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param novo_registro resitro contendo as informações de uma estação
 */
int escrever_registros_csv(FILE* nome_arquivo_csv, FILE* nome_arquivo_binario);

/**
 * @brief Lê um arquivo binário e printa no terminal
 * @param arquivo_binario O arquivo que será lido
 */
int printar_arquivo_binario(FILE* arquivo_binario);

int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int rrn);

#endif