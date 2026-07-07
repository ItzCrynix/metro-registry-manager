#include "adjacencia.h"

int imprimir_lista_adjacencia(FILE* arquivo_binario) {
    if (arquivo_binario == NULL)
        return FILE_NOT_FOUND_ERROR;

    Grafo* grafo = construir_grafo(arquivo_binario);
    if (grafo == NULL)
        return FILE_READ_ERROR;

    for (int i = 0; i < grafo->qtd_vertices; i++) {
        Vertice* v = &grafo->vertices[i];

        if (v->qtd_arestas == 0)
            continue;

        printf("%s", v->nome);

        for (int j = 0; j < v->qtd_arestas; j++) {
            Aresta* a = &v->arestas[j];

            printf(", %s, %d", a->destino, a->distancia);

            for (int k = 0; k < a->qtd_linhas; k++) {
                printf(", %s", a->linhas[k]);
            }
        }

        printf("\n");
    }

    free_grafo(&grafo);
    return NO_ERROR;
}
