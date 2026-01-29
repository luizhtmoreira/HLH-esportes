#ifndef PRODUTO_H
#define PRODUTO_H

typedef struct Produto{
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
    struct Produto *prox; //cria um ponteiro do tipo 'Produto' pra apontar pro próximo da lista
} Produto;

Produto *adicionar_produto(Produto *lista, int codigo, char *nome, float preco, int quantidade);

void listar_produtos(Produto *lista);





#endif
