#ifndef INDICE_H
#define INDICE_H

#include "../utils/utils.h"
#include "../structs/cabecalho.h"
#include "../structs/registro.h"

/* Tamanho do cabeçalho do arquivo de índice: 1 byte (status) */
#define TAM_CABECALHO_INDICE 1
/* Tamanho de cada registro do índice: 4 bytes (codEstacao) + 4 bytes (RRN) */
#define TAM_REGISTRO_INDICE 8

typedef struct indice {
    int id;   /* codEstacao — chave de busca */
    int RRN;  /* posição do registro no arquivo de dados */
} Indice;

void free_indice(Indice** indice);

/**
 * @brief Ordena a lista de índices por id em ordem crescente (insertion sort)
 */
void organiza_lista_indice(Indice* lista, int tam);

/**
 * @brief Grava o cabeçalho + todos os registros de índice no arquivo binário
 */
int salvar_indices(FILE* arquivo_indice, Indice* indices, int tam);

/**
 * @brief [5] Cria o arquivo de índice primário a partir do arquivo de dados
 */
int gerar_arquivo_indice(FILE* arquivo_binario, FILE* arquivo_indice);

/**
 * @brief Lê todos os registros do arquivo de índice para um vetor em memória.
 *        Retorna o vetor alocado e preenche *qtd com a quantidade de entradas.
 *        Retorna NULL em caso de erro.
 */
Indice* carregar_indice(FILE* arquivo_indice, int* qtd);

/**
 * @brief Busca binária no vetor de índices pelo codEstacao.
 *        Retorna a posição no vetor, ou -1 se não encontrar.
 */
int busca_binaria_indice(Indice* indices, int qtd, int cod_estacao);

/**
 * @brief Remove a entrada com o id informado do arquivo de índice.
 *        Lê o índice, remove a entrada, reescreve o arquivo.
 */
int remover_do_indice(FILE* arquivo_indice, int cod_estacao);

/**
 * @brief Insere uma nova entrada (cod_estacao, rrn) no arquivo de índice,
 *        mantendo a ordenação crescente por codEstacao.
 */
int inserir_no_indice(FILE* arquivo_indice, int cod_estacao, int rrn);

/**
 * @brief Atualiza o RRN de uma entrada existente no índice (usado no UPDATE
 *        quando o codEstacao muda) ou apenas mantém consistente.
 *        Na prática: remove o antigo e insere o novo.
 */
int atualizar_no_indice(FILE* arquivo_indice, int cod_antigo, int cod_novo, int rrn);

#endif
