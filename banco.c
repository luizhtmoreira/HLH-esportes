#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"
#include "banco.h"
#include "produto.h"
#include "cliente.h"

#define DB_NAME "loja_database.db"

void inicializar_banco(){
    sqlite3 *db;
    char *erro = 0;
    int rc = sqlite3_open(DB_NAME, &db);

    if (rc){
        printf("Erro ao abrir banco: %s\n", sqlite3_errmsg(db));
        return;
    }

    char *sql_vendas = "CREATE TABLE IF NOT EXISTS vendas ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "cpf_cliente TEXT NOT NULL, "
                "nome_cliente TEXT NOT NULL, "
                "produto_nome TEXT NOT NULL, "
                "qtd INTEGER, "
                "total REAL, "
                "data_hora DATETIME DEFAULT CURRENT_TIMESTAMP);";
    sqlite3_exec(db, sql_vendas, 0, 0, &erro);

    char *sql_prod = "CREATE TABLE IF NOT EXISTS produtos ("
                     "codigo INTEGER PRIMARY KEY, "
                     "nome TEXT, preco REAL, qtd INTEGER, categoria INTEGER);";
    sqlite3_exec(db, sql_prod, 0, 0, &erro);

    char *sql_cli = "CREATE TABLE IF NOT EXISTS clientes ("
                    "cpf TEXT PRIMARY KEY, nome TEXT, email TEXT, "
                    "telefone TEXT, nascimento TEXT);";
    sqlite3_exec(db, sql_cli, 0, 0, &erro);

    sqlite3_close(db);
}

void salvar_venda_sql(char *cpf, char *nome_cliente, produto* carrinho){
    sqlite3 *db;
    char *erro = 0;
    sqlite3_open(DB_NAME, &db);

    produto *p = carrinho;
    char sql[500];

    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);

    while (p != NULL){
        float total_item = p->preco * p->quantidade;
        sprintf(sql, "INSERT INTO vendas (cpf_cliente, nome_cliente, produto_nome, qtd, total) "
                     "VALUES ('%s', '%s', '%s', %d, %.2f);",
                     cpf, nome_cliente, p->nome, p->quantidade, total_item);

        int rc = sqlite3_exec(db, sql, 0, 0, &erro);

        if (rc != SQLITE_OK) {
            printf("Erro ao salvar item %s: %s\n", p->nome, erro);
            sqlite3_free(erro);
        }

        p = p->prox;
    }
    sqlite3_exec(db, "COMMIT;", 0, 0, 0);
    printf("Venda registrada no Banco de Dados com sucesso!\n");
    sqlite3_close(db);
}

void imprimir_linha_sql(sqlite3_stmt *res) {
    
    const unsigned char *data = sqlite3_column_text(res, 6);
    const unsigned char *prod = sqlite3_column_text(res, 3);
    const unsigned char *cliente = sqlite3_column_text(res, 2);
    int qtd = sqlite3_column_int(res, 4);
    double total = sqlite3_column_double(res, 5);

    if (cliente != NULL){
        printf("[%s] %s comprou %dx %s (R$ %.2f)\n", data, cliente, qtd, prod, total);
    } else {
        printf("[%s] %dx %s (R$ %.2f)\n", data, qtd, prod, total);
    }
}

void listar_historico_geral_sql(){
    sqlite3 *db;
    sqlite3_stmt *res;

    sqlite3_open(DB_NAME, &db);

    char *sql = "SELECT * FROM vendas ORDER BY id DESC;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc != SQLITE_OK) {
        printf("Erro ao buscar dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n=== HISTORICO GERAL DE VENDAS ===\n");
    
    int encontrou = 0;
    while (sqlite3_step(res) == SQLITE_ROW) {
        imprimir_linha_sql(res);
        encontrou = 1;
    }

    if (!encontrou) printf("Nenhuma venda registrada no sistema.\n");
    printf("==========================================\n");

    sqlite3_finalize(res); 
    sqlite3_close(db);
}

void listar_historico_cliente_sql(char *cpf_busca){
    sqlite3 *db;
    sqlite3_stmt *res;

    sqlite3_open(DB_NAME, &db);

    char *sql = "SELECT * FROM vendas WHERE cpf_cliente = ? ORDER BY id DESC;";

    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_text(res, 1, cpf_busca, -1, SQLITE_STATIC);

    printf("\n=== HISTORICO DO CLIENTE %s ===\n", cpf_busca);

    while (sqlite3_step(res) == SQLITE_ROW) {
        imprimir_linha_sql(res); 
    }

    sqlite3_finalize(res); 
    sqlite3_close(db);

}

void salvar_produto_sql(produto *p){
    sqlite3 *db;
    char sql[300];
    sqlite3_open(DB_NAME, &db);
    sprintf(sql, "INSERT INTO produtos VALUES (%d, '%s', %.2f, %d, %d);",
            p->codigo, p->nome, p->preco, p->quantidade, p->categoria);
    sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_close(db);
}

void atualizar_produto_sql(produto *p, int codigo_original) {
    sqlite3 *db;
    char sql[300];
    sqlite3_open(DB_NAME, &db);
    
    sprintf(sql, "UPDATE produtos SET codigo=%d, nome='%s', preco=%.2f, qtd=%d, categoria=%d WHERE codigo=%d;",
            p->codigo, p->nome, p->preco, p->quantidade, p->categoria, codigo_original);
            
    sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_close(db);
}

void remover_produto_sql(int codigo) {
    sqlite3 *db;
    char sql[100];
    sqlite3_open(DB_NAME, &db);
    sprintf(sql, "DELETE FROM produtos WHERE codigo=%d;", codigo);
    sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_close(db);
}

produto* carregar_produtos_do_sql() {
    sqlite3 *db;
    sqlite3_stmt *res;
    sqlite3_open(DB_NAME, &db);
    
    if(sqlite3_prepare_v2(db, "SELECT * FROM produtos;", -1, &res, 0) != SQLITE_OK){
        sqlite3_close(db); return NULL;
    }

    produto *lista = NULL; 

    while (sqlite3_step(res) == SQLITE_ROW) {
        produto *novo = (produto*) malloc(sizeof(produto));
        novo->codigo = sqlite3_column_int(res, 0);
        strcpy(novo->nome, (char*)sqlite3_column_text(res, 1));
        novo->preco = sqlite3_column_double(res, 2);
        novo->quantidade = sqlite3_column_int(res, 3);
        novo->categoria = sqlite3_column_int(res, 4);
        novo->prox = lista;
        lista = novo;
    }
    sqlite3_finalize(res);
    sqlite3_close(db);
    return lista;
}

void salvar_cliente_sql(cliente *c) {
    sqlite3 *db;
    char sql[500];
    sqlite3_open(DB_NAME, &db);
    sprintf(sql, "INSERT INTO clientes VALUES ('%s', '%s', '%s', '%s', '%s');",
            c->CPF, c->nome, c->email, c->telefone, c->data_de_nascimento);
    sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_close(db);
}

void atualizar_cliente_sql(cliente *c, char *cpf_original) {
    sqlite3 *db;
    char sql[500];
    sqlite3_open(DB_NAME, &db);
    
    sprintf(sql, "UPDATE clientes SET cpf='%s', nome='%s', email='%s', telefone='%s', nascimento='%s' WHERE cpf='%s';",
            c->CPF, c->nome, c->email, c->telefone, c->data_de_nascimento, cpf_original);
            
    sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_close(db);
}

void remover_cliente_sql(char *cpf) {
    sqlite3 *db;
    char sql[200];
    sqlite3_open(DB_NAME, &db);
    sprintf(sql, "DELETE FROM clientes WHERE cpf='%s';", cpf);
    sqlite3_exec(db, sql, 0, 0, 0);
    sqlite3_close(db);
}

cliente* carregar_clientes_do_sql() {
    sqlite3 *db;
    sqlite3_stmt *res;
    sqlite3_open(DB_NAME, &db);

    if(sqlite3_prepare_v2(db, "SELECT * FROM clientes;", -1, &res, 0) != SQLITE_OK){
        sqlite3_close(db); return NULL;
    }

    cliente *lista = NULL;

    while (sqlite3_step(res) == SQLITE_ROW) {
        cliente *novo = (cliente*) malloc(sizeof(cliente));
        strcpy(novo->CPF, (char*)sqlite3_column_text(res, 0));
        strcpy(novo->nome, (char*)sqlite3_column_text(res, 1));
        strcpy(novo->email, (char*)sqlite3_column_text(res, 2));
        strcpy(novo->telefone, (char*)sqlite3_column_text(res, 3));
        strcpy(novo->data_de_nascimento, (char*)sqlite3_column_text(res, 4));
        
        novo->carrinho = NULL;
        novo->prox = lista;
        lista = novo;
    }
    sqlite3_finalize(res);
    sqlite3_close(db);
    return lista;
}




