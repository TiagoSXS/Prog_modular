/* Testes unitarios do modulo Receita (usa o receita.c real).
 * Compilar: gcc -Wall -o teste_receita teste_receita.c receita.c
 * O teste 3 cria o arquivo teste_receitas.bin na pasta atual. */
#include <stdio.h>
#include <string.h>
#include "receita.h"

static int testar_AdicionarIngrediente_ReceitaInexistente(void) {
    CatalogoReceitas* cat = criarCatalogo();
    tpCondRet ret = adicionarIngredienteNaReceita(cat, "Bolo", "Farinha", 200.0f, "g");
    destruirCatalogo(cat);
    return (ret == COND_NAO_ENCONTRADO);
}

static int testar_RemoverReceita_Inexistente(void) {
    CatalogoReceitas* cat = criarCatalogo();
    tpCondRet ret = removerReceita(cat, "Sopa");
    destruirCatalogo(cat);
    return (ret == COND_NAO_ENCONTRADO);
}

/* Salva um catalogo em arquivo, carrega em outro e confere titulo e ingrediente da receita */
static int testar_Persistencia_Catalogo(void) { 
    CatalogoReceitas* cat1 = criarCatalogo();
    adicionarReceita(cat1, "Omelete", "Bater ovos e fritar");
    adicionarIngredienteNaReceita(cat1, "Omelete", "Ovo", 2.0f, "un");
    salvarCatalogo(cat1, "teste_receitas.bin");
    destruirCatalogo(cat1);

    CatalogoReceitas* cat2 = criarCatalogo();
    tpCondRet retCarga = carregarCatalogo(cat2, "teste_receitas.bin");

    char nomeIng[50];
    char unidadeIng[15];
    float qtd = 0.0f;
    tpCondRet retIng = obterIngredienteDaReceita(cat2, 0, 0, nomeIng, sizeof(nomeIng), &qtd, unidadeIng, sizeof(unidadeIng));

    int passou = (retCarga == COND_OK &&
                  obterTotalReceitas(cat2) == 1 &&
                  strcmp(obterTituloReceita(cat2, 0), "Omelete") == 0 &&
                  retIng == COND_OK &&
                  strcmp(nomeIng, "Ovo") == 0 &&
                  strcmp(unidadeIng, "un") == 0 &&
                  qtd == 2.0f);

    destruirCatalogo(cat2);
    return passou;
}

/* So imprime o resultado: o programa retorna 0 mesmo se algum teste falhar */
int main(void) {
    printf("=== TESTES UNITARIOS: MODULO RECEITA ===\n");
    printf("1. Ingrediente em Receita Inexistente: %s\n", testar_AdicionarIngrediente_ReceitaInexistente() ? "PASSOU" : "FALHOU");
    printf("2. Remover Receita Inexistente:        %s\n", testar_RemoverReceita_Inexistente() ? "PASSOU" : "FALHOU");
    printf("3. Persistencia de Catalogo (.bin):    %s\n", testar_Persistencia_Catalogo() ? "PASSOU" : "FALHOU");
    return 0;
}
