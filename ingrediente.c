#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingrediente.h"

/* ATENCAO: alterar MAX_* ou os campos de ItemIngrediente torna incompativeis os .bin ja salvos
 * (salvarDespensa grava a struct crua). Manter MAX_NOME e MAX_UNIDADE iguais aos buffers
 * de receita.c, buscador.c e main.c. */
#define MAX_NOME 50
#define MAX_UNIDADE 15
#define MAX_ITENS 100

typedef struct { 
    char nome[MAX_NOME];
    float quantidade;
    char unidade[MAX_UNIDADE];
} ItemIngrediente;

struct despensa {
    ItemIngrediente itens[MAX_ITENS];
    int totalItens;
};

Despensa* criarDespensa(void) {
    Despensa* d = (Despensa*) malloc(sizeof(Despensa));
    if (d != NULL) {
        d->totalItens = 0;
    }
    return d;
}

void destruirDespensa(Despensa* d) {
    if (d != NULL) {
        free(d);
    }
}

tpCondRet adicionarIngrediente(Despensa* d, const char* nome, float qtd, const char* unidade) {
    if (d == NULL || nome == NULL || unidade == NULL || qtd <= 0.0f) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (strlen(nome) == 0 || strlen(unidade) == 0) {
        return COND_PARAMETRO_INVALIDO;
    }

    /* Nome ja existe: soma se a unidade for a mesma, recusa se for outra */
    for (int i = 0; i < d->totalItens; i++) {
        if (strcmp(d->itens[i].nome, nome) == 0) {
            if (strcmp(d->itens[i].unidade, unidade) == 0) {
                d->itens[i].quantidade += qtd;
                return COND_OK;
            }
            return COND_PARAMETRO_INVALIDO;
        }
    }

    if (d->totalItens >= MAX_ITENS) {
        return COND_ERRO_MEMORIA;
    }

    /* strncpy + '\0' final: trunca textos maiores que o buffer sem perder o terminador */
    strncpy(d->itens[d->totalItens].nome, nome, MAX_NOME - 1);
    d->itens[d->totalItens].nome[MAX_NOME - 1] = '\0';
    d->itens[d->totalItens].quantidade = qtd;
    strncpy(d->itens[d->totalItens].unidade, unidade, MAX_UNIDADE - 1);
    d->itens[d->totalItens].unidade[MAX_UNIDADE - 1] = '\0';

    d->totalItens++;
    return COND_OK;
}

tpCondRet removerIngrediente(Despensa* d, const char* nomeBusca) {
    if (d == NULL || nomeBusca == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }

    for (int i = 0; i < d->totalItens; i++) {
        if (strcmp(d->itens[i].nome, nomeBusca) == 0) {
            /* Desloca os itens seguintes uma posicao para fechar o buraco */
            for (int j = i; j < d->totalItens - 1; j++) {
                d->itens[j] = d->itens[j + 1];
            }
            d->totalItens--;
            return COND_OK;
        }
    }
    return COND_NAO_ENCONTRADO;
}

tpCondRet editarIngrediente(Despensa* d, const char* nomeBusca, const char* novoNome, float novaQuantidade, const char* novaUnidade) {
    if (d == NULL || nomeBusca == NULL || novoNome == NULL || novaUnidade == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (novaQuantidade < 0.0f || strlen(novoNome) == 0 || strlen(novaUnidade) == 0) {
        return COND_PARAMETRO_INVALIDO;
    }

    for (int i = 0; i < d->totalItens; i++) {
        if (strcmp(d->itens[i].nome, nomeBusca) == 0) {
            strncpy(d->itens[i].nome, novoNome, MAX_NOME - 1);
            d->itens[i].nome[MAX_NOME - 1] = '\0';
            d->itens[i].quantidade = novaQuantidade;
            strncpy(d->itens[i].unidade, novaUnidade, MAX_UNIDADE - 1);
            d->itens[i].unidade[MAX_UNIDADE - 1] = '\0';
            return COND_OK;
        }
    }
    return COND_NAO_ENCONTRADO;
}

int verificarDisponibilidadeIngrediente(Despensa* d, const char* nome, float qtdMinima, const char* unidade) {
    if (d == NULL || nome == NULL || unidade == NULL) {
        return 0;
    }

    /* strcmp exato no nome e na unidade: "Ovo" != "ovo" e "kg" != "g" */
    for (int i = 0; i < d->totalItens; i++) {
        if (strcmp(d->itens[i].nome, nome) == 0 && strcmp(d->itens[i].unidade, unidade) == 0) {
            return (d->itens[i].quantidade >= qtdMinima) ? 1 : 0;
        }
    }
    return 0;
}

int obterTotalIngredientes(Despensa* d) {
    if (d == NULL) {
        return 0;
    }
    return d->totalItens;
}

tpCondRet obterIngredientePorIndice(Despensa* d, int indice, char* nomeOut, int tamNome, float* qtdOut, char* unidadeOut, int tamUnidade) {
    if (d == NULL || nomeOut == NULL || qtdOut == NULL || unidadeOut == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (tamNome <= 1 || tamUnidade <= 1) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (indice < 0 || indice >= d->totalItens) {
        return COND_NAO_ENCONTRADO;
    }

    strncpy(nomeOut, d->itens[indice].nome, (size_t)(tamNome - 1));
    nomeOut[tamNome - 1] = '\0';

    *qtdOut = d->itens[indice].quantidade;

    strncpy(unidadeOut, d->itens[indice].unidade, (size_t)(tamUnidade - 1));
    unidadeOut[tamUnidade - 1] = '\0';

    return COND_OK;
}

/* Formato do arquivo: [int totalItens][ItemIngrediente x totalItens] */
tpCondRet salvarDespensa(Despensa* d, const char* caminhoArquivo) {
    if (d == NULL || caminhoArquivo == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }

    FILE* fp = fopen(caminhoArquivo, "wb");
    if (fp == NULL) {
        return COND_ERRO_ARQUIVO;
    }

    if (fwrite(&(d->totalItens), sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return COND_ERRO_ARQUIVO;
    }

    if (d->totalItens > 0) {
        if (fwrite(d->itens, sizeof(ItemIngrediente), (size_t)d->totalItens, fp) != (size_t)d->totalItens) {
            fclose(fp);
            return COND_ERRO_ARQUIVO;
        }
    }

    fclose(fp);
    return COND_OK;
}

tpCondRet carregarDespensa(Despensa* d, const char* caminhoArquivo) {
    if (d == NULL || caminhoArquivo == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }

    FILE* fp = fopen(caminhoArquivo, "rb");
    if (fp == NULL) {
        return COND_ERRO_ARQUIVO;
    }

    /* Valida o total lido antes de usar, para nao estourar o array itens[] */
    int totalLido = 0;
    if (fread(&totalLido, sizeof(int), 1, fp) != 1 || totalLido > MAX_ITENS || totalLido < 0) {
        fclose(fp);
        return COND_ERRO_ARQUIVO;
    }

    if (totalLido > 0) {
        if (fread(d->itens, sizeof(ItemIngrediente), (size_t)totalLido, fp) != (size_t)totalLido) {
            fclose(fp);
            return COND_ERRO_ARQUIVO;
        }
    }

    d->totalItens = totalLido;
    fclose(fp);
    return COND_OK;
}
