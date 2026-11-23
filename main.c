#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingrediente.h"
#include "receita.h"
#include "buscador.h"

#define ARQ_DESPENSA "despensa.bin"
#define ARQ_RECEITAS "receitas.bin"

static void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void lerTexto(char* destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        }
    }
}

static void listarTodosIngredientes(Despensa* despensa) {
    int total = obterTotalIngredientes(despensa);
    if (total == 0) {
        printf("A despensa esta vazia.\n");
        return;
    }

    char nome[50];
    char unidade[15];
    float qtd = 0.0f;

    printf("\n--- INGREDIENTES NA DESPENSA (%d) ---\n", total);
    for (int i = 0; i < total; i++) {
        if (obterIngredientePorIndice(despensa, i, nome, sizeof(nome), &qtd, unidade, sizeof(unidade)) == COND_OK) {
            printf("%d. %s: %.2f %s\n", i + 1, nome, qtd, unidade);
        }
    }
}

static void listarTodasReceitas(CatalogoReceitas* catalogo) {
    int total = obterTotalReceitas(catalogo);
    if (total == 0) {
        printf("Nenhuma receita cadastrada no catalogo.\n");
        return;
    }

    char nomeIng[50];
    char unidadeIng[15];
    float qtdIng = 0.0f;

    printf("\n--- CATALOGO DE RECEITAS (%d) ---\n", total);
    for (int i = 0; i < total; i++) {
        printf("\n%d. %s\n", i + 1, obterTituloReceita(catalogo, i));
        printf("   Modo de preparo: %s\n", obterModoPreparoReceita(catalogo, i));
        printf("   Ingredientes:\n");

        int qtdReq = obterQtdIngredientesReceita(catalogo, i);
        for (int j = 0; j < qtdReq; j++) {
            if (obterIngredienteDaReceita(catalogo, i, j, nomeIng, sizeof(nomeIng), &qtdIng, unidadeIng, sizeof(unidadeIng)) == COND_OK) {
                printf("   - %s: %.2f %s\n", nomeIng, qtdIng, unidadeIng);
            }
        }
    }
}

int main(void) {
    Despensa* despensa = criarDespensa();
    CatalogoReceitas* catalogo = criarCatalogo();

    carregarDespensa(despensa, ARQ_DESPENSA);
    carregarCatalogo(catalogo, ARQ_RECEITAS);

    int opcao = -1;
    char nomeBusca[50], novoNome[50], unidade[15];
    char titulo[60], preparo[500];
    float quantidade = 0.0f;

    while (opcao != 0) {
        printf("\n=== SISTEMA DE DESPENSA E RECEITAS ===\n");
        printf("1. Adicionar Ingrediente\n");
        printf("2. Editar Ingrediente\n");
        printf("3. Remover Ingrediente\n");
        printf("4. Cadastrar Receita\n");
        printf("5. Buscar Receitas Compativeis\n");
        printf("6. Listar Receitas Cadastradas\n");
        printf("7. Listar Ingredientes da Despensa\n");
        printf("0. Salvar e Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 1) {
            printf("Nome do ingrediente: ");
            lerTexto(novoNome, sizeof(novoNome));
            printf("Quantidade: ");
            if (scanf("%f", &quantidade) != 1) {
                limparBuffer();
                printf("Quantidade invalida.\n");
                continue;
            }
            limparBuffer();
            printf("Unidade (ex: g, kg, ml, L, un): ");
            lerTexto(unidade, sizeof(unidade));

            if (adicionarIngrediente(despensa, novoNome, quantidade, unidade) == COND_OK) {
                printf("Ingrediente adicionado/acumulado com sucesso!\n");
            } else {
                printf("Erro: dados invalidos ou ingrediente ja existe com outra unidade.\n");
            }
        } else if (opcao == 2) {
            printf("Nome do ingrediente que deseja editar: ");
            lerTexto(nomeBusca, sizeof(nomeBusca));
            printf("Novo nome: ");
            lerTexto(novoNome, sizeof(novoNome));
            printf("Nova quantidade: ");
            if (scanf("%f", &quantidade) != 1) {
                limparBuffer();
                printf("Quantidade invalida.\n");
                continue;
            }
            limparBuffer();
            printf("Nova unidade (ex: g, kg, ml, L, un): ");
            lerTexto(unidade, sizeof(unidade));

            tpCondRet ret = editarIngrediente(despensa, nomeBusca, novoNome, quantidade, unidade);
            if (ret == COND_OK) {
                printf("Ingrediente atualizado com sucesso!\n");
            } else if (ret == COND_NAO_ENCONTRADO) {
                printf("Erro: Ingrediente nao encontrado na despensa.\n");
            } else {
                printf("Erro: Dados invalidos para edicao.\n");
            }
        } else if (opcao == 3) {
            printf("Nome do ingrediente para remover: ");
            lerTexto(nomeBusca, sizeof(nomeBusca));

            if (removerIngrediente(despensa, nomeBusca) == COND_OK) {
                printf("Ingrediente removido!\n");
            } else {
                printf("Erro: Ingrediente nao encontrado.\n");
            }
        } else if (opcao == 4) {
            int qtdIng = 0;
            printf("Titulo da receita: ");
            lerTexto(titulo, sizeof(titulo));
            printf("Modo de preparo: ");
            lerTexto(preparo, sizeof(preparo));

            if (adicionarReceita(catalogo, titulo, preparo) != COND_OK) {
                printf("Erro ao cadastrar receita.\n");
                continue;
            }

            printf("Quantos ingredientes esta receita usa? ");
            if (scanf("%d", &qtdIng) != 1 || qtdIng <= 0) {
                limparBuffer();
                printf("Numero invalido.\n");
                continue;
            }
            limparBuffer();

            for (int i = 0; i < qtdIng; i++) {
                printf("Ingrediente %d - Nome: ", i + 1);
                lerTexto(novoNome, sizeof(novoNome));
                printf("Ingrediente %d - Quantidade: ", i + 1);
                if (scanf("%f", &quantidade) != 1) {
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("Ingrediente %d - Unidade (ex: g, kg, ml, L, un): ", i + 1);
                lerTexto(unidade, sizeof(unidade));

                adicionarIngredienteNaReceita(catalogo, titulo, novoNome, quantidade, unidade);
            }
            printf("Receita cadastrada com sucesso!\n");
        } else if (opcao == 5) {
            int totalEncontradas = 0;
            ListaReceitas* sugeridas = buscarReceitasCompativeis(despensa, catalogo, &totalEncontradas);

            if (sugeridas == NULL || totalEncontradas == 0) {
                printf("Nenhuma receita compativel com os ingredientes atuais.\n");
            } else {
                printf("Foram encontradas %d receitas possiveis:\n", totalEncontradas);
                for (int i = 0; i < obterTamanhoListaReceitas(sugeridas); i++) {
                    printf("- %s: %s\n", obterTituloListaReceitas(sugeridas, i), obterModoPreparoListaReceitas(sugeridas, i));
                }
            }
            destruirListaReceitas(sugeridas);
        } else if (opcao == 6) {
            listarTodasReceitas(catalogo);
        } else if (opcao == 7) {
            listarTodosIngredientes(despensa);
        } else if (opcao == 0) {
            salvarDespensa(despensa, ARQ_DESPENSA);
            salvarCatalogo(catalogo, ARQ_RECEITAS);
            printf("Dados salvos nos arquivos com sucesso. Encerrando...\n");
        }
    }

    destruirDespensa(despensa);
    destruirCatalogo(catalogo);
    return 0;
}