#if !defined( RECEITA_MOD )
#define RECEITA_MOD

#if !defined( TP_COND_RET_DEF )
#define TP_COND_RET_DEF
typedef enum {
    COND_OK = 0,
    COND_NAO_ENCONTRADO = 1,
    COND_PARAMETRO_INVALIDO = 2,
    COND_ERRO_ARQUIVO = 3,
    COND_ERRO_MEMORIA = 4
} tpCondRet;
#endif

typedef struct catalogo_receitas CatalogoReceitas;

CatalogoReceitas* criarCatalogo(void);
void destruirCatalogo(CatalogoReceitas* cat);
tpCondRet adicionarReceita(CatalogoReceitas* cat, const char* titulo, const char* modoPreparo);
tpCondRet adicionarIngredienteNaReceita(CatalogoReceitas* cat, const char* tituloReceita, const char* nomeIng, float qtdNecessaria, const char* unidade);
tpCondRet removerReceita(CatalogoReceitas* cat, const char* titulo);
int obterTotalReceitas(CatalogoReceitas* cat);
const char* obterTituloReceita(CatalogoReceitas* cat, int indiceReceita);
const char* obterModoPreparoReceita(CatalogoReceitas* cat, int indiceReceita);
int obterQtdIngredientesReceita(CatalogoReceitas* cat, int indiceReceita);
tpCondRet obterIngredienteDaReceita(CatalogoReceitas* cat, int indiceReceita, int indiceIng, char* nomeOut, int tamNome, float* qtdOut, char* unidadeOut, int tamUnidade);
tpCondRet salvarCatalogo(CatalogoReceitas* cat, const char* caminhoArquivo);
tpCondRet carregarCatalogo(CatalogoReceitas* cat, const char* caminhoArquivo);

#endif