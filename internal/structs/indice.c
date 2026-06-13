#include "indice.h"

/* ============================================================
 * FUNÇÕES JÁ EXISTENTES (não alteradas)
 * ============================================================ */

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
    char status = STATUS_INCONSISTENT;
    fwrite(&status, sizeof(char), 1, arquivo_indice);

    for (int i = 0; i < tam; i++) {
        int conseguiu_escrever = 0;
        conseguiu_escrever += fwrite(&indices[i].id,  sizeof(int), 1, arquivo_indice);
        conseguiu_escrever += fwrite(&indices[i].RRN, sizeof(int), 1, arquivo_indice);

        if (conseguiu_escrever < 2) {
            return INCOSISTENT_FILE_ERROR;
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

    Cabecalho* cabecalho_binario = novo_cabecalho();
    if (cabecalho_binario == NULL) {
        return MALLOC_ERROR;
    }

    if (ler_cabecalho_binario(arquivo_binario, cabecalho_binario) < NUM_CAMPOS_CABECALHO) {
        return FILE_READ_ERROR;
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
        int byte_offset = TAM_REGISTRO_CABECALHO + TAM_REGISTRO_DADOS * RRN_atual++;
        fseek(arquivo_binario, byte_offset, SEEK_SET);

        char removido;
        fread(&removido, sizeof(char), 1, arquivo_binario);
        if (removido == STATUS_REMOVED) {
            continue;
        }

        int codigo_estacao = 0;
        fread(&codigo_estacao, sizeof(int), 1, arquivo_binario);
        fread(&codigo_estacao, sizeof(int), 1, arquivo_binario);

        indices[pos_indice].id  = codigo_estacao;
        indices[pos_indice].RRN = RRN_atual - 1;
        pos_indice++;

        organiza_lista_indice(indices, pos_indice);
    }

    int resultado = salvar_indices(arquivo_indice, indices, pos_indice);
    free(indices);
    free_cabecalho(&cabecalho_binario);
    return resultado;
}

/* ============================================================
 * NOVAS FUNÇÕES — usadas pela funcionalidade [6]
 * ============================================================ */

/*
 * carregar_indice
 *
 * Lê o arquivo de índice do disco e coloca tudo num vetor na memória.
 *
 * POR QUE fazemos isso?
 * Fazer busca binária diretamente no disco seria lento: cada comparação
 * exigiria um fseek + fread. Trazer tudo para a RAM de uma vez e buscar
 * no vetor é muito mais rápido — as comparações acontecem na memória.
 *
 * COMO funciona o arquivo de índice:
 *   byte 0        → status (1 char)
 *   bytes 1..8    → entrada 0: [codEstacao int][RRN int]
 *   bytes 9..16   → entrada 1: [codEstacao int][RRN int]
 *   ...
 *
 * Então: quantidade de entradas = (tamanho_do_arquivo - 1) / 8
 */
Indice* carregar_indice(FILE* arquivo_indice, int* qtd) {
    if (arquivo_indice == NULL) {
        return NULL;
    }

    /* Lê o status (byte 0) e verifica se o arquivo está consistente */
    char status;
    rewind(arquivo_indice);
    fread(&status, sizeof(char), 1, arquivo_indice);

    if (status != STATUS_CONSISTENT) {
        return NULL;
    }

    /*
     * Descobre o tamanho total do arquivo.
     * fseek com SEEK_END move o cursor para depois do último byte.
     * ftell diz em que posição (byte) o cursor está — ou seja, o tamanho.
     */
    fseek(arquivo_indice, 0, SEEK_END);
    long tamanho = ftell(arquivo_indice);

    /*
     * Calcula quantas entradas existem.
     * Subtrai 1 (o byte do cabeçalho), divide por 8 (cada entrada ocupa 8 bytes).
     */
    int quantidade = (int)((tamanho - TAM_CAB_INDICE) / TAM_REG_INDICE);

    if (quantidade <= 0) {
        *qtd = 0;
        return NULL;
    }

    /*
     * Aloca o vetor em memória.
     * malloc(N * sizeof(Indice)) reserva espaço para N structs Indice.
     * É como pedir ao sistema: "me dê N gavetas do tamanho de Indice".
     */
    Indice* vec = (Indice*) malloc(sizeof(Indice) * quantidade);
    if (vec == NULL) {
        return NULL;
    }

    /*
     * Volta para o byte 1 (logo após o status) e lê cada entrada.
     * Cada entrada tem: [int id][int RRN] = 4 + 4 = 8 bytes.
     * Lemos campo por campo — nunca a struct inteira de uma vez.
     */
    fseek(arquivo_indice, TAM_CAB_INDICE, SEEK_SET);
    for (int i = 0; i < quantidade; i++) {
        fread(&vec[i].id,  sizeof(int), 1, arquivo_indice);
        fread(&vec[i].RRN, sizeof(int), 1, arquivo_indice);
    }

    /* Diz para quem chamou quantas entradas foram lidas */
    *qtd = quantidade;
    return vec;
}

/*
 * busca_binaria_indice
 *
 * Encontra a posição de um codEstacao no vetor de índices.
 *
 * POR QUE busca binária (e não percorrer tudo)?
 * O índice está ordenado por codEstacao crescente.
 * A busca binária funciona como o jogo "adivinhe o número":
 * sempre chuta o meio e descarta metade das opções.
 * Com 1000 entradas, são no máximo log2(1000) ≈ 10 comparações.
 * Percorrer tudo seria até 1000 comparações.
 *
 * RETORNA: posição no vetor onde achou, ou -1 se não encontrou.
 */
int busca_binaria_indice(Indice* indices, int qtd, int cod_estacao) {
    int esquerda = 0;
    int direita  = qtd - 1;

    while (esquerda <= direita) {
        /*
         * Calcula o índice do meio entre esquerda e direita.
         * Ex: esquerda=0, direita=9 → meio=4
         */
        int meio = (esquerda + direita) / 2;

        if (indices[meio].id == cod_estacao) {
            return meio; /* Achou! Retorna a posição no vetor */
        }

        if (indices[meio].id < cod_estacao) {
            /*
             * O valor do meio é MENOR que o buscado.
             * O que procuramos está na metade DIREITA.
             * Move a borda esquerda para depois do meio.
             */
            esquerda = meio + 1;
        } else {
            /*
             * O valor do meio é MAIOR que o buscado.
             * O que procuramos está na metade ESQUERDA.
             * Move a borda direita para antes do meio.
             */
            direita = meio - 1;
        }
    }

    return -1; /* Não encontrou */
}
