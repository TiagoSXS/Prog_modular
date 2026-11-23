#include <stdio.h>
#include "ingrediente.h"

static int testar_EditarIngrediente_Inexistente(void) {
    Despensa* d = criarDespensa();
    adicionarIngrediente(d, "Arroz", 1000.0f, "g");
    tpCondRet ret = editarIngrediente(d, "Feijao", "Feijao Preto", 500.0f, "g");
    destruirDespensa(d);
    return (ret == COND_NAO_ENCONTRADO);
}

static int testar_RemoverIngrediente_Inexistente(void) {
    Despensa* d = criarDespensa();
    adicionarIngrediente(d, "Sal", 500.0f, "g");
    tpCondRet ret = removerIngrediente(d, "Acucar");
    destruirDespensa(d);
    return (ret == COND_NAO_ENCONTRADO);
}

static int testar_Editar_E_Persistencia(void) {
    Despensa* d1 = criarDespensa();
    adicionarIngrediente(d1, "Leite", 1000.0f, "ml");
    tpCondRet retEdit = editarIngrediente(d1, "Leite", "Leite Integral", 2.0f, "L");
    tpCondRet retSave = salvarDespensa(d1, "teste_despensa.bin");
    destruirDespensa(d1);

    Despensa* d2 = criarDespensa();
    tpCondRet retLoad = carregarDespensa(d2, "teste_despensa.bin");
    int achouEditado = verificarDisponibilidadeIngrediente(d2, "Leite Integral", 2.0f, "L");
    destruirDespensa(d2);

    return (retEdit == COND_OK && retSave == COND_OK && retLoad == COND_OK && achouEditado == 1);
}

static int testar_UnidadeDivergente_Recusada(void) {
    Despensa* d = criarDespensa();
    adicionarIngrediente(d, "Farinha", 1.0f, "kg");
    int disponivel = verificarDisponibilidadeIngrediente(d, "Farinha", 500.0f, "g");
    destruirDespensa(d);
    return (disponivel == 0);
}

static int testar_AdicionarIngrediente_Acumulativo(void) {
    Despensa* d = criarDespensa();
    adicionarIngrediente(d, "Ovo", 2.0f, "un");
    adicionarIngrediente(d, "Ovo", 1.0f, "un");

    int totalItens = obterTotalIngredientes(d);
    int temTres = verificarDisponibilidadeIngrediente(d, "Ovo", 3.0f, "un");
    tpCondRet retUnidadeDiferente = adicionarIngrediente(d, "Ovo", 100.0f, "g");

    destruirDespensa(d);
    return (totalItens == 1 && temTres == 1 && retUnidadeDiferente == COND_PARAMETRO_INVALIDO);
}

int main(void) {
    printf("=== TESTES UNITARIOS: MODULO INGREDIENTE ===\n");
    printf("1. Editar Inexistente:        %s\n", testar_EditarIngrediente_Inexistente() ? "PASSOU" : "FALHOU");
    printf("2. Remover Inexistente:       %s\n", testar_RemoverIngrediente_Inexistente() ? "PASSOU" : "FALHOU");
    printf("3. Editar 3 Params + Arquivo: %s\n", testar_Editar_E_Persistencia() ? "PASSOU" : "FALHOU");
    printf("4. Unidade Divergente (kg/g): %s\n", testar_UnidadeDivergente_Recusada() ? "PASSOU" : "FALHOU");
    printf("5. Acumulacao (2 + 1 = 3):    %s\n", testar_AdicionarIngrediente_Acumulativo() ? "PASSOU" : "FALHOU");
    return 0;
}