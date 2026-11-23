#include <stdlib.h>
#include <string.h>
#include "buscador.h"

#define MAX_RESULTADOS 50
#define MAX_TITULO_RES 60
#define MAX_PREPARO_RES 500
#define MAX_NOME_BUSCA 50
#define MAX_UNIDADE_BUSCA 15

typedef struct {
    char titulo[MAX_TITULO_RES];
    char modoPreparo[MAX_PREPARO_RES];
} ItemResultado;

struct lista_receitas {
    ItemResultado itens[MAX_RESULTADOS];
    int total;
};

static int receitaEhViavel(Despensa* d, CatalogoReceitas* cat, int indiceReceita) {
    int qtdIngredientes = obterQtdIngredientesReceita(cat, indiceReceita);
    if (qtdIngredientes <= 0) {
        return 0;
    }

    char nomeIng[MAX_NOME_BUSCA];
    char unidadeIng[MAX_UNIDADE_BUSCA];
    float qtdNecessaria = 0.0f;

    for (int j = 0; j < qtdIngredientes; j++) {
        tpCondRet ret = obterIngredienteDaReceita(cat, indiceReceita, j, nomeIng, MAX_NOME_BUSCA, &qtdNecessaria, unidadeIng, MAX_UNIDADE_BUSCA);
        if (ret != COND_OK) {
            return 0;
        }
        if (verificarDisponibilidadeIngrediente(d, nomeIng, qtdNecessaria, unidadeIng) == 0) {
            return 0;
        }
    }

    return 1;
}

ListaReceitas* buscarReceitasCompativeis(Despensa* d, CatalogoReceitas* cat, int* totalEncontradas) {
    ListaReceitas* lista = (ListaReceitas*) malloc(sizeof(ListaReceitas));
    if (lista == NULL) {
        if (totalEncontradas != NULL) {
            *totalEncontradas = 0;
        }
        return NULL;
    }
    lista->total = 0;

    if (d == NULL || cat == NULL) {
        if (totalEncontradas != NULL) {
            *totalEncontradas = 0;
        }
        return lista;
    }

    int totalCatalogo = obterTotalReceitas(cat);
    for (int i = 0; i < totalCatalogo && lista->total < MAX_RESULTADOS; i++) {
        if (receitaEhViavel(d, cat, i) == 1) {
            const char* titulo = obterTituloReceita(cat, i);
            const char* preparo = obterModoPreparoReceita(cat, i);

            if (titulo != NULL && preparo != NULL) {
                strncpy(lista->itens[lista->total].titulo, titulo, MAX_TITULO_RES - 1);
                lista->itens[lista->total].titulo[MAX_TITULO_RES - 1] = '\0';

                strncpy(lista->itens[lista->total].modoPreparo, preparo, MAX_PREPARO_RES - 1);
                lista->itens[lista->total].modoPreparo[MAX_PREPARO_RES - 1] = '\0';

                lista->total++;
            }
        }
    }

    if (totalEncontradas != NULL) {
        *totalEncontradas = lista->total;
    }

    return lista;
}

int obterTamanhoListaReceitas(ListaReceitas* lista) {
    if (lista == NULL) {
        return 0;
    }
    return lista->total;
}

const char* obterTituloListaReceitas(ListaReceitas* lista, int indice) {
    if (lista == NULL || indice < 0 || indice >= lista->total) {
        return NULL;
    }
    return lista->itens[indice].titulo;
}

const char* obterModoPreparoListaReceitas(ListaReceitas* lista, int indice) {
    if (lista == NULL || indice < 0 || indice >= lista->total) {
        return NULL;
    }
    return lista->itens[indice].modoPreparo;
}

void destruirListaReceitas(ListaReceitas* lista) {
    if (lista != NULL) {
        free(lista);
    }
}