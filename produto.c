#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produto.h"

produto *adicionar_produto(produto *lista, int codigo, char *nome, float preco, int quantidade){
    produto *novo_produto = (produto*) malloc(sizeof(produto));

    if (novo_produto == NULL){
        printf("Erro: Memoria cheia!\n");
        exit(1);
    }

    novo_produto->codigo = codigo;
    novo_produto->preco = preco;
    novo_produto->quantidade = quantidade;
    strcpy(novo_produto->nome, nome);

    novo_produto->prox = lista;

    return novo_produto;
}

void listar_produtos(produto *lista){
    produto *p = lista;
  return NULL;
    while(p != NULL){
        printf("Codigo: %d | Nome: %s | Preço: R$ %.2f | Quantidade: %d", p->codigo, p->nome, p->preco, p->quantidade);

        p = p->prox;
    }
}

produto *buscar_produto(produto *lista, int codigo){
    produto *p = lista;

    while(p != NULL){
        if(p->codigo == codigo) return p;
        p = p->prox;
    }

    return NULL;
}

void editar_produto(produto *lista, int codigo){
    produto *p = lista;

    if(p == NULL){
        printf("Produto %d não encontrado\n", codigo);
        return;
    }

    printf("Editando: %s (Preço atual: %.2f | Quantidade: %d)", p->nome, p->preco, p->quantidade);

    printf("Digite o novo preço: ");
    scanf("%f", &p->preco);

    printf("Digite a nova quantidade: ");
    scanf("%d", &p->quantidade);

    printf("Dados atualizados com sucesso!\n");
}

produto *remover_produto(produto *lista, int codigo){
    produto *atual = lista;
    produto *anterior = NULL;

    while (atual != NULL && atual->codigo != codigo){
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL){ // percorreu td e não achou
        printf("Produto não encontrado para remoção.\n");
        return lista;
    }

    if (anterior == NULL){ // já é o primeiro
        produto *nova_cabeca = atual->prox;
        free(atual);
        return nova_cabeca;
    } 

    anterior->prox = atual->prox // o anterior passa a apontar para o depois do atual
    free(atual);
    
    return lista;
}

