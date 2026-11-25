/* Testes unitarios do Buscador, isolados por STUBS: as funcoes de Ingrediente e Receita abaixo sao
 * versoes FALSAS que substituem as reais. Compilar SO com buscador.c (nao incluir ingrediente.c nem
 * receita.c, senao da "multiple definition"):
 *   gcc -Wall -o teste_buscador teste_buscador.c buscador.c
 * Os ponteiros (Despensa*) 0x1 sao seguros: o Buscador nunca acessa a struct, so repassa aos stubs. */
#include <stdio.h>
#include <string.h>
#include "ingrediente.h"
#include "receita.h"
#include "buscador.h"

/* Resposta do stub de verificarDisponibilidadeIngrediente: 1 = tem o ingrediente, 0 = nao tem */
static int g_stubDisponivel = 0; 

int verificarDisponibilidadeIngrediente(Despensa* d, const char* nome, float qtdMinima, const char* unidade) {
    (void)d;
    (void)nome;
    (void)qtdMinima;
    (void)unidade;
    return g_stubDisponivel;
}

/* Catalogo falso: 1 receita ("Omelete") com 1 ingrediente (Ovo, 2 un) */
int obterTotalReceitas(CatalogoReceitas* cat) {
    (void)cat;
    return 1;
}

int obterQtdIngredientesReceita(CatalogoReceitas* cat, int indiceReceita) {
    (void)cat;
    (void)indiceReceita;
    return 1;
}

tpCondRet obterIngredienteDaReceita(CatalogoReceitas* cat, int indiceReceita, int indiceIng, char* nomeOut, int tamNome, float* qtdOut, char* unidadeOut, int tamUnidade) {
    (void)cat;
    (void)indiceReceita;
    (void)indiceIng;
    strncpy(nomeOut, "Ovo", (size_t)(tamNome - 1));
    nomeOut[tamNome - 1] = '\0';
    *qtdOut = 2.0f;
    strncpy(unidadeOut, "un", (size_t)(tamUnidade - 1));
    unidadeOut[tamUnidade - 1] = '\0';
    return COND_OK;
}

const char* obterTituloReceita(CatalogoReceitas* cat, int indiceReceita) {
    (void)cat;
    (void)indiceReceita;
    return "Omelete";
}

const char* obterModoPreparoReceita(CatalogoReceitas* cat, int indiceReceita) {
    (void)cat;
    (void)indiceReceita;
    return "Bater e fritar";
}

/* TU06: nenhum ingrediente disponivel -> lista vazia */
static int testar_TU06_ListaVazia(void) {
    Despensa* despensaSimulada = (Despensa*) 0x1;
    CatalogoReceitas* catalogoSimulado = (CatalogoReceitas*) 0x1;
    g_stubDisponivel = 0;

    int qtdEncontrada = -1;
    ListaReceitas* lista = buscarReceitasCompativeis(despensaSimulada, catalogoSimulado, &qtdEncontrada);

    int passou = (lista != NULL && qtdEncontrada == 0 && obterTamanhoListaReceitas(lista) == 0);
    destruirListaReceitas(lista);
    return passou;
}

/* TU07: ingrediente disponivel -> a receita "Omelete" e devolvida */
static int testar_TU07_ReceitaCompativel(void) {
    Despensa* despensaSimulada = (Despensa*) 0x1;
    CatalogoReceitas* catalogoSimulado = (CatalogoReceitas*) 0x1;
    g_stubDisponivel = 1;

    int qtdEncontrada = 0;
    ListaReceitas* lista = buscarReceitasCompativeis(despensaSimulada, catalogoSimulado, &qtdEncontrada);

    int passou = (lista != NULL &&
                  qtdEncontrada == 1 &&
                  obterTamanhoListaReceitas(lista) == 1 &&
                  strcmp(obterTituloListaReceitas(lista, 0), "Omelete") == 0);

    destruirListaReceitas(lista);
    return passou;
}

/* TU08: despensa e catalogo NULL -> lista vazia, sem travar */
static int testar_TU08_ParametrosNulos(void) {
    int qtdEncontrada = -1;
    ListaReceitas* lista = buscarReceitasCompativeis(NULL, NULL, &qtdEncontrada);

    int passou = (lista != NULL && qtdEncontrada == 0 && obterTamanhoListaReceitas(lista) == 0);
    destruirListaReceitas(lista);
    return passou;
}

/* So imprime o resultado: o programa retorna 0 mesmo se algum teste falhar */
int main(void) {
    printf("=== TESTES UNITARIOS ISOLADOS: MODULO BUSCADOR (STUBS) ===\n");
    printf("TU06 - Lista Vazia (Sem printf):  %s\n", testar_TU06_ListaVazia() ? "PASSOU" : "FALHOU");
    printf("TU07 - Receita Compativel (Stub): %s\n", testar_TU07_ReceitaCompativel() ? "PASSOU" : "FALHOU");
    printf("TU08 - Parametros NULL seguros:   %s\n", testar_TU08_ParametrosNulos() ? "PASSOU" : "FALHOU");
    return 0;
}
