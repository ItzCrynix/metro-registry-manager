#include "ciclos.h"

static void dfs_ciclos(Grafo* grafo, int origem, int atual, char* na_pilha, int* contador) {
    Vertice* v_atual = &grafo->vertices[atual];

    for (int a = 0; a < v_atual->qtd_arestas; a++) {
        int vizinho = busca_vertice(grafo, v_atual->arestas[a].destino);
        if (vizinho == -1) continue;

        if (vizinho == origem) {
            (*contador)++;
            continue;
        }

        if (!na_pilha[vizinho]) {
            na_pilha[vizinho] = 1;
            dfs_ciclos(grafo, origem, vizinho, na_pilha, contador);
            na_pilha[vizinho] = 0;
        }
    }
}

int contar_ciclos(FILE* arquivo_binario, const char* nome_origem, int* qtd_ciclos) {
    if (arquivo_binario == NULL || nome_origem == NULL)
        return FILE_NOT_FOUND_ERROR;

    Grafo* grafo = construir_grafo(arquivo_binario);
    if (grafo == NULL)
        return FILE_READ_ERROR;

    int idx_origem = busca_vertice(grafo, nome_origem);
    if (idx_origem == -1) {
        free_grafo(&grafo);
        return NO_DATA_FOUND_ERROR;
    }

    int n = grafo->qtd_vertices;
    char* na_pilha = (char*) calloc(n, sizeof(char));
    na_pilha[idx_origem] = 1;

    int contador = 0;
    dfs_ciclos(grafo, idx_origem, idx_origem, na_pilha, &contador);

    free(na_pilha);
    free_grafo(&grafo);

    *qtd_ciclos = contador;
    return NO_ERROR;
}
