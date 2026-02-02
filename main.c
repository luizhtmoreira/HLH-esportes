#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "cliente.h"
#include "produto.h"
#include "modo_compra.h"

int main(){
    cliente *lista_de_clientes = inicializar_lista_cliente();
    produto * lista_de_produtos = NULL;
    
    int opcao;

    do{
        printf("\n=== HLH ESPORTES ===\n");
        printf("1. Gestão dos Clientes\n");
        printf("2. Gestão dos Produtos\n");
        printf("3. Modo Compra\n");
        printf("0. Sair\n");

        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 0:
                printf("\nEncerrando o sistema...\n");
                break;

            case 1:
                menu_clientes(&lista_de_clientes);
                break;

            case 2:
                {
                    int op_prod;
                    do{
                        printf("\n--- GESTAO DE ESTOQUE ---\n");
                        printf("1. Cadastrar Produto\n");
                        printf("2. Listar Estoque\n");
                        printf("3. Editar Produto\n");
                        printf("4. Remover Produto\n");
                        printf("0. Voltar\n");
                        printf("Escolha: ");
                        scanf("%d", &op_prod);

                        int cod, qtd;
                        float preco;
                        char nome[50];  

                        switch(op_prod){
                            case 1:

                                char buffer_cod[50];
                                int cod_valido = 0;

                                do {
                                    printf("Codigo: ");
                                    scanf(" %s", buffer_cod);

                                    cod_valido = 1;
                                    for (int i=0; buffer_cod[i] != '\0'; i++){
                                        if (!isdigit(buffer_cod[i])){
                                            cod_valido = 0;
                                            break;
                                        }
                                    }
                                    
                                    if(!cod_valido){
                                        printf("Erro: O codigo deve conter APENAS inteiros positivos. Tente novamente.\n");
                                    }

                                }while(!cod_valido);

                                cod = atoi(buffer_cod);

                                printf("Nome: "); scanf(" %[^\n]", nome);
                                printf("Preco: "); scanf("%f", &preco);
                                printf("Qtd Inicial: "); scanf("%d", &qtd);
                                
                                lista_de_produtos = adicionar_produto(lista_de_produtos, cod, nome, preco, qtd);
                                printf("Produto cadastrado!\n");
                                break;
                            case 2:
                                printf("\n--- Estoque Atual ---\n");
                                listar_produtos(lista_de_produtos);
                                break;
                            case 3:
                                printf("Codigo para editar: "); scanf("%d", &cod);
                                editar_produto(lista_de_produtos, cod);
                                break;
                            case 4:
                                printf("Codigo para remover: "); scanf("%d", &cod);
                                lista_de_produtos = remover_produto(lista_de_produtos, cod);
                                break;
                        }                      
                    }while (op_prod != 0);
                }
                break;

            case 3:
                {
                    char cpf_login[14];
                    printf("\n--- LOGIN NO CAIXA ---\n");
                    printf("Digite o CPF do cliente: ");
                    scanf(" %[^\n]", cpf_login);

                    cliente *cliente_atual = buscar_cliente(lista_de_clientes, cpf_login);

                    if (cliente_atual == NULL) {
                        printf("ERRO: Cliente nao encontrado. Cadastre-o no menu 1 antes de comprar.\n");
                    } else {
                        iniciar_modo_compra(lista_de_produtos, cliente_atual);
                    }
                }
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}