#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cliente.h"
#include "produto.h"
#include "banco.h"
#include "modo_compra.h"

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

        printf("\nEscolha uma opção: ");
        opcao = -1; 
    
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); 
        }

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
                    printf("\nDigite o CPF desejado: ");
                    scanf(" %[^\n]", cpf);
                    cliente *res = buscar_cliente(*inicio, cpf);
                    if(res){
                        printf("--------------------------------------------------\n");
                        printf("Cliente encontrado!\n");
                        printf("Nome: %s | CPF: %.3s.%.3s.%.3s-%.2s\n", res->nome, res->CPF, res->CPF + 3, res->CPF +6, res->CPF + 9);
                        int tam_tel = strlen(res->telefone);
                        if (tam_tel == 11){
                            printf("Email: %s | Telefone: (%.2s) %.5s-%.4s\n", res->email, res->telefone, res ->telefone +2, res->telefone +7);
                        } else {
                            printf("Email: %s | Telefone: (%.2s) %.4s-%.4s\n", res -> email, res->telefone, res->telefone + 2, res->telefone+6);
                        }
                        printf("--------------------------------------------------\n");
                    } else{
                        printf("Cliente não encontrado!\n");
                    }
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
                printf("Digite o CPF do cliente que deseja ver o histórico: ");
                scanf(" %[^\n]", cpf_busca);
                cliente *c = buscar_cliente(*inicio, cpf_busca);
                if (c == NULL){
                    printf("\n>> ERRO: CPF %s não encontrado no cadastro.\n", cpf_busca);
                } else {
                    listar_historico_cliente_sql(cpf_busca, c->nome);
                }  
            }
            break;
                
            case 0:
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}
int validar_nome (char *nome){
    int i = 0;

    while (nome[i] != 0){
        if (!isalpha((unsigned char)nome[i]) && nome[i] != ' '){
            return 0;
        }
        i++;
    }
    return 1;
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

    int nome_valido = 0;
    do {
        printf("NOME: ");
        scanf(" %[^\n]", novo_cliente->nome);

        if (validar_nome(novo_cliente->nome)){
            nome_valido = 1;
        } else {
            printf(">> ERRO: Nome deve conter apenas letras e espaços.\n");
        }
    } while (nome_valido == 0);

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

    if (buscar_cliente(*inicio, novo_cliente->CPF) != NULL) {
        printf("\n>> ERRO: CPF ja cadastrado no sistema!\n");
        free(novo_cliente);
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
    listar_clientes(inicio);
    printf("Digite o CPF ATUAL do cliente que deseja editar: ");
    scanf(" %[^\n]", cpf_busca);

    cliente *encontrado = buscar_cliente(inicio, cpf_busca);

    if (encontrado == NULL){
        printf("Nenhum cliente encontrado com o CPF %s.\n", cpf_busca);
        return;
    }

    printf("\nCliente encontrado, editando dados de: %s", encontrado->nome);
    char r;

    printf("\nDeseja editar o NOME ? (S/N): ");
    scanf(" %c", &r);

    if (r == 'S' || r == 's'){
        int nome_valido = 0;
        do{
            printf("NOME: ");
            scanf(" %[^\n]", encontrado->nome);

            if (validar_nome(encontrado->nome)){
            nome_valido = 1;

        } else {
            printf(">> ERRO: Nome deve conter apenas letras e espaços.\n");
        }
    } while (nome_valido == 0);
    }

    printf("\nDeseja editar o CPF? (S/N): ");
    scanf(" %c", &r); 

    if (r == 'S' || r == 's') {
        char novo_cpf_temp[15];
        int cpf_valido = 0;
        
        do {
            printf("Novo CPF: ");
            scanf(" %[^\n]", novo_cpf_temp);
            
            if (!validar_cpf(novo_cpf_temp)) {
                printf("Erro: Formato inválido.\n");
                continue;
            }
            
            if (strcmp(novo_cpf_temp, encontrado->CPF) != 0 && buscar_cliente(inicio, novo_cpf_temp) != NULL) {
                printf("Erro: Este CPF ja pertence a outro cliente!\n");
            } else {
                cpf_valido = 1;
                }
            } while (cpf_valido != 1);
            
            strcpy(encontrado->CPF, novo_cpf_temp);
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
        int telefone_valido = 0;
        do {
            printf("Numero (DDD + Telefone): ");
            scanf(" %[^\n]", encontrado->telefone);
            
            if (validar_telefone(encontrado->telefone)){
                telefone_valido = 1;
            } else {
                printf(">> ERRO: Telefone inválido! Digite exatamente 11 numeros.\n");
            }
    } while (telefone_valido == 0);
    }

    printf("\nDeseja editar a DATA DE NASCIMENTO? (S/N): ");
    scanf(" %c", &r); 

    if (r == 'S' || r == 's') {
        int data_valida = 0;
        do {
            printf("DATA DE NASCIMENTO (DD/MM/AAAA): ");
            scanf(" %[^\n]", encontrado->data_de_nascimento);
            
            if (validar_data(encontrado->data_de_nascimento)){
                data_valida = 1;
            } else {
                printf(">> ERRO: Formato invalido ! (XX/XX/XXXX).\n");
            }
        } while (data_valida == 0);
    }

    printf("\n--- Edicao concluida! ---\n");

    char cpf_velho[14];
    strcpy(cpf_velho, encontrado->CPF);
    atualizar_cliente_sql(encontrado, cpf_velho);
}

void remover_cliente(cliente **inicio){
    char cpf_deletado[14];
    printf("\n--- REMOVER CLIENTE ---\n");
    listar_clientes(*inicio);
    printf("Digite o CPF do cliente que deseja remover: ");
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
    limpar_carrinho((produto*) atual-> carrinho);
    atual -> carrinho = NULL;
    free(atual);
    printf("--- Cliente removido com sucesso ---\n");
}