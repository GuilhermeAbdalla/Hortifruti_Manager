#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compraproduto.h"
#define MAX 100

int opcao;
int quantidade = 0;
int quantidadeForne = 0;

typedef struct {
    int id;
    char nome[50];
    float preco_compra;
    float preco_venda;
    int quantidade;
    int categoria;
    int fornecedor;
} Produto;

typedef struct {
    int id;
    char nome[50];
    long long int cnpj;
    char endereco[100]; //endereço
    int lista;
} fornecedor;

fornecedor Fornecedores[MAX];
Produto produtos[MAX];

int proximo_id = 1; // ID do próximo produto a ser cadastrado

// Função para salvar os produtos no arquivo CSV
void salvar_arquivo(Produto produtos[], int quantidade) {
    FILE *arquivo = fopen("banco_dados/produtos.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Cabeçalho do arquivo
    fprintf(arquivo, "ID,Nome,Preco de Compra,Preco de Venda,Quantidade,Categoria,Fornecedores\n");

    // Salvando os produtos
    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d,%s,%.2f,%.2f,%d,%d,%d\n",
                produtos[i].id, produtos[i].nome,
                produtos[i].preco_compra,
                produtos[i].preco_venda,
                produtos[i].quantidade,
                produtos[i].categoria,
                produtos[i].fornecedor);
    }

    fclose(arquivo);
    printf("Dados salvos com sucesso no arquivo banco_dados/produtos.csv!\n");
}

void carregarProdutos() {
    quantidade = 0;
    FILE *arquivo = fopen("banco_dados/produtos.csv", "r");
    if (arquivo != NULL) {
        char linha[100];
        fgets(linha, sizeof(linha), arquivo);
        while (fscanf(arquivo,
                      "%d,%49[^,],%f,%f,%d,%d,%d\n",
                      &produtos[quantidade].id,
                      produtos[quantidade].nome,
                      &produtos[quantidade].preco_compra,
                      &produtos[quantidade].preco_venda,
                      &produtos[quantidade].quantidade,
                      &produtos[quantidade].categoria,
                      &produtos[quantidade].fornecedor) == 7) {
            quantidade++;
            if (produtos[quantidade - 1].id >= proximo_id) {
                proximo_id = produtos[quantidade - 1].id + 1;
            }
        }
        fclose(arquivo);
    }
}

void visualizarProdutosPorCategoria(int categoria) {
    const char *categorias[] = {"Frutas", "Legumes", "Verduras", "Enlatados", "Outros"};
    printf("\nProdutos na categoria %s:\n", categorias[categoria - 1]);
    int encontrou = 0; // Para verificar se algum produto foi encontrado
    for (int i = 0; i < quantidade; i++) {
        if (produtos[i].categoria == categoria) {
            printf("ID: %d\n", produtos[i].id);
            printf("Nome: %s\n", produtos[i].nome);
            printf("Preço de Compra: %.2f\n", produtos[i].preco_compra);
            printf("Preço de Venda: %.2f\n", produtos[i].preco_venda);
            printf("Quantidade: %d\n", produtos[i].quantidade);
            carregarFornecedor();
            for(int j = 0; j < quantidadeForne; j++){
                if(produtos[i].fornecedor == Fornecedores[j].id){
                    printf("Fornecedor: %s\n", Fornecedores[j].nome);
                    break;
                }
            }
            printf("-----------------------------\n");
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("\n Nenhum produto encontrado na categoria %s.\n\n", categorias[categoria - 1]);
    }
    printf("=====================================================================\n\n");
    printf("pressione ENTER para retornar");
    printf("\n\n=====================================================================\n\n");
    getchar();
    getchar();
}

void carregarFornecedor() {
    FILE *arquivo = fopen("banco_dados/fornecedores.csv", "r");
    quantidadeForne = 0;
    if (arquivo != NULL) {
        char linha[100];
        fgets(linha, sizeof(linha), arquivo);
        while (fscanf(arquivo, "%d,%49[^,],%lld,%s\n",
                      &Fornecedores[quantidadeForne].id,
                      Fornecedores[quantidadeForne].nome,
                      &Fornecedores[quantidadeForne].cnpj,
                      &Fornecedores[quantidadeForne].endereco) == 4){
            quantidadeForne++;
        }
        fclose(arquivo);
    }
}

void visualizarFornecedor() {
    printf("\n LISTA DE FORNECEDORES\n");
    for (int i = 0; i < quantidadeForne; i++) {
        printf("ID: %d\n", Fornecedores[i].id);
        printf("Nome: %s\n", Fornecedores[i].nome);
        printf("CNPJ: %lld\n", Fornecedores[i].cnpj);
        printf("Endereço: %s\n", Fornecedores[i].endereco);
        printf("-----------------------------\n");
    }
}

void cadastrarProduto() {
    Produto novo_produto;
    novo_produto.id = quantidade > 0 ? produtos[quantidade - 1].id + 1 : 1;

    printf("==============================================\n\n");
    printf("Cadastrar produto:");
    printf("\n\n==============================================\n\n");

    printf("Digite o nome do produto:\n> ");
    scanf(" %49[^\n]", novo_produto.nome);

    printf("Digite o preço de compra do produto:\n> ");
    scanf("%f", &novo_produto.preco_compra);

    printf("Digite o preço de venda do produto:\n> ");
    scanf("%f", &novo_produto.preco_venda);

    novo_produto.quantidade = 0;

    printf("Escolha a categoria do produto:\n");
    printf("1 - Frutas\n2 - Verduras\n3 - Legumes\n4 - Enlatados\n5 - Outros\n");
    scanf("%d", &novo_produto.categoria);

    carregarFornecedor();
    visualizarFornecedor();
    printf("\nQual o fornecedor deste produto?\n");
    int opcaoForn = 0;
    scanf("%d", &opcaoForn);
    for(int i = 0; i < quantidadeForne; i++){
        if(opcaoForn == Fornecedores[i].id){
            novo_produto.fornecedor = Fornecedores[i].id;
            printf("fornecedor %s cadastrado como fornecedor deste produto", Fornecedores[i].nome);
        }
    }
    getchar();
    getchar();
    produtos[quantidade] = novo_produto;
    quantidade++;

//    qsort(produtos, quantidade, sizeof(Produto), compara_nome);

    salvar_arquivo(produtos, quantidade);

    printf("\nProduto cadastrado com sucesso!\n\n");
    printf("Pressione Enter para continuar...");
    getchar();
    getchar();
    limpar_tela();
}
int removerProduto() {

    int id;

    printf("\t\tLista de Produtos\t\n");
    printf("\t-----------------------------\t\n");

    for (int j = 0; j < quantidade; j++) {

        printf("%d - Nome: %s | Preço de Compra: %.2f | Preço de Venda: %.2f | Categoria: ",
               produtos[j].id,
               produtos[j].nome,
               produtos[j].preco_compra,
               produtos[j].preco_venda);
        int categoriaProd;
        categoriaProd = produtos[j].categoria;
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

    printf("\t-----------------------------\t\n");
    printf("\nDigite o ID do produto que deseja remover: ");
    scanf("%d", &id);
    getchar();

    int index = -1;

    for (int i = 0; i < quantidade; i++) {
        if (produtos[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nID não encontrado!\n");
        return 0;
    }

    for (int i = index; i < quantidade - 1; i++) {
        produtos[i] = produtos[i + 1];
        produtos[i].id = produtos[i].id - 1;
    }

    quantidade--;

    salvar_arquivo(produtos, quantidade);

    printf("Produto removido com sucesso!");
    printf("\n\n==============================================\n\n");
    printf("Pressione ENTER para continuar...");
    printf("\n\n==============================================\n\n");
    getchar();
    limpar_tela();
    return 0;
}


void visualizarCategorias() {
    while (1) {
        printf("==============================================\n\n");
        printf("Escolha uma categoria para visualizar:");
        printf("\n\n==============================================\n\n");
        printf("1 - Frutas\n");
        printf("2 - Verduras\n");
        printf("3 - Legumes\n");
        printf("4 - Enlatados\n");
        printf("5 - Outros\n");
        printf("0 - Retornar ao menu anterior\n");
        scanf("%d", &opcao);

        if (opcao >= 1 && opcao <= 5) {
            limpar_tela();
            visualizarProdutosPorCategoria(opcao);
            limpar_tela();
        } else if (opcao == 0) {
            limpar_tela();
            return;
        } else {
            printf("Opção não encontrada, digite novamente.\n");
            getchar();
            getchar();
            limpar_tela();
        }
    }
}

void alterarProduto() {
    int input;
    int id;

    printf("\t\tLista de Produtos\t\n");
    printf("\t-----------------------------\t\n");
    for (int j = 0; j < quantidade; j++) {
        printf(" %d - Nome: %s | Preço de Compra: %.2f | Preço de Venda: %.2f | Categoria: ",
               produtos[j].id,
               produtos[j].nome,
               produtos[j].preco_compra,
               produtos[j].preco_venda);
        int categoriaProd;
        categoriaProd = produtos[j].categoria;
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
    printf("\t-----------------------------\t\n");

    printf("\nDigite o ID do produto que deseja alterar: ");
    scanf("%d", &id);
    getchar();
    int index = -1;
    for (int i = 0; i < quantidade; i++) {
        if (produtos[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nID inválido!\n");
        return;
    }

    Produto *produto = &produtos[index];
    printf("Digite o novo nome do produto (ou digite X para manter o atual): ");
    char novo_nome[50];
    scanf(" %[^\n]", novo_nome);

    if (strcmp(novo_nome, "X") != 0) {
        strcpy(produto->nome, novo_nome);
    }

    printf("Digite o novo preço de compra (ou digite X para manter o atual): ");
    char input_preco_compra[10];
    scanf("%s", input_preco_compra);
    if (strcmp(input_preco_compra, "X") != 0) {
        produto->preco_compra = atof(input_preco_compra);
    }

    printf("Digite o novo preço de venda (ou digite X para manter o atual): ");
    char input_preco_venda[10];
    scanf("%s", input_preco_venda);
    if (strcmp(input_preco_venda, "X") != 0) {
        produto->preco_venda = atof(input_preco_venda);
    }

    printf("Digite a nova categoria do produto (ou digite 0 para manter o atual): ");
    int input_categoria = -1;
    scanf("%d", &input_categoria);
    if (input_categoria != 0) {
        produto->categoria =input_categoria;
    }

    printf("Produto alterado com sucesso!\n");

    salvar_arquivo(produtos, quantidade);

    getchar();
    getchar();

    limpar_tela();
}

void rotinaCompraCadastro(int id) {
    int idUser = id;
    while (1) {
        printf("Escolha uma opção:\n");
        printf("1 - Registrar novos produtos\n");
        printf("2 - Visualizar por categoria\n");
        printf("3 - Alterar produto\n");
        printf("4 - Remover produto\n");
        printf("5 - Comprar produto\n");
        printf("0 - Retornar ao menu anterior\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                limpar_tela();
                cadastrarProduto();
                break;
            case 2:
                limpar_tela();
                carregarProdutos();
                visualizarCategorias();
                break;
            case 3:
                alterarProduto();
                break;
            case 4:
                removerProduto();
                break;
            case 5:
                limpar_tela();
                comprarProduto(idUser);
                break;
            case 0:
                return;
            default:
                printf("Opção não encontrada, digite novamente.\n");
                getchar();
                getchar();
        }
    }
}
