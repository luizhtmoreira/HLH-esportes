#include <stdio.h>
#include <stdlib.h>

#include "cliente.h"

int main(){
    cliente *lista_de_clientes = inicializar_lista_cliente();
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
                printf("\nem desenvolvimento...\n");
                break;

            case 3:
                printf("\nem desenvolvimento...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}