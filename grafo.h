#ifndef GRAFO_H
#define GRAFO_H

#include "utils.h"
#include "cabecalho.h"
#include "registro.h"

/**
 * Representa uma aresta direcionada nome_origem -> nome_destino, com a
 * distância percorrida e o(s) nome(s) da(s) linha(s) que atendem esse trecho
 * (pode haver mais de uma linha ligando fisicamente as duas estações).
 */
typedef struct aresta {
    char* destino;
    int distancia;

    int qtd_linhas;
    char** linhas; // ordenadas de forma crescente
} Aresta;

/**
 * Representa um vértice (estação) do grafo e sua lista de adjacência.
 */
typedef struct vertice {
    char* nome;

    int qtd_arestas;
    Aresta* arestas; // ordenadas de forma crescente pelo nome da estação destino
} Vertice;

typedef struct grafo {
    Vertice* vertices; // ordenados de forma crescente pelo nome da estação
    int qtd_vertices;
} Grafo;

/**
 * @brief Constrói o grafo direcionado a partir do arquivo binário de estações,
 *        agrupando estações repetidas e mesclando linhas paralelas entre o
 *        mesmo par origem/destino.
 * @param arquivo_binario arquivo binário já aberto para leitura
 * @return o grafo montado, ou NULL em caso de erro
 */
Grafo* construir_grafo(FILE* arquivo_binario);

/**
 * @brief Libera toda a memória alocada para o grafo
 */
void free_grafo(Grafo** grafo);

/**
 * @brief Busca binária pelo nome da estação dentro do vetor de vértices
 * @return o índice do vértice, ou -1 se não encontrado
 */
int busca_vertice(Grafo* grafo, const char* nome);

#endif
