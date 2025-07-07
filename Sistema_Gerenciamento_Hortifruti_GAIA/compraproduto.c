#include <stdio.h>
#include "compracadastro.h"

#define MAX 100

int quantidadeProd = 0;

typedef struct {
    int id;
    char nome[50];
    float preco_compra;
    float preco_venda;
    int quantidade;
    int categoria;
    int fornecedor;
} Produto;

Produto produtos_lista[MAX];

int proximo_id2 = 1;

void carregarProdutosCompra(){
    int i = 0;
    quantidadeProd = 0;
    FILE *arquivo = fopen("banco_dados/produtos.csv", "r");
    if(!arquivo){
        printf("\nNão há produtos cadastrados!\n");
        return 0;
    }

    if (arquivo != NULL) {
        char linha[100];
        fgets(linha, sizeof(linha), arquivo);
        while (fscanf(arquivo, "%d,%49[^,],%f,%f,%i,%d, %d\n",
            &produtos_lista[i].id,
            produtos_lista[i].nome,
            &produtos_lista[i].preco_compra,
            &produtos_lista[i].preco_venda,
            &produtos_lista[i].quantidade,
            &produtos_lista[i].categoria,
            &produtos_lista[i].fornecedor) == 7) {
            quantidadeProd++;
            i++;
            if (produtos_lista[quantidadeProd - 1].id >= proximo_id2) {
                proximo_id2 = produtos_lista[quantidadeProd - 1].id + 1;
            }
        }
        fclose(arquivo);
    }
}

int mostrarProdutosCompra(){

    carregarProdutosCompra();

    for (int i = 0; i < quantidadeProd; i++){
        printf("%d - Nome: %s | Preço de Compra: %.2f | Preço de Venda: %.2f | Quantidade: %d | Categoria: ",
               produtos_lista[i].id,
               produtos_lista[i].nome,
               produtos_lista[i].preco_compra,
               produtos_lista[i].preco_venda,
               produtos_lista[i].quantidade);
        int categoriaProd;
        categoriaProd = produtos_lista[i].categoria;
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
    return 0;
}

void salvarCompra(){
    FILE *arquivo = fopen("banco_dados/produtos.csv", "w");

    // Cabeçalho do arquivo
    fprintf(arquivo, "ID,Nome,Preco de Compra,Preco de Venda,Quantidade,Categoria\n");

    // Salvando os produtos
    for (int i = 0; i < quantidadeProd; i++) {
        fprintf(arquivo, "%d,%s,%.2f,%.2f,%d,%d,%d\n",
                produtos_lista[i].id,
                produtos_lista[i].nome,
                produtos_lista[i].preco_compra,
                produtos_lista[i].preco_venda,
                produtos_lista[i].quantidade,
                produtos_lista[i].categoria,
                produtos_lista[i].fornecedor);
    }
    fclose(arquivo);
    printf("\nDados salvos com sucesso no arquivo banco_dados/produtos.csv!\n");

}

void realizarCompra(){
    printf("\nQual produto deseja comprar? [Digite 0 para CANCELAR]: ");
    int escolhaProd= 0;
    scanf("%d", &escolhaProd);
    printf("\n");

    if(escolhaProd != 0 && escolhaProd < quantidadeProd + 1){
        printf("==============================================\n");
        printf("\nQual a quantidade de %s que deseja comprar? [Digite 0 para CANCELAR]: ", produtos_lista[escolhaProd - 1].nome);
        int escolhaQtd= 0;
        scanf("%d", &escolhaQtd);
        printf("\n==============================================\n\n");
        if(escolhaQtd != 0){
            printf("Você deseja comprar %dx %s(s)? [1 - S/0 - N]\n", escolhaQtd, produtos_lista[escolhaProd - 1].nome);
            int escolhaAlt;
            scanf("%d", &escolhaAlt);

            while(escolhaAlt != 1 && escolhaAlt != 0){
                printf("\nEscolha inválida, tente novamente\n");
                printf("Você deseja comprar %dx %s(s)? [1 - S/0 - N]\n", escolhaQtd, produtos_lista[escolhaProd - 1].nome);
                scanf("%d", &escolhaAlt);
            }

            if(escolhaAlt == 1){
                printf("%dx %s(s) foram adicionados ao estoque!", escolhaQtd, produtos_lista[escolhaProd - 1].nome);
                produtos_lista[escolhaProd - 1].quantidade = escolhaQtd;
                salvarCompra();
                return;
            }
            else{
                printf("%dx %s(s) não foram adicionados ao estoque!", escolhaQtd, produtos_lista[escolhaProd - 1].nome);
            }
        }
    }
    printf("COMPRA CANCELADA!!!");
    return;
}

void comprarProduto(int id){
    int idUser = id;
    idUser = id;
    printf("==============================================\n\n");
    printf("O que deseja Comprar?");
    printf("\n\n==============================================\n\n");

    mostrarProdutosCompra();
    realizarCompra();

    getchar();
    getchar();
    limpar_tela();
    return;
}
