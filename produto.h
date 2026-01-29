#ifndef PRODUTO_H
#define PRODUTO_H

typedef struct Produto{
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
    struct Produto *prox; //cria um ponteiro do tipo 'Produto' pra apontar pro próximo da lista
} produto;

produto *adicionar_produto(produto *lista, int codigo, char *nome, float preco, int quantidade);

void listar_produtos(produto *lista);

produto *buscar_produto(produto *lista, int codigo);

produto *remover_produto(produto *lista, int codigo);

void editar_produto(produto *lista, int codigo_busca);


#endif
