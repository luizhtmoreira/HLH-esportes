# HLH Esportes - Sistema de Gestão de Loja

Projeto acadêmico desenvolvido para a disciplina de **Estrutura de Dados e Algoritmos** da Universidade de Brasília (UnB), campus Gama (FCTE). O sistema gerencia o estoque, clientes e vendas de uma loja de artigos esportivos, focando na manipulação de estruturas de dados dinâmicas e persistência em banco de dados relacional.

## Tecnologias Utilizadas

* **Linguagem:** C 
* **Banco de Dados:** SQLite3
* **Estrutura de Dados:** Listas Encadeadas Simples
* **Gerenciamento de Memória:** Alocação Dinâmica (`malloc`, `free`)

---

## Estrutura do Projeto

O projeto adota uma estrutura plana, facilitando a manutenção e a separação de responsabilidades. Por ser um projeto acadêmico, optou-se por essa organização.

```text
HLH_ESPORTES/
│
├── main.c              # Ponto de entrada, menu principal e configurações de locale
├── banco.c / .h        # Camada de persistência e integração direta com SQLite3
├── cliente.c / .h      # Lógica de negócio e Lista Encadeada para Clientes
├── produto.c / .h      # Lógica de negócio e Lista Encadeada para Produtos
├── modo_compra.c / .h  # Lógica transacional de vendas e gerenciamento de carrinho
├── sqlite3.c / .h      # Driver do SQLite (Amalgamation)
├── loja_database.db    # Arquivo de dados gerado automaticamente
└── README.md
```

## Arquitetura e Estruturas de Dados

O sistema implementa uma abordagem híbrida onde os dados são carregados do banco para a memória RAM no início da execução.

### Principais Implementações:
* **Listas Encadeadas:** Utilizadas para gerenciar  a lista de clientes e também o estoque de produtos. Permitindo inserção e remoção dinâmica sem tamanho fixo.
* **Ponteiros de Estrutura:** Cada nó da lista possui um ponteiro `prox` para o próximo elemento. O carrinho de compras é, na verdade, uma sub-lista de produtos temporária associada a um cliente.
* **Persistência SQL:** As funções em `banco.c` traduzem as operações da lista (`INSERT`, `UPDATE`, `DELETE`) para comandos SQL, garantindo que os dados sobrevivam ao encerramento do programa.

---

## Funcionalidades Implementadas

### Gestão de Clientes
* **CRUD Completo:** Cadastro, Leitura, Atualização e Remoção.
* **Busca:** Algoritmo de busca linear na lista encadeada por CPF.
* **Validações:** Verifica formato de CPF (11 dígitos), datas e nomes.
* **Histórico:** Consulta ao banco de dados para exibir todas as compras passadas de um cliente específico.

### Gestão de Produtos
* **CRUD Completo:** Cadastro, Leitura, Atualização e Remoção.
* **Categorização:** Produtos divididos em categorias (Futebol, Basquete).
* **Controle de Saldo:** Baixa automática no estoque no momento da finalização da venda.
* **Recriação de Itens:** Se uma compra é cancelada, os itens voltam para a lista de estoque (recriando o nó se necessário).

### Sistema de Vendas (Modo Compra)
* **Login de Cliente:** Vínculo da venda a um CPF existente, criando um carrinho. Com isso ele pode adicionar, listar (mostrando a quantidade de itens e o valor da compra) e remover produtos do carrinho na hora que quiser.
* **Carrinho de Compras:** Lista encadeada temporária que armazena os produtos selecionados.
* **Cupom Fiscal:** Exibição detalhada dos itens e total no terminal.
* **Transacionalidade:** Uso de `BEGIN TRANSACTION` e `COMMIT` no SQLite para garantir a integridade da venda.

---

## Como Executar

### Pré-requisitos
* Compilador GCC.
* Bibliotecas padrão do C e pthreads/dl (necessárias para o SQLite no Linux).

### Compilação
Abra o terminal na pasta raiz do projeto e execute o seguinte comando:

```bash
gcc -Wall main.c banco.c cliente.c produto.c modo_compra.c sqlite3.c -o sistema_hlh
```

### Execução

Após compilar, execute o programa utilizando o comando correspondente ao seu sistema operacional:

**Linux / Mac:**
```bash
./sistema_hlh
```
**Windows:**
```bash
sistema_hlh.exe
```
---

## Autores

* **Luiz Henrique Tomaz Moreira**
* **Henrique Mendes Elias**
* *Engenharia de Software (UnB - FCTE)*

