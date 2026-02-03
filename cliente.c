#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cliente.h"
#include "produto.h"
#include "banco.h"

void menu_clientes(cliente **inicio){
    int opcao;
    do {
        printf("\n=== MENU CLIENTES ===\n");
        printf("1. Cadastrar Novo\n");
        printf("2. Listar Todos\n");
        printf("3. Buscar por CPF\n");
        printf("4. Editar Cliente\n");
        printf("5. Remover Cliente\n");
        printf("6. Ver histórico de cliente\n");
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
            case 6:
            {
                char cpf_busca[15];
                printf("Digite o CPF do cliente para ver o histórico: ");
                scanf(" %[^\n]", cpf_busca);
                listar_historico_cliente_sql(cpf_busca);
            }
            break;
                
            case 0:
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

int validar_cpf(char *cpf){
    if (strlen(cpf) != 11){
        return 0;
    }

    for (int i = 0; i < 11; i++){
        if (!isdigit(cpf[i])){
            return 0;
        }
    }
    return 1;
}

int validar_telefone(char *telefone){
    int tam = strlen(telefone);
    if (strlen(telefone) != 11 && strlen(telefone) != 10){
        return 0;
    }

    for (int i = 0; i < tam; i++){
        if (!isdigit(telefone[i])){
            return 0;
        }
    }
    return 1;
}

int validar_data(char *data){
    if (strlen(data) != 10){
        return 0;
    }

    if (data[2] != '/' || data[5] != '/'){
        return 0;
    }

    for (int i = 0; i < 10; i++){
        if (i == 2 || i == 5){
            continue;
        }

        if (!isdigit(data[i])){
            return 0;
        }
    }
    return 1;
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

    int cpf_valido = 0;
    do {
        printf("CPF (Apenas 11 numeros): ");
        scanf(" %[^\n]", novo_cliente->CPF);

        if (validar_cpf(novo_cliente->CPF)){
            cpf_valido = 1;
        } else {
            printf(">> ERRO: CPF inválido! Digite exatamente 11 numeros.\n");
        }
    } while (cpf_valido == 0);

    printf("EMAIL: ");
    scanf(" %[^\n]", novo_cliente->email);

    int telefone_valido = 0;
    do {
        printf("Numero (DDD + Telefone): ");
        scanf(" %[^\n]", novo_cliente->telefone);

        if (validar_telefone(novo_cliente->telefone)){
            telefone_valido = 1;
        } else {
            printf(">> ERRO: Telefone inválido! Digite exatamente 11 numeros.\n");
        }
    } while (telefone_valido == 0);


    int data_valida = 0;
    do {
        printf("DATA DE NASCIMENTO (DD/MM/AAAA): ");
        scanf(" %[^\n]", novo_cliente->data_de_nascimento);

        if (validar_data(novo_cliente->data_de_nascimento)){
            data_valida = 1;
        } else {
            printf(">> ERRO: Formato invalido ! (XX/XX/XXXX).\n");
        }

    } while (data_valida == 0);

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
    salvar_cliente_sql(novo_cliente);
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
        printf("Nome: %s | CPF: %.3s.%.3s.%.3s-%.2s\n", atual->nome, atual->CPF, atual->CPF + 3, atual->CPF +6, atual->CPF + 9);
        
        int tam_tel = strlen(atual->telefone);
        
        if (tam_tel == 11){
            printf("Email: %s | Telefone: (%.2s) %.5s-%.4s\n", atual->email, atual->telefone, atual ->telefone +2, atual->telefone +7);
        } else {
            printf("Email: %s | Telefone: (%.2s) %.4s-%.4s\n", atual -> email, atual->telefone, atual->telefone + 2, atual->telefone+6);
        }
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

    char cpf_velho[14];
    strcpy(cpf_velho, encontrado->CPF);

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
    atualizar_cliente_sql(encontrado, cpf_velho);
}

void ver_carrinho_cliente(cliente *c){
    if (c->carrinho == NULL){
        printf("\nCarrinho vazio.\n");
        return;
    }

    printf("\n--- CARRINHO DE %s ---\n", c->nome);

    produto *p = (produto*) c->carrinho;
    float total = 0;

    while (p != NULL){
        float sub = p-> preco * p->quantidade;
        printf("- %dx %s (R$ %.2f) = R$ %.2f\n", p->quantidade, p->nome, p->preco, sub);
        total += sub;
        p = p->prox;
    }
    printf("-------------------------\n");
    printf("TOTAL: R$ %.2f\n", total);

}

void limpar_carrinho_cliente(cliente *c){
    if (c->carrinho == NULL){
        return;
    }

    produto *p = (produto*) c->carrinho;

    while(p != NULL){
        produto *temp = p;
        p = p->prox;
        free(temp);
    }
    c -> carrinho = NULL;
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

    remover_cliente_sql(cpf_deletado);
    limpar_carrinho_cliente(atual);
    free(atual);
    printf("--- Cliente removido com sucesso ---");
}