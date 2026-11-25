/* Modulo Receita: catalogo de receitas (titulo, modo de preparo e ingredientes exigidos).
 * 'CatalogoReceitas' e um tipo opaco: a struct so existe em receita.c. Use apenas as funcoes abaixo.
 * Receitas sao identificadas pelo titulo (unico) ou pela posicao (indice, comecando em 0). */
#if !defined( RECEITA_MOD )
#define RECEITA_MOD

/* Mesma definicao de ingrediente.h (protegida por TP_COND_RET_DEF): se alterar, altere nos dois. */
#if !defined( TP_COND_RET_DEF )
#define TP_COND_RET_DEF
typedef enum {
    COND_OK = 0,
    COND_NAO_ENCONTRADO = 1,       /* item, receita ou indice nao existe */
    COND_PARAMETRO_INVALIDO = 2,   /* ponteiro NULL, texto vazio ou valor fora do permitido */
    COND_ERRO_ARQUIVO = 3,         /* falha ao abrir, ler ou gravar arquivo */
    COND_ERRO_MEMORIA = 4          /* limite do array atingido (MAX_*) */
} tpCondRet;
#endif 

typedef struct catalogo_receitas CatalogoReceitas;

/* Cria catalogo vazio. Retorna NULL se faltar memoria. Liberar com destruirCatalogo. */
CatalogoReceitas* criarCatalogo(void);
void destruirCatalogo(CatalogoReceitas* cat);

/* Cadastra receita SEM ingredientes (use adicionarIngredienteNaReceita em seguida).
 * titulo e modoPreparo: textos nao vazios | titulo deve ser unico.
 * Retorna PARAMETRO_INVALIDO (nulo, vazio ou titulo repetido) ou ERRO_MEMORIA (catalogo cheio). */
tpCondRet adicionarReceita(CatalogoReceitas* cat, const char* titulo, const char* modoPreparo);

/* Acrescenta um ingrediente exigido pela receita de titulo 'tituloReceita'.
 * nomeIng e unidade: nao vazios | qtdNecessaria: > 0
 * Retorna NAO_ENCONTRADO (receita nao existe) ou ERRO_MEMORIA (receita ja tem o maximo de ingredientes). */
tpCondRet adicionarIngredienteNaReceita(CatalogoReceitas* cat, const char* tituloReceita, const char* nomeIng, float qtdNecessaria, const char* unidade);

/* Remove a receita de titulo 'titulo'. Retorna NAO_ENCONTRADO se nao existir. */
tpCondRet removerReceita(CatalogoReceitas* cat, const char* titulo);

/* Numero de receitas no catalogo (0 se cat for NULL). */
int obterTotalReceitas(CatalogoReceitas* cat);

/* Retornam ponteiro para texto INTERNO do catalogo (nao liberar nem alterar; so vale ate o catalogo mudar).
 * indiceReceita: posicao da receita, comecando em 0. Retornam NULL se o indice for invalido. */
const char* obterTituloReceita(CatalogoReceitas* cat, int indiceReceita);
const char* obterModoPreparoReceita(CatalogoReceitas* cat, int indiceReceita);

/* Numero de ingredientes exigidos pela receita (0 se o indice for invalido). */
int obterQtdIngredientesReceita(CatalogoReceitas* cat, int indiceReceita);

/* Copia o ingrediente 'indiceIng' da receita 'indiceReceita' (ambos comecando em 0) para os buffers do chamador.
 * nomeOut/unidadeOut: buffers de saida | tamNome/tamUnidade: tamanho desses buffers em bytes (> 1) | qtdOut: saida
 * Retorna NAO_ENCONTRADO se algum indice for invalido. */
tpCondRet obterIngredienteDaReceita(CatalogoReceitas* cat, int indiceReceita, int indiceIng, char* nomeOut, int tamNome, float* qtdOut, char* unidadeOut, int tamUnidade);

/* Gravam/leem o catalogo em arquivo binario (int com o total + array de receitas).
 * Retornam ERRO_ARQUIVO se o arquivo nao abrir ou estiver invalido (ex.: ainda nao existe). */
tpCondRet salvarCatalogo(CatalogoReceitas* cat, const char* caminhoArquivo);
tpCondRet carregarCatalogo(CatalogoReceitas* cat, const char* caminhoArquivo);

#endif
