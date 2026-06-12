#ifndef BINARIO_H
#define BINARIO_H

#include "../structs/cabecalho.h"
#include "../structs/par_estacao.h"
#include "../structs/registro.h"
#include "../utils/utils.h"

/**
 * @brief Escreve um registro novo no arquivo
 * @param nome_arquivo nome do arquivo binário que será usado
 * @param novo_registro resitro contendo as informações de uma estação
 */
int escrever_csv_para_binario(FILE* nome_arquivo_csv, FILE* nome_arquivo_binario);

/**
 * @brief Lê um arquivo binário e printa no terminal
 * @param arquivo_binario O arquivo que será lido
 */
int printar_arquivo_binario(FILE* arquivo_binario);

#endif