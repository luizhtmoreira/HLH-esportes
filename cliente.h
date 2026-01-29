#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Cliente{
    char CPF[13];
    char nome[100];
    char email[100];
    char telefone[100];
    char data_de_nascimento[11];

    void *carrinho; //ponteiro p carrinho 
    struct Cliente *prox; //ponteiro p proximo cliente: lista encadeada

} cliente; 

void menu_clientes(cliente **inicio);

void cadastrar_cliente(cliente **inicio);

cliente* inicializar_lista_cliente();

void listar_clientes(cliente *inicio);

cliente* buscar_cliente(cliente *inicio,                     char *cpf_busca);

void editar_cliente(cliente *inicio);

void remover_cliente(cliente **inicio);




#endif