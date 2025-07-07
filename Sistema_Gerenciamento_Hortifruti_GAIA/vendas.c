#include <stdio.h>
#include <stdlib.h>
#include "vendas.h"

#define NOMEMAX 50
#define PRODMAXVENDA 50
#define VENDAMAX 999
#define NOMEFUNC 40
#define MAXFUNC
#define PRODMAX 100

int opcaovenda;
int vendaQtd;
int quantidadeFunc;
int USERID;
int quantprod;
int proximo_id3;

typedef struct{
    char nome[NOMEFUNC];
}funcionario; funcionario f[MAXFUNC];

typedef struct{
    int id;
    int quantidade;
}produtoVendido;

typedef struct{
    int id_venda;
    int id_func;
    char nome_cliente[NOMEMAX];
    unsigned long long int cpf_cliente;
    int qtdItem;
    produtoVendido produtos[PRODMAXVENDA];
    float ValorTotal;
} venda; venda vendas[VENDAMAX];

typedef struct {
    int id;
    char nome[50];
    float preco_compra;
    float preco_venda;
    int quantidade;
    int categoria;
    int fornecedor;
} Produto;

Produto lista_produtos[PRODMAX];

void LerFuncionariosList(){
    FILE *bd_funcionarios = fopen("banco_dados/funcionarios.csv", "r");

    if(!bd_funcionarios){
        printf("\nNão há funcionários cadastrados!\n");
        limpar_tela();
        return 0;
    }

    char buffer_funcionario[1024];
    char *data;

    fgets(buffer_funcionario, sizeof(buffer_funcionario), bd_funcionarios);

    int i = 0;

    while(fgets(buffer_funcionario, sizeof(buffer_funcionario), bd_funcionarios)){ //Confere se ainda tem dados nas linhas
        data = strtok(buffer_funcionario, ",");
        strcpy(f[i].nome, data);
        i++;
    }
    fclose(bd_funcionarios);
}

void LerFuncionariosVenda(int id){
    FILE *bd_funcionarios = fopen("banco_dados/funcionarios.csv", "r");
    if(!bd_funcionarios){
        printf("\nNão há funcionários cadastrados!\n");
        limpar_tela();
        return 0;
    }

    char buffer_funcionario[1024];
    char *data;

    fgets(buffer_funcionario, sizeof(buffer_funcionario), bd_funcionarios);

    int i = 0;

    while(fgets(buffer_funcionario, sizeof(buffer_funcionario), bd_funcionarios)){ //Confere se ainda tem dados nas linhas
        data = strtok(buffer_funcionario, ",");
        strcpy(f[i].nome, data);
        i++;
    }
    fclose(bd_funcionarios);

    vendas[vendaQtd].id_func = id;
    quantidadeFunc = i;
}

void mostrarProdutosVenda(){
    for (int i = 0; i < quantprod; i++){
        printf("%d - Nome: %s | Preço de Compra: %.2f | Preço de Venda: %.2f | Quantidade: %d | Categoria: ",
               lista_produtos[i].id,
               lista_produtos[i].nome,
               lista_produtos[i].preco_compra,
               lista_produtos[i].preco_venda,
               lista_produtos[i].quantidade);
        int categoriaProd;
        categoriaProd = lista_produtos[i].categoria;
        switch (categoriaProd){
            case 1:
                printf("%s", "Frutas");
                break;
            case 2:
                printf("%s", "Verduras");
                break;
            case 3:
                printf("%s", "Legumes");
                break;
            case 4:
                printf("%s", "Enlatados");
                break;
            case 5:
                printf("%s", "Outros");
                break;
        }
        printf("\n");
    }
}

void carregarProdutosVenda(){
    int i = 0;
    quantprod = 0;
    FILE *arquivo = fopen("banco_dados/produtos.csv", "r");
    if(!arquivo){
        printf("\nNão há produtos cadastrados!\n");
        return 0;
    }

    if (arquivo != NULL) {
        char linha[100];
        fgets(linha, sizeof(linha), arquivo);
        while (fscanf(arquivo, "%d,%49[^,],%f,%f,%i,%d,%d\n",
            &lista_produtos[i].id,
            lista_produtos[i].nome,
            &lista_produtos[i].preco_compra,
            &lista_produtos[i].preco_venda,
            &lista_produtos[i].quantidade,
            &lista_produtos[i].categoria,
            &lista_produtos[i].fornecedor) == 7)
        {
            quantprod++;
            i++;
            if (lista_produtos[quantprod - 1].id >= proximo_id3) {
                    proximo_id3 = lista_produtos[quantprod - 1].id + 1;
            }
        }
        fclose(arquivo);
    }
}

int adicionarProdutoVenda(int produto, int quantidade, int prodcount){
    printf("Deseja adicionar: %dx %s(s)? [S/N]", quantidade, lista_produtos[produto - 1].nome);
    char opcao = 'O';
    while(opcao != 's' && opcao != 'S' && opcao != 'n' && opcao != 'N'){
        scanf(" %c", &opcao);
        printf("\n");
        if(opcao == 's' || opcao == 'S'){
            if(lista_produtos[produto - 1].quantidade < quantidade || quantidade == 0){
                printf("Não há produtos o suficiente para adicionar a lista!");
            }else{
                vendas[vendaQtd].produtos[prodcount].id = lista_produtos[produto - 1].id;
                vendas[vendaQtd].produtos[prodcount].quantidade = quantidade;
                vendas[vendaQtd].ValorTotal += lista_produtos[produto - 1].preco_venda * quantidade;
                printf("%dx %s(s) foram adicionados a venda\n", quantidade, lista_produtos[produto - 1].nome);
                printf("\n");
                printf("Valor Total até o momento: R$%0.2f\n", vendas[vendaQtd].ValorTotal);
                return 1;
            }
        }
        else if(opcao == 'n' || opcao == 'N'){
            printf("%dx %s(s) não foram adicionados a venda\n", quantidade, lista_produtos[produto - 1].nome);
        }
    }
    return 0;
}

void salvarVenda(venda nova_venda){
    FILE *bd_vendas = fopen("banco_dados/vendas.csv", "a");
    if(bd_vendas == NULL){
        printf("Impossível encontrar o banco de dados de vendas. Por favor, contate o suporte");
        return;
    }else
    {
        fseek(bd_vendas, 0, SEEK_END); //Coloca o ponteiro no final do arquivo;
        int size = ftell(bd_vendas); //Define o tamanho da tabela baseado na posição do ponteiro;
        if(size == 0){ //Caso o tamanho da tabela = 0;
            // Cabeçalho do arquivo
            fprintf(bd_vendas, "IdV,IdF,Cliente,CPF,qtdItem,produtos,valorTotal\n");
        }
    }
    // Salvando os produtos
    fprintf(bd_vendas, "%d,%d,%s,%llu,%d,",
            vendas[vendaQtd].id_venda,
            vendas[vendaQtd].id_func,
            vendas[vendaQtd].nome_cliente,
            vendas[vendaQtd].cpf_cliente,
            vendas[vendaQtd].qtdItem);
    for(int j = 0; j < vendas[vendaQtd].qtdItem; j++){
        fprintf(bd_vendas, "%d,%d,",
                vendas[vendaQtd].produtos[j].id,
                vendas[vendaQtd].produtos[j].quantidade);
    }
    fprintf(bd_vendas, "%.2f\n", vendas[vendaQtd].ValorTotal);
    fclose(bd_vendas);
}

void carregarVendas(){
    FILE *bd_vendas = fopen("banco_dados/vendas.csv", "r");

    if(!bd_vendas){
    printf("Nenhuma venda foi efetuada\n");
    getchar();
    getchar();
    return 0;
    }

    char buffer_venda[1024]; //buffer para armazenar dados da linha
    char *data;

    int i = 0;

    fgets(buffer_venda, sizeof(buffer_venda), bd_vendas);
    //Lê o resto das linhas
    while(fscanf(bd_vendas, "%d,%d,%49[^,],%llu,%d,",
                      &vendas[i].id_venda,
                      &vendas[i].id_func,
                      vendas[i].nome_cliente,
                      &vendas[i].cpf_cliente,
                      &vendas[i].qtdItem)== 5){
        for(int j = 0; j < vendas[i].qtdItem; j++){
            fscanf(bd_vendas,"%d,%d,",
                    &vendas[i].produtos[j].id,
                    &vendas[i].produtos[j].quantidade);
        }
        fscanf(bd_vendas, "%f\n", &vendas[i].ValorTotal);
        i++;
      }
    vendaQtd = i;
    fclose(bd_vendas);
}

void registrarVenda(int id){

    carregarVendas();

    venda nova_venda;

    printf("==============================================\n\n");
    printf("Digite o pedido de venda");
    printf("\n\n==============================================\n\n");

    //CALCULAR ID DA VENDA
    vendas[vendaQtd].id_venda = vendaQtd + 1;
    printf("ID da venda: %d \n", vendas[vendaQtd].id_venda);
    //BUSCAR NOME FUNCIONARIO POR ID
    LerFuncionariosVenda(id);
    printf("Funcionario a realizar a venda: %s\n", f[vendas[vendaQtd].id_func].nome);
    printf("Digite o nome do Cliente [MAX 49]: ");
    //NOME CLIENTE
    char nome_cliente[NOMEMAX];
    scanf(" %49[^\n]", nome_cliente);
    strcpy(vendas[vendaQtd].nome_cliente, nome_cliente);
    //CPF CLIENTE
    unsigned long long int cpf_cliente;
    printf("Digite o CPF: ");
    scanf("%llu", &cpf_cliente);
    vendas[vendaQtd].cpf_cliente = cpf_cliente;

    carregarProdutosVenda();
    mostrarProdutosVenda();

    int qtd_variedade_produto = 0;
    char confirmacaoproduto = 's';
    while(confirmacaoproduto == 's' || confirmacaoproduto == 'S'){
        int id_opcao = 0;
        int add_qtd = 0;
        printf("Qual produto deseja adicionar a venda? ");
        scanf("%d", &id_opcao);
        printf("\n");
        printf("Quantos %s(s) deseja adicionar a venda? ", lista_produtos[id_opcao - 1].nome);
        scanf("%d", &add_qtd);
        qtd_variedade_produto += adicionarProdutoVenda(id_opcao, add_qtd, qtd_variedade_produto);
        printf("\n");
        printf("Quantidade de produtos na lista: %d", qtd_variedade_produto);
        printf("\n");
        printf("Deseja adicionar outro produto a venda? [S/N]\n");
        scanf(" %c", &confirmacaoproduto);
    }
    vendas[vendaQtd].qtdItem = qtd_variedade_produto;

    char confirmacao = 'o';
    while(confirmacao != 's' && confirmacao != 'S' && confirmacao != 'n' && confirmacao != 'N'){
        printf("==============================================\n\n");
        printf("CONFIRMAR ou CANCELAR venda? [S/N] ");
        scanf(" %c", &confirmacao);
        if(confirmacao == 's' || confirmacao == 'S'){
            printf("\n\n==============================================\n\n");
            printf("Pressione ENTER para confirmar Venda");
            printf("\n\n==============================================\n\n");
            salvarVenda(vendas[vendaQtd]);
            printf("Venda finalizada com sucesso!\n");;
        }
        else if(confirmacao == 'n' || confirmacao == 'N'){
            printf("\n==============================================\n\n");
            printf("Venda CANCELADA, pressione ENTER para sair");
            printf("\n\n==============================================\n\n");
        }
    }
    getchar();
    getchar();
    limpar_tela();
}

void visualizarVenda(){
    carregarVendas();
    LerFuncionariosList();
    carregarProdutosVenda();
    for(int i = 0; i < vendaQtd; i++){
        printf("==============================================\n\n");
        printf("Id venda: %d | ", vendas[i].id_venda);
        printf("Func. Venda: %s | ", f[vendas[i].id_func].nome);
        printf("Nome cliente: %s | ", vendas[i].nome_cliente);
        printf("Cpf cliente: %llu | ", vendas[i].cpf_cliente);
        printf("Variedade de itens: %d", vendas[i].qtdItem);
        printf("\n\n----------PRODUTOS----------\n\n");
        for(int j = 0; j < vendas[i].qtdItem; j++){
            printf("%d - %s - ", j + 1, lista_produtos[vendas[i].produtos[j].id - 1].nome);
            printf("Quantidade: %dx\n", vendas[i].produtos[j].quantidade);
        }

        printf("[Valor total: R$%.2f]\n", vendas[i].ValorTotal);
        printf("\n----------------------------\n");
    }
}

void salvarRemocaoVenda(){
    FILE *bd_vendas = fopen("banco_dados/vendas.csv", "w");
    if(bd_vendas == NULL){
        printf("Impossível encontrar o banco de dados de vendas. Por favor, contate o suporte");
        return;
    }else
    {
        fseek(bd_vendas, 0, SEEK_END);
        int size = ftell(bd_vendas);
        if(size == 0){
            fprintf(bd_vendas, "IdV,IdF,Cliente,CPF,qtdItem,produtos,valorTotal\n");
        }
    }
    vendaQtd--;
    for(int i = 0; i <= vendaQtd; i++){
        fprintf(bd_vendas, "%d,%d,%s,%llu,%d,",
                vendas[i].id_venda,
                vendas[i].id_func,
                vendas[i].nome_cliente,
                vendas[i].cpf_cliente,
                vendas[i].qtdItem);
        for(int j = 0; j < vendas[i].qtdItem; j++){
            fprintf(bd_vendas, "%d,%d,",
                    vendas[i].produtos[j].id,
                    vendas[i].produtos[j].quantidade);
        }
        fprintf(bd_vendas, "%.2f\n", vendas[i].ValorTotal);
    }
    fclose(bd_vendas);
}

void cancelarPedido(){
    carregarVendas();
    LerFuncionariosList();
    carregarProdutosVenda();
    if(vendaQtd < 1){
        printf("\n==============================================\n\n");
        printf("Nenhuma venda foi efetuada [pressione ENTER para retornar]");
        printf("\n\n==============================================\n\n");
        getchar();
        getchar();
        return;
    }
    visualizarVenda();
    printf("\n==============================================\n\n");
    printf("Qual pedido deseja cancelar? [Digite 0 para CANCELAR]");
    printf("\n\n==============================================\n\n");
    int opcao = 0;
    printf("Pedido: ");
    scanf("%d", &opcao);
    if(opcao == 0 || opcao > vendaQtd){
        limpar_tela();
        printf("Operação de cancelamento de pedido abortada\n");
        getchar();
        getchar();
        limpar_tela();
        return;
    }

    for(int i = opcao - 1; i < vendaQtd; i++){
        vendas[i] = vendas[i + 1];
        vendas[i].id_venda -= 1;
    }
    salvarRemocaoVenda();
    getchar();
    getchar();
    return;
}

void rotinaVendas(int id){
    int idUser = -1;
    idUser = id;
    while (1) {
        printf("Escolha uma opção:\n");
        printf("1 - Registrar pedido de venda\n");
        printf("2 - Visualizar Vendas\n");
        printf("3 - Cancelar pedido de venda\n");
        printf("4 - Retornar ao menu anterior\n");
        scanf("%d", &opcaovenda);

        if (opcaovenda == 1) {
            limpar_tela();
            registrarVenda(idUser);
        }else if(opcaovenda == 2){
            limpar_tela();
            visualizarVenda();
            printf("\n==============================================\n\n");
            printf("Pressione ENTER para voltar\n");
            printf("\n==============================================\n");
            getchar();
            getchar();
            limpar_tela();
        }
         else if (opcaovenda == 3) {
            limpar_tela();
            cancelarPedido();
            limpar_tela();
        } else if (opcaovenda == 4) {
            break; // Retorna ao menu anterior
        } else {
            printf("Opção não encontrada, digite novamente.\n");
        }
    }
}
