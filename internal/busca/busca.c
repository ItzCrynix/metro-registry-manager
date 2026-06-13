#include "busca.h"

int procurar_registro_RRN(FILE* arquivo_binario, Registro** registro, int rrn) {
    if (arquivo_binario == NULL) {
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
        return INCOSISTENT_FILE_ERROR;
    }

    if (rrn < 0 || rrn >= cabecalho_binario->proximo_rrn) {
        free_cabecalho(&cabecalho_binario);
        return INVALID_RRN_ERROR;
    }

    free_cabecalho(&cabecalho_binario);

    *registro = ler_registro_RRN(arquivo_binario, rrn);
    if (*registro == NULL) {
        return NO_DATA_FOUND_ERROR;
    }

    return NO_ERROR;
}


int buscar_registro_filtro(FILE* arquivo_binario, int qtd_buscas) {
    if (arquivo_binario == NULL) {
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

    while (qtd_buscas > 0) {
        int qtd_campos;
        scanf("%d ", &qtd_campos);

        Filtro* filtros = (Filtro*) malloc(sizeof(Filtro) * qtd_campos);

        // Pega todos os campos que serão pesquisados
        for (int i = 0; i < qtd_campos; i++) {
            filtros[i].campo = calloc(100, sizeof(char));
            filtros[i].valor = calloc(100, sizeof(char));

            scanf("%s", filtros[i].campo);
            ScanQuoteString(filtros[i].valor);
        }

        int encontrou_um = 0;

        // passa registro por registro procurando um correspondente
        for (int RRN_atual = 0; RRN_atual < cabecalho_binario->proximo_rrn; RRN_atual++) {
            Registro* registro_atual = ler_registro_RRN(arquivo_binario, RRN_atual);
            if (registro_atual == NULL) {
                continue;
            }
           
            int qtd_correspondencias = 0;
            for (int i = 0; i < qtd_campos; i++) {
                if (passou_no_filtro(registro_atual, (filtros + i))) {
                    qtd_correspondencias++;
                }
            }

            if (qtd_correspondencias == qtd_campos) {
                print_registro(registro_atual);
                encontrou_um = 1;
            }

            free_registro(&registro_atual);
        }

        if (encontrou_um == 0) {
            printf("Registro inexistente.");
        }

        qtd_buscas--;
        
        if (qtd_buscas > 0) {
            printf("\n");
        }

        free_filtro(&filtros, qtd_campos);
    }

    free_cabecalho(&cabecalho_binario);

    return NO_ERROR;
}

int passou_no_filtro(Registro* registro, Filtro* filtro) {
        if (strcmp(filtro->campo, "codEstacao") == 0) {
            return registro->codigo_estacao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "codLinha") == 0) {
            return registro->codigo_linha == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "codProxEstacao") == 0) {
            return registro->codigo_proxima_estacao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "distProxEstacao") == 0) {
            return registro->distancia_proxima_estacao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "codLinhaIntegra") == 0) {
            return registro->codigo_linha_integracao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "codEstIntegra") == 0) {
            return registro->codigo_estacao_integracao == atoi(filtro->valor);
        }
        if (strcmp(filtro->campo, "nomeEstacao") == 0) {
            return strcmp(registro->nome_estacao, filtro->valor) == 0;
        }
        if (strcmp(filtro->campo, "nomeLinha") == 0) {
            return strcmp(registro->nome_linha, filtro->valor) == 0;
        }
        
        return 0;
}

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
/* ============================================================
 * FUNCIONALIDADE [6] — Busca com apoio do índice primário
 * ============================================================ */

/*
 * buscar_registro_filtro_indexado
 *
 * Diferença em relação à [3] (buscar_registro_filtro):
 *   [3] sempre varre TODOS os registros (busca sequencial).
 *   [6] verifica se algum filtro é por "codEstacao":
 *       → SIM: usa o índice (busca binária) — muito mais rápido.
 *       → NÃO: faz busca sequencial igual à [3].
 *
 * ESTRUTURA GERAL:
 *   Para cada busca pedida pelo usuário:
 *     1. Lê os filtros da entrada padrão
 *     2. Verifica se algum filtro é "codEstacao"
 *     3. Executa o caminho certo (indexado ou sequencial)
 *     4. Imprime os registros encontrados (ou "Registro inexistente.")
 */
int buscar_registro_filtro_indexado(FILE* arquivo_binario,
                                    FILE* arquivo_indice,
                                    int   qtd_buscas) {

    /* Guarda 1: arquivos precisam ter aberto corretamente */
    if (arquivo_binario == NULL || arquivo_indice == NULL) {
        return FILE_NOT_FOUND_ERROR;
    }

    /*
     * Lê o cabeçalho do arquivo de DADOS.
     * Precisamos dele para saber quantos registros existem (proximo_rrn)
     * e para verificar se o arquivo está consistente (status == '1').
     */
    Cabecalho* cab = novo_cabecalho();
    if (cab == NULL) {
        return MALLOC_ERROR;
    }
    if (ler_cabecalho_binario(arquivo_binario, cab) < NUM_CAMPOS_CABECALHO) {
        free_cabecalho(&cab);
        return FILE_READ_ERROR;
    }
    if (cab->status == STATUS_INCONSISTENT) {
        free_cabecalho(&cab);
        return INCOSISTENT_FILE_ERROR;
    }

    /* ── Loop principal: repete para cada busca pedida ─────────────────── */
    for (int busca = 0; busca < qtd_buscas; busca++) {

        /* Lê quantos filtros esta busca tem (ex: "2 codEstacao 9 nomeLinha ...") */
        int qtd_filtros;
        scanf("%d ", &qtd_filtros);

        /*
         * Aloca um vetor de Filtro com espaço para qtd_filtros elementos.
         * Cada Filtro tem dois campos: campo (ex: "codEstacao") e valor (ex: "9").
         */
        Filtro* filtros = (Filtro*) malloc(sizeof(Filtro) * qtd_filtros);

        /*
         * tem_cod_estacao: flag que marca se algum filtro pede codEstacao.
         * valor_cod_estacao: o valor numérico que estamos buscando.
         */
        int tem_cod_estacao  = 0;
        int valor_cod_estacao = -1;

        /* Lê cada filtro e detecta se algum é por codEstacao */
        for (int i = 0; i < qtd_filtros; i++) {
            filtros[i].campo = calloc(100, sizeof(char));
            filtros[i].valor = calloc(100, sizeof(char));

            scanf("%s", filtros[i].campo);
            ScanQuoteString(filtros[i].valor);

            if (strcmp(filtros[i].campo, "codEstacao") == 0) {
                tem_cod_estacao   = 1;
                valor_cod_estacao = atoi(filtros[i].valor);
            }
        }

        int encontrou = 0;

        /* ── CAMINHO A: busca INDEXADA (tem filtro por codEstacao) ────────── */
        if (tem_cod_estacao) {
            /*
             * Carrega o índice inteiro para a memória (vetor de Indice).
             * Isso nos permite fazer busca binária sem tocar no disco a cada
             * comparação — muito mais rápido.
             */
            int qtd_indices = 0;
            Indice* indices = carregar_indice(arquivo_indice, &qtd_indices);

            if (indices != NULL) {
                /*
                 * Busca binária pelo codEstacao.
                 * Retorna a posição no vetor, ou -1 se não achou.
                 */
                int pos = busca_binaria_indice(indices, qtd_indices, valor_cod_estacao);

                if (pos != -1) {
                    /*
                     * Achou no índice! indices[pos].RRN nos diz exatamente
                     * em qual posição do arquivo de dados está o registro.
                     * Não precisa varrer nada — vai direto ao byte certo.
                     */
                    int rrn = indices[pos].RRN;
                    Registro* reg = ler_registro_RRN(arquivo_binario, rrn);

                    if (reg != NULL) {
                        /*
                         * Aplica TODOS os filtros no registro encontrado.
                         * Mesmo na busca indexada, pode haver filtros extras
                         * (ex: codEstacao=9 E nomeLinha="Azul").
                         * O registro só é exibido se passar em todos.
                         */
                        int correspondencias = 0;
                        for (int i = 0; i < qtd_filtros; i++) {
                            if (passou_no_filtro(reg, &filtros[i])) {
                                correspondencias++;
                            }
                        }
                        if (correspondencias == qtd_filtros) {
                            print_registro(reg);
                            encontrou = 1;
                        }
                        free_registro(&reg);
                    }
                }

                /*
                 * Libera o vetor de índices da memória.
                 * Sempre que usamos malloc/calloc, precisamos do free correspondente.
                 */
                free(indices);
            }

        /* ── CAMINHO B: busca SEQUENCIAL (outros campos) ─────────────────── */
        } else {
            /*
             * Sem filtro por codEstacao: percorre todos os registros do arquivo
             * de dados um por um, verificando cada um contra os filtros.
             * Isso é igual à funcionalidade [3] — reaproveitamos a mesma lógica.
             */
            for (int rrn = 0; rrn < cab->proximo_rrn; rrn++) {
                Registro* reg = ler_registro_RRN(arquivo_binario, rrn);
                if (reg == NULL) {
                    continue; /* NULL = removido ou erro, pula para o próximo */
                }

                int correspondencias = 0;
                for (int i = 0; i < qtd_filtros; i++) {
                    if (passou_no_filtro(reg, &filtros[i])) {
                        correspondencias++;
                    }
                }

                if (correspondencias == qtd_filtros) {
                    print_registro(reg);
                    encontrou = 1;
                }

                free_registro(&reg);
            }
        }

        /* Se não achou nenhum registro nessa busca, exibe a mensagem padrão */
        if (!encontrou) {
            printf("Registro inexistente.");
        }

        /*
         * Separa as buscas com uma linha em branco,
         * mas NÃO depois da última (comportamento exigido pelo runcodes).
         */
        if (busca < qtd_buscas - 1) {
            printf("\n");
        }

        free_filtro(&filtros, qtd_filtros);
    }

    free_cabecalho(&cab);
    return NO_ERROR;
}
