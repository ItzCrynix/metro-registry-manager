#include "indice.h"

void organiza_lista_indice(Indice* lista, int tam) {
    int i, j;

    for (i = 1; i < tam; i++) {
        Indice aux = lista[i];
        j = i - 1;

        while (j >= 0 && lista[j].id > aux.id) {
            lista[j + 1] = lista[j];
            j = j - 1;
        }
        lista[j + 1] = aux;
    }
}

int salvar_indices(FILE* arquivo_indice, Indice* indices, int tam) {
    char status = STATUS_INCONSISTENT;
    fwrite(&status, sizeof(char), 1, arquivo_indice);

    for (int i = 0; i < tam; i++) {
        int conseguiu_escrever = 0;
        conseguiu_escrever += fwrite(&indices[i].id, sizeof(int), 1, arquivo_indice);
        conseguiu_escrever += fwrite(&indices[i].RRN, sizeof(int), 1, arquivo_indice);

        if (conseguiu_escrever < 2) {
            return INCOSISTENT_FILE_ERROR; // Talvez um outro nome para esse erro ?
        }
    }

    rewind(arquivo_indice);
    status = STATUS_CONSISTENT;
    fwrite(&status, sizeof(char), 1, arquivo_indice);

    return NO_ERROR;
}

int gerar_arquivo_indice(FILE* arquivo_binario, FILE* arquivo_indice) {
    if (arquivo_binario == NULL || arquivo_indice == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    Cabecalho* cabecalho_binario = ler_cabecalho_binario(arquivo_binario);
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
    }

    if (cabecalho_binario->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cabecalho_binario);
        return INCOSISTENT_FILE_ERROR;
    }

    if (cabecalho_binario->proximo_rrn == 0) {
        free_cabecalho(&cabecalho_binario);
        return NO_DATA_FOUND_ERROR;
    }

    Indice* indices = (Indice*) malloc(sizeof(Indice) * cabecalho_binario->proximo_rrn);
    int pos_indice = 0;

    int RRN_atual = 0;
    while (RRN_atual < cabecalho_binario->proximo_rrn) {
        int byte_offset = TAM_REGISTRO_CABECALHO + TAM_REGISTRO_DADOS * RRN_atual;
        fseek(arquivo_binario, byte_offset, SEEK_SET);

        char removido;
        fread(&removido, sizeof(char), 1, arquivo_binario);
        if (removido == STATUS_REMOVED) {
            continue;
        }

        int codigo_estacao = 0;
        fread(&codigo_estacao, sizeof(int), 1, arquivo_binario); // Primeiro campo depois do removido é o topo da pilha
        fread(&codigo_estacao, sizeof(int), 1, arquivo_binario);

        indices[pos_indice].id = codigo_estacao;
        indices[pos_indice].RRN = RRN_atual;
        pos_indice++;

        organiza_lista_indice(indices, pos_indice);

        RRN_atual++;
    }

    for (int i = 0; i < pos_indice; i++) {
        printf("{id: %d, rrn: %d}\n", indices[i].id, indices[i].RRN);
    }

    return salvar_indices(arquivo_indice, indices, pos_indice);
}
