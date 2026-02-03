#ifndef BANCO_H
#define BANCO_H

#include "produto.h"
#include "cliente.h"


void inicializar_banco();

void salvar_venda_sql(char *cpf, char *nome_cliente, produto *carrinho);
void listar_historico_geral_sql();
void listar_historico_cliente_sql(char *cpf_busca, char *nome_cliente);

produto* carregar_produtos_do_sql();
void salvar_produto_sql(produto *p);
void atualizar_produto_sql(produto *p, int codigo_original);
void remover_produto_sql(int codigo);

cliente* carregar_clientes_do_sql();
void salvar_cliente_sql(cliente *c);
void atualizar_cliente_sql(cliente *c, char *cpf_original);
void remover_cliente_sql(char *cpf);

#endif