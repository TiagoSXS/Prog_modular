#if !defined( INGREDIENTE_MOD )
#define INGREDIENTE_MOD

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

typedef struct despensa Despensa;

Despensa* criarDespensa(void);
void destruirDespensa(Despensa* d);
tpCondRet adicionarIngrediente(Despensa* d, const char* nome, float qtd, const char* unidade);
tpCondRet removerIngrediente(Despensa* d, const char* nomeBusca);
tpCondRet editarIngrediente(Despensa* d, const char* nomeBusca, const char* novoNome, float novaQuantidade, const char* novaUnidade);
int verificarDisponibilidadeIngrediente(Despensa* d, const char* nome, float qtdMinima, const char* unidade);
int obterTotalIngredientes(Despensa* d);
tpCondRet obterIngredientePorIndice(Despensa* d, int indice, char* nomeOut, int tamNome, float* qtdOut, char* unidadeOut, int tamUnidade);
tpCondRet salvarDespensa(Despensa* d, const char* caminhoArquivo);
tpCondRet carregarDespensa(Despensa* d, const char* caminhoArquivo);

#endif  