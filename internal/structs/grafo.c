#include "grafo.h"

typedef struct {
    int codigo;
    char* nome;
} CodigoNome;

// busca linear pelo nome associado a um código de estação
static const char* nome_por_codigo(CodigoNome* mapa, int qtd, int codigo) {
    for (int i = 0; i < qtd; i++) {
        if (mapa[i].codigo == codigo)
            return mapa[i].nome;
    }
    return NULL;
}

//
// Criamos essas três funções para passar como parâmetro no qsort, que pede um comparador como argumento
//

static int cmp_str(const void* a, const void* b) {
    return strcmp(*(const char* const*) a, *(const char* const*) b);
}

static int cmp_vertice(const void* a, const void* b) {
    return strcmp(((Vertice*) a)->nome, ((Vertice*) b)->nome);
}

static int cmp_aresta(const void* a, const void* b) {
    return strcmp(((Aresta*) a)->destino, ((Aresta*) b)->destino);
}

// procura (ou cria, caso não exista) o vértice de uma estação pelo nome
static Vertice* obter_vertice(Grafo* grafo, int* cap_vertices, const char* nome) {
    for (int i = 0; i < grafo->qtd_vertices; i++) {
        if (strcmp(grafo->vertices[i].nome, nome) == 0)
            return &grafo->vertices[i];
    }

    if (grafo->qtd_vertices >= *cap_vertices) {
        *cap_vertices += 32;
        grafo->vertices = (Vertice*) realloc(grafo->vertices, sizeof(Vertice) * (*cap_vertices));
    }

    Vertice* novo = &grafo->vertices[grafo->qtd_vertices++];
    novo->nome = strdup(nome);
    novo->qtd_arestas = 0;
    novo->arestas = NULL;

    return novo;
}

// adiciona (ou mescla, caso já exista uma aresta para o mesmo destino) uma aresta a um vértice
static void adicionar_aresta(Vertice* origem, const char* destino, int distancia, const char* linha) {
    for (int i = 0; i < origem->qtd_arestas; i++) {
        if (strcmp(origem->arestas[i].destino, destino) == 0) {
            Aresta* aresta = &origem->arestas[i];

            // verifica se a linha já está presente nessa aresta
            for (int j = 0; j < aresta->qtd_linhas; j++) {
                if (strcmp(aresta->linhas[j], linha) == 0)
                    return;
            }

            aresta->linhas = (char**) realloc(aresta->linhas, sizeof(char*) * (aresta->qtd_linhas + 1));
            aresta->linhas[aresta->qtd_linhas++] = strdup(linha);
            return;
        }
    }

    origem->arestas = (Aresta*) realloc(origem->arestas, sizeof(Aresta) * (origem->qtd_arestas + 1));
    Aresta* aresta = &origem->arestas[origem->qtd_arestas++];

    aresta->destino = strdup(destino);
    aresta->distancia = distancia;
    aresta->qtd_linhas = 1;
    aresta->linhas = (char**) malloc(sizeof(char*));
    aresta->linhas[0] = strdup(linha);
}

Grafo* construir_grafo(FILE* arquivo_binario) {
    if (arquivo_binario == NULL)
        return NULL;

    Cabecalho* cab = novo_cabecalho();
    if (cab == NULL)
        return NULL;

    if (ler_cabecalho_binario(arquivo_binario, cab) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cab);
        return NULL;
    }

    if (cab->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cab);
        return NULL;
    }

    // lê todos os registros não removidos para a memória
    int qtd_registros = 0;
    int cap_registros = cab->proximo_rrn > 0 ? cab->proximo_rrn : 1;
    Registro** registros = (Registro**) malloc(sizeof(Registro*) * cap_registros);

    for (int rrn = 0; rrn < cab->proximo_rrn; rrn++) {
        Registro* reg = ler_registro_RRN(arquivo_binario, rrn);
        if (reg == NULL)
            continue;

        registros[qtd_registros++] = reg;
    }

    free_cabecalho(&cab);

    // constrói o mapa código -> nome da estação
    int qtd_mapa = 0;
    CodigoNome* mapa = (CodigoNome*) malloc(sizeof(CodigoNome) * (qtd_registros > 0 ? qtd_registros : 1));

    for (int i = 0; i < qtd_registros; i++) {
        int ja_existe = 0;
        for (int j = 0; j < qtd_mapa; j++) {
            if (mapa[j].codigo == registros[i]->codigo_estacao) {
                ja_existe = 1;
                break;
            }
        }

        if (!ja_existe) {
            mapa[qtd_mapa].codigo = registros[i]->codigo_estacao;
            mapa[qtd_mapa].nome = registros[i]->nome_estacao;
            qtd_mapa++;
        }
    }

    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->vertices = NULL;
    grafo->qtd_vertices = 0;
    int cap_vertices = 0;

    // garante que toda estação (mesmo sem arestas de saída) vire um vértice
    for (int i = 0; i < qtd_registros; i++) {
        obter_vertice(grafo, &cap_vertices, registros[i]->nome_estacao);
    }

    // monta as arestas
    for (int i = 0; i < qtd_registros; i++) {
        Registro* reg = registros[i];

        if (reg->codigo_proxima_estacao != -1) {
            const char* nome_destino = nome_por_codigo(mapa, qtd_mapa, reg->codigo_proxima_estacao);

            if (nome_destino != NULL) {
                Vertice* origem = obter_vertice(grafo, &cap_vertices, reg->nome_estacao);
                adicionar_aresta(origem, nome_destino, reg->distancia_proxima_estacao, reg->nome_linha);
            }
        }

        if (reg->codigo_estacao_integracao != -1) {
            const char* nome_integra = nome_por_codigo(mapa, qtd_mapa, reg->codigo_estacao_integracao);

            if (nome_integra != NULL && strcmp(nome_integra, reg->nome_estacao) != 0) {
                Vertice* origem = obter_vertice(grafo, &cap_vertices, reg->nome_estacao);
                adicionar_aresta(origem, nome_integra, 0, "Integração");
            }
        }
    }

    free(mapa);

    for (int i = 0; i < qtd_registros; i++)
        free_registro(&registros[i]);
    free(registros);

    // ordena vértices, arestas e nomes de linhas em ordem crescente
    qsort(grafo->vertices, grafo->qtd_vertices, sizeof(Vertice), cmp_vertice);

    for (int i = 0; i < grafo->qtd_vertices; i++) {
        Vertice* v = &grafo->vertices[i];
        qsort(v->arestas, v->qtd_arestas, sizeof(Aresta), cmp_aresta);

        for (int j = 0; j < v->qtd_arestas; j++)
            qsort(v->arestas[j].linhas, v->arestas[j].qtd_linhas, sizeof(char*), cmp_str);
    }

    return grafo;
}

void free_grafo(Grafo** grafo) {
    if (grafo == NULL || *grafo == NULL) return;

    for (int i = 0; i < (*grafo)->qtd_vertices; i++) {
        Vertice* v = &(*grafo)->vertices[i];

        for (int j = 0; j < v->qtd_arestas; j++) {
            for (int k = 0; k < v->arestas[j].qtd_linhas; k++)
                free(v->arestas[j].linhas[k]);
            free(v->arestas[j].linhas);
            free(v->arestas[j].destino);
        }

        free(v->arestas);
        free(v->nome);
    }

    free((*grafo)->vertices);
    free(*grafo);
    *grafo = NULL;
}

int busca_vertice(Grafo* grafo, const char* nome) {
    if (grafo == NULL || nome == NULL) return -1;

    int esq = 0;
    int dir = grafo->qtd_vertices - 1;

    while (esq <= dir) {
        int meio = (esq + dir) / 2;
        int cmp = strcmp(grafo->vertices[meio].nome, nome);

        if (cmp == 0)
            return meio;

        if (cmp < 0)
            esq = meio + 1;
        else
            dir = meio - 1;
    }

    return -1;
}
