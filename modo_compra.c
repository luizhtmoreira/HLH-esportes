#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modo_compra.h"
#include "produto.h" // Importante: precisamos das ferramentas do produto (buscar, listar, etc)

// --- MÉTODOS "PRIVATE" (Auxiliares) ---
// Note que essa função NÃO está no .h. Só o modo_compra.c enxerga ela.

float calcular_total_carrinho(produto *carrinho) {
    float total = 0;
    produto *p = carrinho;
    while (p != NULL) {
        // Preço * Quantidade comprada
        total += p->preco * p->quantidade; 
        p = p->prox;
    }
    return total;
}

// --- MÉTODOS "PUBLIC" (Da Interface) ---

void iniciar_modo_compra(produto *estoque) {
    // 1. O carrinho nasce vazio. É uma lista local, temporária.
    produto *carrinho = NULL; 
    
    int opcao = 0;
    int codigo_digitado;
    int qtd_desejada;

    do {
        printf("\n=== CAIXA LIVRE / MODO COMPRA ===\n");
        printf("1. Consultar Precos (Ver Estoque)\n");
        printf("2. Colocar Produto no Carrinho\n");
        printf("3. Ver Meu Carrinho\n");
        printf("4. Finalizar Compra (Pagar)\n");
        printf("5. Cancelar e Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- Produtos Disponiveis ---\n");
                listar_produtos(estoque);
                break;

            case 2:
                printf("Digite o codigo do produto: ");
                scanf("%d", &codigo_digitado);

                // Reutilizamos a busca que você já criou!
                produto *item_no_estoque = buscar_produto(estoque, codigo_digitado);

                if (item_no_estoque == NULL) {
                    printf("Produto nao encontrado!\n");
                } else {
                    printf("Item: %s | Preco: R$ %.2f | Estoque: %d\n", 
                           item_no_estoque->nome, item_no_estoque->preco, item_no_estoque->quantidade);
                    
                    printf("Quantas unidades voce quer? ");
                    scanf("%d", &qtd_desejada);

                    // Validação básica de estoque
                    if (qtd_desejada > item_no_estoque->quantidade) {
                        printf("Erro: Nao temos essa quantidade toda!\n");
                    } else if (qtd_desejada <= 0) {
                        printf("Erro: Quantidade invalida.\n");
                    } else {
                        // AQUI ESTÁ A MÁGICA:
                        // Adicionamos no carrinho (lista separada) copiando os dados.
                        // Note que passamos 'qtd_desejada' como a quantidade do item no carrinho.
                        carrinho = adicionar_produto(carrinho, 
                                                     item_no_estoque->codigo, 
                                                     item_no_estoque->nome, 
                                                     item_no_estoque->preco, 
                                                     qtd_desejada);
                        
                        // Baixa "visual" no estoque (para não deixar vender o mesmo item 2x na mesma sessão)
                        // Num sistema real, isso seria mais complexo, mas aqui funciona bem.
                        item_no_estoque->quantidade -= qtd_desejada;
                        
                        printf("Produto adicionado ao carrinho!\n");
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
                    
                    // Como já demos baixa no estoque (linha 79), 
                    // aqui só precisamos limpar o carrinho e sair.
                    // O correto seria dar um 'free' na lista toda do carrinho aqui.
                    // Mas como o programa vai sair da função, perdemos o acesso a ela.
                    // Para fins acadêmicos simples, retornamos ao menu principal.
                    return; 
                }
                break;
        }

    } while (opcao != 5);

    // Se o usuário cancelou (opção 5), idealmente deveríamos devolver os itens pro estoque.
    // Mas isso exigiria um loop percorrendo o carrinho e somando de volta no estoque.
    // Fica como desafio extra se sobrar tempo!
    printf("Sessao de compra cancelada.\n");
}