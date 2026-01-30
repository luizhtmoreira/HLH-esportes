#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cliente.h"

void menu_clientes(cliente **inicio){
    int opcao;
    do {
        printf("\n=== MENU CLIENTES ===\n");
        printf("1. Cadastrar Novo\n");
        printf("2. Listar Todos\n");
        printf("3. Buscar por CPF\n");
        printf("4. Editar Cliente\n");
        printf("5. Remover Cliente\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: 
                cadastrar_cliente(inicio);
                break;
            case 2: 
                listar_clientes(*inicio);
                break;
            case 3: 
                {
                    char cpf[14];
                    printf("CPF: ");
                    scanf(" %[^\n]", cpf);
                    cliente *res = buscar_cliente(*inicio, cpf);
                    if(res) printf("Encontrado: %s\n", res->nome);
                    else printf("Nao encontrado.\n");
                }
                break;
            case 4:
                editar_cliente(*inicio);
                break;
            case 5:
                remover_cliente(inicio);
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
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
    cliente *atual = inicio;

    while (atual != NULL){
        if (strcmp(atual->CPF, cpf_busca) == 0){
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void editar_cliente(cliente *inicio){
    char cpf_busca[14];
    printf("\n--- EDITAR CADASTRO COMPLETO ---\n");
    printf("Digite o CPF ATUAL do cliente para encontrar: ");
    scanf(" %[^\n]", cpf_busca);

    cliente *encontrado = buscar_cliente(inicio, cpf_busca);

    if (encontrado == NULL){
        printf("Nenhum cliente encontrado com o CPF %s.\n", cpf_busca);
        return;
    }

    printf("\nEditando dados de: %s", encontrado->nome);
    char r;

    printf("\nDeseja editar o NOME ? (S/N): ");
    scanf(" %c", &r);

    if (r == 'S' || r == 's'){
        printf("Novo Nome: ");
        scanf(" %[^\n]", encontrado->nome); 
    }

    printf("\nDeseja editar o CPF? (S/N): ");
    scanf(" %c", &r); 

    if (r == 'S' || r == 's') {
        printf("Novo CPF: ");
        scanf(" %[^\n]", encontrado->CPF);
    }

    printf("\nDeseja editar o EMAIL? (S/N): ");
    scanf(" %c", &r); 

    if (r == 'S' || r == 's') {
        printf("Novo Email: ");
        scanf(" %[^\n]", encontrado->email);
    }

    printf("\nDeseja editar o TELEFONE? (S/N): ");
    scanf(" %c", &r); 

    if (r == 'S' || r == 's') {
        printf("Novo Telefone: ");
        scanf(" %[^\n]", encontrado->telefone);
    }

    printf("\nDeseja editar a DATA DE NASCIMENTO? (S/N): ");
    scanf(" %c", &r); 

    if (r == 'S' || r == 's') {
        printf("Nova Data: ");
        scanf(" %[^\n]", encontrado->data_de_nascimento);
    }

    printf("\n--- Edicao concluida! ---\n");
}

void remover_cliente(cliente **inicio){
    char cpf_deletado[14];
    printf("\n--- REMOVER CLIENTE ---\n");
    printf("Digite o CPF para remover: ");
    scanf(" %[^\n]", cpf_deletado);

    cliente *anterior = NULL;
    cliente *atual = *inicio;

    while (atual != NULL && strcmp(atual->CPF, cpf_deletado) != 0){
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL){
        printf("\nCliente não encontrado.\n");
        return;
    }

    if (anterior == NULL){ //alvo é o primeiro da lista
        *inicio = atual->prox; //inicio agora é o 2do
    } else { //alvo no meio ou fim
        anterior->prox = atual->prox; //anterior aponta p prox;
    }

    free(atual);
    printf("--- Cliente removido com sucesso ---");
}