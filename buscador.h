/* Modulo Buscador: cruza a despensa com o catalogo e devolve as receitas que podem ser feitas.
 * Nao guarda dados: so le (via interfaces) os modulos Ingrediente e Receita.
 * 'ListaReceitas' e um tipo opaco (struct em buscador.c): use apenas as funcoes abaixo. */
#if !defined( BUSCADOR_MOD )
#define BUSCADOR_MOD

#include "ingrediente.h" 
#include "receita.h"

typedef struct lista_receitas ListaReceitas;

/* Devolve a lista de receitas COMPATIVEIS: aquelas em que TODOS os ingredientes existem na despensa
 * (mesmo nome e mesma unidade) em quantidade suficiente, conforme verificarDisponibilidadeIngrediente.
 * d: despensa | cat: catalogo de receitas | totalEncontradas: saida com o tamanho da lista (pode ser NULL)
 * Retorno: lista NOVA, que o chamador deve liberar com destruirListaReceitas. Vazia se nada for compativel
 * ou se d/cat forem NULL. Retorna NULL somente se faltar memoria. */
ListaReceitas* buscarReceitasCompativeis(Despensa* d, CatalogoReceitas* cat, int* totalEncontradas);

/* Numero de receitas na lista (0 se lista for NULL). */
int obterTamanhoListaReceitas(ListaReceitas* lista);

/* Titulo / modo de preparo da receita na posicao 'indice' (comecando em 0).
 * Ponteiro para texto interno da lista (nao liberar). NULL se o indice for invalido. */
const char* obterTituloListaReceitas(ListaReceitas* lista, int indice);
const char* obterModoPreparoListaReceitas(ListaReceitas* lista, int indice);

/* Libera a lista devolvida por buscarReceitasCompativeis (aceita NULL). */
void destruirListaReceitas(ListaReceitas* lista);

#endif
