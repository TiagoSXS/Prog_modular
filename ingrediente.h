/* Modulo Ingrediente: despensa do usuario (nome, quantidade e unidade de cada item).
 * 'Despensa' e um tipo opaco: a struct so existe em ingrediente.c. Use apenas as funcoes abaixo. */
#if !defined( INGREDIENTE_MOD )
#define INGREDIENTE_MOD

/* Codigos de retorno. Mesma definicao em receita.h (protegida por TP_COND_RET_DEF):
 * se alterar aqui, altere la tambem. */
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

typedef struct despensa Despensa;

/* Cria despensa vazia. Retorna NULL se faltar memoria. Liberar com destruirDespensa. */
Despensa* criarDespensa(void);
void destruirDespensa(Despensa* d);

/* Soma 'qtd' ao item de mesmo nome e unidade, ou cria um item novo.
 * nome e unidade: textos nao vazios (ex.: "g", "ml", "un") | qtd: > 0
 * Retorna PARAMETRO_INVALIDO (nulo, vazio, qtd <= 0 ou mesmo nome com OUTRA unidade)
 * ou ERRO_MEMORIA (despensa cheia). */
tpCondRet adicionarIngrediente(Despensa* d, const char* nome, float qtd, const char* unidade);

/* Remove o item cujo nome e igual a nomeBusca. Retorna NAO_ENCONTRADO se nao existir. */
tpCondRet removerIngrediente(Despensa* d, const char* nomeBusca);

/* Substitui nome, quantidade e unidade (os tres) do item cujo nome e nomeBusca.
 * novaQuantidade: >= 0 | novoNome e novaUnidade: nao vazios. Retorna NAO_ENCONTRADO se nomeBusca nao existir. */
tpCondRet editarIngrediente(Despensa* d, const char* nomeBusca, const char* novoNome, float novaQuantidade, const char* novaUnidade);

/* Retorna 1 se existe item com nome E unidade iguais e quantidade >= qtdMinima; senao 0.
 * Comparacao exata (diferencia maiuscula/minuscula, sem conversao de unidade). Usada pelo Buscador. */
int verificarDisponibilidadeIngrediente(Despensa* d, const char* nome, float qtdMinima, const char* unidade);

/* Numero de itens na despensa (0 se d for NULL). */
int obterTotalIngredientes(Despensa* d);

/* Copia o item de posicao 'indice' (comecando em 0) para os buffers do chamador.
 * nomeOut/unidadeOut: buffers de saida | tamNome/tamUnidade: tamanho desses buffers em bytes (> 1) | qtdOut: saida
 * Retorna NAO_ENCONTRADO se o indice for invalido. */
tpCondRet obterIngredientePorIndice(Despensa* d, int indice, char* nomeOut, int tamNome, float* qtdOut, char* unidadeOut, int tamUnidade);

/* Gravam/leem a despensa em arquivo binario (int com o total + array de itens).
 * Retornam ERRO_ARQUIVO se o arquivo nao abrir ou estiver invalido (ex.: ainda nao existe). */
tpCondRet salvarDespensa(Despensa* d, const char* caminhoArquivo);
tpCondRet carregarDespensa(Despensa* d, const char* caminhoArquivo);

#endif
