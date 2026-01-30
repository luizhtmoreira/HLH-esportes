#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cliente.h"

void menu_clientes(cliente **inicio){
    int opcao;
    do{
        printf("\n=== MENU CLIENTES ===\n");
        printf("1. Cadastrar Novo\n");
        printf("2. Listar Todos\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            case 1:
                cadastrar_cliente(inicio);
                break;
            case 2:
                listar_clientes(*inicio);
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 0);
}


cliente* criar_cliente(){
    cliente *novo_cliente = (cliente*) malloc(sizeof(cliente));

    if (novo_cliente == NULL){
        printf("Memória insuficiente!\n");
        return NULL;
    }

    printf("\n--- CADASTRO DE CLIENTE ---\n");

    while(getchar() != '\n');

    printf("NOME: ");
    scanf(" %[^\n]", novo_cliente->nome);

    printf("CPF: ");
    scanf(" %[^\n]", novo_cliente->CPF);

    printf("EMAIL: ");
    scanf(" %[^\n]", novo_cliente->email);

    printf("TELEFONE: ");
    scanf(" %[^\n]", novo_cliente->telefone);

    printf("DATA DE NASCIMENTO (DD/MM/AAAA): ");
    scanf(" %[^\n]", novo_cliente->data_de_nascimento);

    novo_cliente->prox = NULL;
    novo_cliente->carrinho = NULL;

    return novo_cliente;
}

void cadastrar_cliente(cliente **inicio){
    cliente* novo_cliente = criar_cliente();

    if (novo_cliente == NULL){
        return;
    }

    if (*inicio == NULL){
        *inicio = novo_cliente;
    } 
    else{
        cliente *aux = *inicio;

        while (aux->prox != NULL){
            aux = aux->prox;
        }

        aux->prox = novo_cliente;
    }

    printf("\n--- Cliente %s cadastrado(a) com sucesso! ---\n", novo_cliente->nome);
}

cliente* inicializar_lista_cliente(){
    return NULL;
}

void listar_clientes(cliente *inicio){
    if (inicio == NULL){
        printf("\nNenhum cliente cadastrado, a lista está vazia!\n");
        return;
    }

    printf("\n--- LISTA DE CLIENTES ---\n");

    cliente *atual = inicio; //var auxiliar p nao perder o inicio

    while(atual != NULL){
        printf("Nome: %s | CPF: %s\n", atual->nome, atual->CPF);
        printf("Email: %s | Telefone: %s\n", atual->email, atual->telefone);
        printf("--------------------------------------------------\n");

        atual = atual->prox;
    }
}

cliente* buscar_cliente(cliente *inicio, char *cpf_busca){
    printf("função nao implementada");
    return NULL;
}

void editar_cliente(cliente *inicio){
    printf("função nao implementada");
}

void remover_cliente(cliente **inicio){
    printf("função nao implementada");
}