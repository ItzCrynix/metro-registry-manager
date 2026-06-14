#include "filtro.h"

Filtro* ler_filtros(int* qtd){
    scanf("%d ", qtd);
    Filtro *filtros = (Filtro*) malloc(sizeof(Filtro) * *qtd);

    for(int i = 0; i < *qtd; i++) {
        filtros[i].campo = calloc(100, sizeof(char));
        filtros[i].valor = calloc(100, sizeof(char));
        scanf("%s", filtros[i].campo);
        ScanQuoteString(filtros[i].valor);
    }

    return filtros;

}

// Libera memória alocada para filtros
void free_filtro(Filtro** filtro, int tam) {
    if (filtro == NULL || *filtro == NULL) return;

    for (int i = 0; i < tam; i++) {
        free((*filtro)[i].campo);
        free((*filtro)[i].valor);
        (*filtro)[i].campo = NULL;
        (*filtro)[i].valor = NULL;
    }

    free(*filtro);
    *filtro = NULL;
}

int encontrouReg(Registro* reg, Filtro* filtros, int qtd_filtros) {
    int correspondencias = 0;

    for (int i = 0; i < qtd_filtros; i++) {
        if (passou_no_filtro(reg, &filtros[i]))
            correspondencias++;
    }

    if (correspondencias == qtd_filtros) {
        print_registro(reg);    
        free_registro(&reg);   
        return 1;
    }

    free_registro(&reg);       
    return 0;
}

int passou_no_filtro(Registro* registro, Filtro* filtro) {
    if (strcmp(filtro->campo, "codEstacao") == 0)
        return registro->codigo_estacao == atoi(filtro->valor);
    if (strcmp(filtro->campo, "codLinha") == 0)
        return registro->codigo_linha == atoi(filtro->valor);
    if (strcmp(filtro->campo, "codProxEstacao") == 0)
        return registro->codigo_proxima_estacao == atoi(filtro->valor);
    if (strcmp(filtro->campo, "distProxEstacao") == 0)
        return registro->distancia_proxima_estacao == atoi(filtro->valor);
    if (strcmp(filtro->campo, "codLinhaIntegra") == 0)
        return registro->codigo_linha_integracao == atoi(filtro->valor);
    if (strcmp(filtro->campo, "codEstIntegra") == 0)
        return registro->codigo_estacao_integracao == atoi(filtro->valor);
    if (strcmp(filtro->campo, "nomeEstacao") == 0)
        return strcmp(registro->nome_estacao, filtro->valor) == 0;
    if (strcmp(filtro->campo, "nomeLinha") == 0)
        return strcmp(registro->nome_linha, filtro->valor) == 0;

    return 0;
}
