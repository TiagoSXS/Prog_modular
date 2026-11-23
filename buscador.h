#if !defined( BUSCADOR_MOD )
#define BUSCADOR_MOD

#include "ingrediente.h"
#include "receita.h"

typedef struct lista_receitas ListaReceitas;

ListaReceitas* buscarReceitasCompativeis(Despensa* d, CatalogoReceitas* cat, int* totalEncontradas);
int obterTamanhoListaReceitas(ListaReceitas* lista);
const char* obterTituloListaReceitas(ListaReceitas* lista, int indice);
const char* obterModoPreparoListaReceitas(ListaReceitas* lista, int indice);
void destruirListaReceitas(ListaReceitas* lista);

#endif