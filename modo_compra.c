#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "modo_compra.h"
#include "produto.h"

float calcular_total_carrinho(produto *carrinho){
    float total = 0;
    produto *p = carrinho;
    while (p != NULL){
        total += p -> preco * p -> quantidade; //o ponteiro acessa preço e quantidade do produto e multiplica os dois
        p = p-> prox;
    }
    return total;
}

void limpar_carrinho(produto *carrinho){
    produto *atual = carrinho;
    while (atual != NULL){
        produto *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

void devolver_estoque(produto *carrinho, produto *estoque_oficial){
    produto *p = carrinho;
    while (p != NULL){
        produto *produto_estoque = buscar_produto(estoque_oficial, p->codigo);

        if(produto_estoque != NULL) produto_estoque -> quantidade += p->quantidade;
    }

    p = p->prox;
}

void iniciar_modo_compra(produto *estoque){

    produto *carrinho = NULL; //inicia carrinho como uma lista vazia

    int opcao = 0;
    int codigo_digitado;
    int qtd_desejada;

    do{
        printf("\n=== CAIXA LIVRE / MODO COMPRA ===\n");
        printf("1. Ver Estoque\n");
        printf("2. Colocar Produto no Carrinho\n");
        printf("3. Ver Meu Carrinho\n");
        printf("4. Finalizar Compra\n");
        printf("5. Cancelar e Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                printf("\n--- Produtos Disponíveis---\n");
                listar_produtos(estoque);
                break;

            case 2:
                printf("Digite o código do produto: ");
                scanf("%d", &codigo_digitado);

                produto *item_no_estoque = buscar_produto(estoque, codigo_digitado);

                if(item_no_estoque == NULL){
                    printf("Produto não encontrado");
                }
                else{
                    printf("Item: %s | Preco: R$ %.2f | Estoque: %d\n", item_no_estoque-> nome, item_no_estoque->preco, item_no_estoque->quantidade);
                    
                    printf("Quantas unidades você deseja? ");
                    scanf("%d", &qtd_desejada);

                    if(qtd_desejada > item_no_estoque->quantidade) printf("Erro: Quantidade maior que a disponível em estoque\n");
                    else if(qtd_desejada <= 0){
                        printf("Quantidade invalida\n");
                    }
                    else{
                        //o carrinho é uma lista de produtos, então podemos usar as mesmas funções de produto
                        carrinho = adicionar_produto(carrinho, item_no_estoque->codigo, item_no_estoque->nome, item_no_estoque->preco, qtd_desejada);

                        item_no_estoque->quantidade -= qtd_desejada; //dá baixa no estoque
                        printf("Produto adicionado ao carrinho\n");                    
                    }
                }
                break;

            case 3:
                printf("\n--- Carrinho de Compras ---\n");
                if (carrinho == NULL) {
                    printf("Seu carrinho esta vazio.\n");
                } else {
                    listar_produtos(carrinho);
                    printf("---------------------------\n");
                    printf("Subtotal: R$ %.2f\n", calcular_total_carrinho(carrinho));
                }
                break;

            case 4:
                if (carrinho == NULL) {
                    printf("Carrinho vazio. Nada para pagar.\n");
                } else {
                    float total = calcular_total_carrinho(carrinho);
                    printf("\n=== CUPOM FISCAL ===\n");
                    listar_produtos(carrinho);
                    printf("====================\n");
                    printf("TOTAL A PAGAR: R$ %.2f\n", total);
                    printf("Obrigado pela compra!\n");

                    limpar_carrinho(carrinho);
                    carrinho = NULL;
                    
                    return; 
                }
                break;

            case 5:
                if (carrinho != NULL){
                    printf("Devolvendo itens para as prateleiras\n");
                    devolver_estoque(carrinho, estoque);

                    limpar_carrinho(carrinho);
                    carrinho = NULL;
                }
                printf("Compra cancelada.\n");
                return;
        }


    }while (opcao != 5);

}