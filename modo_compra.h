#ifndef MODO_COMPRA_H
#define MODO_COMPRA_H

#include "produto.h"
#include "cliente.h"

void iniciar_modo_compra(produto **estoque, cliente *comprador);

void limpar_carrinho(produto *carrinho);

#endif