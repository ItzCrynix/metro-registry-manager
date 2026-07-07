#include "grafos.h"

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

// ordena por nome de estação (crescente) um vetor de índices de vértices
static void ordenar_por_nome(Grafo* grafo, int* indices, int qtd) {
    for (int i = 1; i < qtd; i++) {
        int aux = indices[i];
        int j = i - 1;

        while (j >= 0 && strcmp(grafo->vertices[indices[j]].nome, grafo->vertices[aux].nome) > 0) {
            indices[j + 1] = indices[j];
            j--;
        }
        indices[j + 1] = aux;
    }
}

static void imprimir_dfs(Grafo* grafo, int no, int** filhos, int* qtd_filhos, int* peso_aresta_pai) {
    ordenar_por_nome(grafo, filhos[no], qtd_filhos[no]);

    for (int i = 0; i < qtd_filhos[no]; i++) {
        int filho = filhos[no][i];
        printf("%s, %s, %d\n", grafo->vertices[no].nome, grafo->vertices[filho].nome, peso_aresta_pai[filho]);
        imprimir_dfs(grafo, filho, filhos, qtd_filhos, peso_aresta_pai);
    }
}

int arvore_geradora_minima(FILE* arquivo_binario, const char* nome_origem) {
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

    // monta a lista de adjacência não-direcionada (linhas de ida e volta)
    ArestaND** viz = (ArestaND**) calloc(n, sizeof(ArestaND*));
    int* qtd_viz = (int*) calloc(n, sizeof(int));

    for (int u = 0; u < n; u++) {
        Vertice* vu = &grafo->vertices[u];

        for (int a = 0; a < vu->qtd_arestas; a++) {
            int v = busca_vertice(grafo, vu->arestas[a].destino);
            if (v == -1) continue;

            // estações terminais (sem nenhuma aresta de saída própria) não
            // participam da árvore geradora mínima
            if (grafo->vertices[v].qtd_arestas == 0) continue;

            int w = vu->arestas[a].distancia;

            viz[u] = (ArestaND*) realloc(viz[u], sizeof(ArestaND) * (qtd_viz[u] + 1));
            viz[u][qtd_viz[u]++] = (ArestaND) {.destino = v, .peso = w};

            viz[v] = (ArestaND*) realloc(viz[v], sizeof(ArestaND) * (qtd_viz[v] + 1));
            viz[v][qtd_viz[v]++] = (ArestaND) {.destino = u, .peso = w};
        }
    }

    // algoritmo de Prim
    int* key = (int*) malloc(sizeof(int) * n);
    int* pai = (int*) malloc(sizeof(int) * n);
    char* alcancavel = (char*) calloc(n, sizeof(char));
    char* na_agm = (char*) calloc(n, sizeof(char));

    for (int i = 0; i < n; i++) pai[i] = -1;

    key[idx_origem] = 0;
    alcancavel[idx_origem] = 1;

    for (int iteracao = 0; iteracao < n; iteracao++) {
        int u = -1;

        // (i) empate na escolha de vértices: escolhe o de menor nome
        for (int i = 0; i < n; i++) {
            if (!alcancavel[i] || na_agm[i]) continue;
            if (u == -1 || key[i] < key[u]) u = i;
        }

        if (u == -1) break;
        na_agm[u] = 1;

        for (int a = 0; a < qtd_viz[u]; a++) {
            int v = viz[u][a].destino;
            int w = viz[u][a].peso;

            if (na_agm[v]) continue;

            if (!alcancavel[v] || w < key[v]) {
                key[v] = w;
                pai[v] = u;
                alcancavel[v] = 1;
            }
            else if (w == key[v] && pai[v] != -1) {
                // (ii) empate no peso: escolhe a aresta cujo valor de u seja menor
                if (strcmp(grafo->vertices[u].nome, grafo->vertices[pai[v]].nome) < 0)
                    pai[v] = u;
            }
        }
    }

    // monta a lista de filhos de cada nó da árvore geradora mínima
    int** filhos = (int**) calloc(n, sizeof(int*));
    int* qtd_filhos = (int*) calloc(n, sizeof(int));

    for (int v = 0; v < n; v++) {
        if (v == idx_origem || !alcancavel[v]) continue;

        int p = pai[v];
        filhos[p] = (int*) realloc(filhos[p], sizeof(int) * (qtd_filhos[p] + 1));
        filhos[p][qtd_filhos[p]++] = v;
    }

    imprimir_dfs(grafo, idx_origem, filhos, qtd_filhos, key);

    for (int i = 0; i < n; i++) {
        free(viz[i]);
        free(filhos[i]);
    }
    free(viz);
    free(qtd_viz);
    free(filhos);
    free(qtd_filhos);
    free(key);
    free(pai);
    free(alcancavel);
    free(na_agm);
    free_grafo(&grafo);

    return NO_ERROR;
}

void dfs_ciclos(Grafo* grafo, int origem, int atual, char* na_pilha, int* contador) {
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