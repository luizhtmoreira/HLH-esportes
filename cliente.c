#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cliente.h"

void menu_clientes(cliente **inicio){
    printf("função nao implementada");
}


cliente* criar_cliente(){
    cliente *novo = (cliente*) malloc(sizeof(cliente));

    if (novo == NULL){
        printf("Memória insuficiente!\n");
        return NULL;
    }


}

void cadastrar_cliente(cliente **inicio){


}

cliente* inicializar_lista_cliente(){
    return NULL;
}

void listar_clientes(cliente *inicio){
    printf("função nao implementada");
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