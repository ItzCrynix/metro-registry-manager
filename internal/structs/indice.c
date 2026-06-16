#include "indice.h"

void free_indice(Indice** indice) {
    if (indice == NULL || *indice == NULL) return;

    free(*indice);

    *indice = NULL;
}

void organiza_lista_indice(Indice* lista, int tam) {
    for (int i = 1; i < tam; i++) {
        Indice aux = lista[i];
        int j = i - 1;

        while (j >= 0 && lista[j].id > aux.id) {
            lista[j + 1] = lista[j];
            j = j - 1;
        }
        lista[j + 1] = aux;
    }
}

int salvar_indices(FILE* arquivo_indice, Indice* indices, int tam) {
    rewind(arquivo_indice);
    char status = STATUS_INCONSISTENT;
    fwrite(&status, sizeof(char), 1, arquivo_indice);

    for (int i = 0; i < tam; i++) {
        int escritos = 0;
        escritos += fwrite(&indices[i].id,  sizeof(int), 1, arquivo_indice);
        escritos += fwrite(&indices[i].RRN, sizeof(int), 1, arquivo_indice);

        if (escritos < 2)
            return INCOSISTENT_FILE_ERROR;
    }

    rewind(arquivo_indice);
    status = STATUS_CONSISTENT;
    fwrite(&status, sizeof(char), 1, arquivo_indice);

    return NO_ERROR;
}

int gerar_arquivo_indice(FILE* arquivo_binario, FILE* arquivo_indice) {
    if (arquivo_binario == NULL || arquivo_indice == NULL)
        return FILE_NOT_FOUND_ERROR;

    Cabecalho* cab = novo_cabecalho();
    if (cab == NULL)
        return MALLOC_ERROR;

    if (ler_cabecalho_binario(arquivo_binario, cab) < NUM_CAMPOS_CABECALHO)
        return FILE_READ_ERROR;

    if (cab->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cab);
        return INCOSISTENT_FILE_ERROR;
    }

    if (cab->proximo_rrn == 0) {
        free_cabecalho(&cab);
        return NO_DATA_FOUND_ERROR;
    }

    Indice* indices = (Indice*) malloc(sizeof(Indice) * cab->proximo_rrn);
    int pos = 0;
    int rrn = 0;

    while (rrn < cab->proximo_rrn) {
        int offset = TAM_CABECALHO_REGISTRO + TAM_REGISTRO_DADOS * rrn++;
        fseek(arquivo_binario, offset, SEEK_SET);

        char removido;
        fread(&removido, sizeof(char), 1, arquivo_binario);
        if (removido == STATUS_REMOVED)
            continue;

        int cod = 0;
        fread(&cod, sizeof(int), 1, arquivo_binario);
        fread(&cod, sizeof(int), 1, arquivo_binario);

        indices[pos].id  = cod;
        indices[pos].RRN = rrn - 1;
        pos++;

        organiza_lista_indice(indices, pos);
    }

    int resultado = salvar_indices(arquivo_indice, indices, pos);
    free(indices);
    free_cabecalho(&cab);
    return resultado;
}

// carrega o arquivo de indice em um vetor na memoria para permitir busca binaria
Indice* carregar_indice(FILE* arquivo_indice, int* qtd) {
    if (arquivo_indice == NULL)
        return NULL;

    char status;
    rewind(arquivo_indice);
    fread(&status, sizeof(char), 1, arquivo_indice);

    if (status != STATUS_CONSISTENT)
        return NULL;

    fseek(arquivo_indice, 0, SEEK_END);
    long tamanho = ftell(arquivo_indice);

    int quantidade = (int)((tamanho - TAM_CABECALHO_INDICE) / TAM_REGISTRO_INDICE);

    if (quantidade <= 0) {
        *qtd = 0;
        return NULL;
    }

    Indice* vec = (Indice*) malloc(sizeof(Indice) * quantidade);
    if (vec == NULL)
        return NULL;

    fseek(arquivo_indice, TAM_CABECALHO_INDICE, SEEK_SET);
    for (int i = 0; i < quantidade; i++) {
        fread(&vec[i].id,  sizeof(int), 1, arquivo_indice);
        fread(&vec[i].RRN, sizeof(int), 1, arquivo_indice);
    }

    *qtd = quantidade;
    return vec;
}

// busca binaria pelo codEstacao no vetor de indices
// retorna a posicao no vetor, ou -1 se nao encontrar
int busca_binaria_indice(Indice* indices, int qtd, int cod_estacao) {
    int esq = 0;
    int dir = qtd - 1;

    while (esq <= dir) {
        int meio = (esq + dir) / 2;

        if (indices[meio].id == cod_estacao)
            return meio;

        if (indices[meio].id < cod_estacao)
            esq = meio + 1;
        else
            dir = meio - 1;
    }

    return -1;
}
