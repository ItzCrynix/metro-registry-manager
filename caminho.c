#include "caminho.h"

int caminho_mais_curto(FILE* arquivo_binario, const char* nome_origem, const char* nome_destino) {
    if (arquivo_binario == NULL || nome_origem == NULL || nome_destino == NULL)
        return FILE_NOT_FOUND_ERROR;

    Grafo* grafo = construir_grafo(arquivo_binario);
    if (grafo == NULL)
        return FILE_READ_ERROR;

    int idx_origem = busca_vertice(grafo, nome_origem);
    int idx_destino = busca_vertice(grafo, nome_destino);

    if (idx_origem == -1 || idx_destino == -1) {
        free_grafo(&grafo);
        return NO_DATA_FOUND_ERROR;
    }

    int n = grafo->qtd_vertices;
    int* dist = (int*) malloc(sizeof(int) * n);
    int* pred = (int*) malloc(sizeof(int) * n);
    char* alcancavel = (char*) calloc(n, sizeof(char));
    char* visitado = (char*) calloc(n, sizeof(char));

    for (int i = 0; i < n; i++) {
        dist[i] = 0;
        pred[i] = -1;
    }

    dist[idx_origem] = 0;
    alcancavel[idx_origem] = 1;

    // algoritmo de Dijkstra
    for (int iteracao = 0; iteracao < n; iteracao++) {
        int u = -1;

        // (i) empate na escolha de vértices: escolhe o de menor nome
        // (como os vértices estão ordenados de forma crescente pelo nome,
        // basta escolher o menor índice com a menor distância)
        for (int i = 0; i < n; i++) {
            if (!alcancavel[i] || visitado[i]) continue;
            if (u == -1 || dist[i] < dist[u]) u = i;
        }

        if (u == -1) break;
        visitado[u] = 1;

        Vertice* vu = &grafo->vertices[u];
        for (int a = 0; a < vu->qtd_arestas; a++) {
            Aresta* aresta = &vu->arestas[a];
            int v = busca_vertice(grafo, aresta->destino);
            if (v == -1) continue;

            int nova_dist = dist[u] + aresta->distancia;

            if (!alcancavel[v] || nova_dist < dist[v]) {
                dist[v] = nova_dist;
                pred[v] = u;
                alcancavel[v] = 1;
            }
            else if (nova_dist == dist[v] && pred[v] != -1) {
                // (ii) empate no peso: escolhe a aresta cujo valor de u seja menor
                if (strcmp(vu->nome, grafo->vertices[pred[v]].nome) < 0)
                    pred[v] = u;
            }
        }
    }

    int erro = NO_ERROR;

    if (!alcancavel[idx_destino]) {
        erro = SEM_CAMINHO;
    }
    else {
        // reconstrói o caminho percorrendo os predecessores
        int cap_caminho = n;
        int* caminho = (int*) malloc(sizeof(int) * cap_caminho);
        int qtd_caminho = 0;

        int atual = idx_destino;
        while (atual != -1) {
            caminho[qtd_caminho++] = atual;
            if (atual == idx_origem) break;
            atual = pred[atual];
        }

        printf("Numero de estacoes que serao percorridas: %d\n", qtd_caminho - 1);
        printf("Distancia que sera percorrida: %d\n", dist[idx_destino]);

        for (int i = qtd_caminho - 1; i >= 0; i--) {
            printf("%s", grafo->vertices[caminho[i]].nome);
            if (i > 0) printf(", ");
        }
        printf("\n");

        free(caminho);
    }

    free(dist);
    free(pred);
    free(alcancavel);
    free(visitado);
    free_grafo(&grafo);

    return erro;
}
