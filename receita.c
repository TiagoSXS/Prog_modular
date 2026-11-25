#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receita.h"

/* ATENCAO: alterar MAX_* ou os campos das structs torna incompativeis os .bin ja salvos
 * (salvarCatalogo grava a struct crua). MAX_NOME_ING e MAX_UNIDADE_ING devem ser iguais aos
 * de ingrediente.c e aos buffers de buscador.c e main.c (senao nomes sao truncados). */
#define MAX_TITULO 60
#define MAX_PREPARO 500 
#define MAX_NOME_ING 50
#define MAX_UNIDADE_ING 15
#define MAX_INGREDIENTES_REC 15
#define MAX_RECEITAS 50

/* Um ingrediente exigido por uma receita */
typedef struct {
    char nome[MAX_NOME_ING];
    float quantidadeNecessaria;
    char unidade[MAX_UNIDADE_ING];
} RequisitoIngrediente;

typedef struct {
    char titulo[MAX_TITULO];
    char modoPreparo[MAX_PREPARO];
    RequisitoIngrediente requisitos[MAX_INGREDIENTES_REC];
    int qtdRequisitos;
} Receita;

struct catalogo_receitas {
    Receita lista[MAX_RECEITAS];
    int totalReceitas;
};

/* Retorna a posicao da receita de titulo igual (comparacao exata) ou -1 se nao existir */
static int buscarIndiceReceita(CatalogoReceitas* cat, const char* titulo) {
    for (int i = 0; i < cat->totalReceitas; i++) {
        if (strcmp(cat->lista[i].titulo, titulo) == 0) {
            return i;
        }
    }
    return -1;
}

CatalogoReceitas* criarCatalogo(void) {
    CatalogoReceitas* cat = (CatalogoReceitas*) malloc(sizeof(CatalogoReceitas));
    if (cat != NULL) {
        cat->totalReceitas = 0;
    }
    return cat;
}

void destruirCatalogo(CatalogoReceitas* cat) {
    if (cat != NULL) {
        free(cat);
    }
}

tpCondRet adicionarReceita(CatalogoReceitas* cat, const char* titulo, const char* modoPreparo) {
    if (cat == NULL || titulo == NULL || modoPreparo == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (strlen(titulo) == 0 || strlen(modoPreparo) == 0) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (cat->totalReceitas >= MAX_RECEITAS) {
        return COND_ERRO_MEMORIA;
    }
    if (buscarIndiceReceita(cat, titulo) != -1) {
        return COND_PARAMETRO_INVALIDO;
    }

    int idx = cat->totalReceitas;
    strncpy(cat->lista[idx].titulo, titulo, MAX_TITULO - 1);
    cat->lista[idx].titulo[MAX_TITULO - 1] = '\0';

    strncpy(cat->lista[idx].modoPreparo, modoPreparo, MAX_PREPARO - 1);
    cat->lista[idx].modoPreparo[MAX_PREPARO - 1] = '\0';

    cat->lista[idx].qtdRequisitos = 0;
    cat->totalReceitas++;

    return COND_OK;
}

tpCondRet adicionarIngredienteNaReceita(CatalogoReceitas* cat, const char* tituloReceita, const char* nomeIng, float qtdNecessaria, const char* unidade) {
    if (cat == NULL || tituloReceita == NULL || nomeIng == NULL || unidade == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (qtdNecessaria <= 0.0f || strlen(nomeIng) == 0 || strlen(unidade) == 0) {
        return COND_PARAMETRO_INVALIDO;
    }

    int idxRec = buscarIndiceReceita(cat, tituloReceita);
    if (idxRec == -1) {
        return COND_NAO_ENCONTRADO;
    }

    Receita* rec = &(cat->lista[idxRec]);
    if (rec->qtdRequisitos >= MAX_INGREDIENTES_REC) {
        return COND_ERRO_MEMORIA;
    }

    int idxIng = rec->qtdRequisitos;
    strncpy(rec->requisitos[idxIng].nome, nomeIng, MAX_NOME_ING - 1);
    rec->requisitos[idxIng].nome[MAX_NOME_ING - 1] = '\0';

    rec->requisitos[idxIng].quantidadeNecessaria = qtdNecessaria;

    strncpy(rec->requisitos[idxIng].unidade, unidade, MAX_UNIDADE_ING - 1);
    rec->requisitos[idxIng].unidade[MAX_UNIDADE_ING - 1] = '\0';

    rec->qtdRequisitos++;
    return COND_OK;
}

tpCondRet removerReceita(CatalogoReceitas* cat, const char* titulo) {
    if (cat == NULL || titulo == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }

    int idx = buscarIndiceReceita(cat, titulo);
    if (idx == -1) {
        return COND_NAO_ENCONTRADO;
    }

    /* Desloca as receitas seguintes uma posicao para fechar o buraco */
    for (int i = idx; i < cat->totalReceitas - 1; i++) {
        cat->lista[i] = cat->lista[i + 1];
    }
    cat->totalReceitas--;
    return COND_OK;
}

int obterTotalReceitas(CatalogoReceitas* cat) {
    if (cat == NULL) {
        return 0;
    }
    return cat->totalReceitas;
}

const char* obterTituloReceita(CatalogoReceitas* cat, int indiceReceita) {
    if (cat == NULL || indiceReceita < 0 || indiceReceita >= cat->totalReceitas) {
        return NULL;
    }
    return cat->lista[indiceReceita].titulo;
}

const char* obterModoPreparoReceita(CatalogoReceitas* cat, int indiceReceita) {
    if (cat == NULL || indiceReceita < 0 || indiceReceita >= cat->totalReceitas) {
        return NULL;
    }
    return cat->lista[indiceReceita].modoPreparo;
}

int obterQtdIngredientesReceita(CatalogoReceitas* cat, int indiceReceita) {
    if (cat == NULL || indiceReceita < 0 || indiceReceita >= cat->totalReceitas) {
        return 0;
    }
    return cat->lista[indiceReceita].qtdRequisitos;
}

tpCondRet obterIngredienteDaReceita(CatalogoReceitas* cat, int indiceReceita, int indiceIng, char* nomeOut, int tamNome, float* qtdOut, char* unidadeOut, int tamUnidade) {
    if (cat == NULL || nomeOut == NULL || qtdOut == NULL || unidadeOut == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (tamNome <= 1 || tamUnidade <= 1) {
        return COND_PARAMETRO_INVALIDO;
    }
    if (indiceReceita < 0 || indiceReceita >= cat->totalReceitas) {
        return COND_NAO_ENCONTRADO;
    }

    Receita* rec = &(cat->lista[indiceReceita]);
    if (indiceIng < 0 || indiceIng >= rec->qtdRequisitos) {
        return COND_NAO_ENCONTRADO;
    }

    strncpy(nomeOut, rec->requisitos[indiceIng].nome, (size_t)(tamNome - 1));
    nomeOut[tamNome - 1] = '\0';

    *qtdOut = rec->requisitos[indiceIng].quantidadeNecessaria;

    strncpy(unidadeOut, rec->requisitos[indiceIng].unidade, (size_t)(tamUnidade - 1));
    unidadeOut[tamUnidade - 1] = '\0';

    return COND_OK;
}

/* Formato do arquivo: [int totalReceitas][Receita x totalReceitas] */
tpCondRet salvarCatalogo(CatalogoReceitas* cat, const char* caminhoArquivo) {
    if (cat == NULL || caminhoArquivo == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }

    FILE* fp = fopen(caminhoArquivo, "wb");
    if (fp == NULL) {
        return COND_ERRO_ARQUIVO;
    }

    if (fwrite(&(cat->totalReceitas), sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return COND_ERRO_ARQUIVO;
    }

    if (cat->totalReceitas > 0) {
        if (fwrite(cat->lista, sizeof(Receita), (size_t)cat->totalReceitas, fp) != (size_t)cat->totalReceitas) {
            fclose(fp);
            return COND_ERRO_ARQUIVO;
        }
    }

    fclose(fp);
    return COND_OK;
}

tpCondRet carregarCatalogo(CatalogoReceitas* cat, const char* caminhoArquivo) {
    if (cat == NULL || caminhoArquivo == NULL) {
        return COND_PARAMETRO_INVALIDO;
    }

    FILE* fp = fopen(caminhoArquivo, "rb");
    if (fp == NULL) {
        return COND_ERRO_ARQUIVO;
    }

    /* Valida o total lido antes de usar, para nao estourar o array lista[] */
    int total = 0;
    if (fread(&total, sizeof(int), 1, fp) != 1 || total < 0 || total > MAX_RECEITAS) {
        fclose(fp);
        return COND_ERRO_ARQUIVO;
    }

    if (total > 0) {
        if (fread(cat->lista, sizeof(Receita), (size_t)total, fp) != (size_t)total) {
            fclose(fp);
            return COND_ERRO_ARQUIVO;
        }
    }

    cat->totalReceitas = total;
    fclose(fp);
    return COND_OK;
}
