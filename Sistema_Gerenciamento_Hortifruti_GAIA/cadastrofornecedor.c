#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

int opcao;
int quantidadeForn = 0;

typedef struct {
    int id;
    char nome[50];
    long long int cnpj;
    char endereco[100]; //endereço
    int lista;
} Fornecedor;

Fornecedor fornecedores[MAX];

// Função para comparar produtos por nome (usada no qsort)
int compara_nome(const void *a, const void *b) {
    Fornecedor *fornA = (Fornecedor *)a;
    Fornecedor *fornB = (Fornecedor *)b;
    return strcmp(fornA->nome, fornB->nome);
}

// Função para salvar os fornecedores no arquivo CSV
void salvar_forn(Fornecedor fornecedores[], int quantidadeForn) {
    FILE *arquivo = fopen("banco_dados/fornecedores.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }


// Cabeçalho do arquivo
    fprintf(arquivo, "ID,Nome do Fornecedor,CNPJ Fornecedor, Endereço\n");

    for (int i = 0; i < quantidadeForn; i++) {
        fprintf(arquivo, "%d,%s,%lld,%s\n", fornecedores[i].id, fornecedores[i].nome, fornecedores[i].cnpj, fornecedores[i].endereco);
    }

    fclose(arquivo);
    printf("Dados salvos com sucesso no arquivo fornecedor.csv!\n");
}

// Função para carregar fornecedores do arquivo
void carregarFornecedores() {
    FILE *arquivo = fopen("banco_dados/fornecedores.csv", "r");
    if (arquivo != NULL) {
        char linha[100];
        fgets(linha, sizeof(linha), arquivo);
        while (fscanf(arquivo, "%d,%49[^,],%lld,%s\n",
                      &fornecedores[quantidadeForn].id,
                      fornecedores[quantidadeForn].nome,
                      &fornecedores[quantidadeForn].cnpj,
                      &fornecedores[quantidadeForn].endereco) == 4){
            quantidadeForn++;
        }
        fclose(arquivo);
    }
}


// Função para visualizar fornecedores
void visualizarListaFornecedor() {
    printf("\n LISTA DE FORNECEDORES\n");
    for (int i = 0; i < quantidadeForn; i++) {
        printf("ID: %d\n", fornecedores[i].id);
        printf("Nome: %s\n", fornecedores[i].nome);
        printf("CNPJ: %lld\n", fornecedores[i].cnpj);
        printf("Endereço: %s\n", fornecedores[i].endereco);
        printf("-----------------------------\n");
    }
}


// Função para cadastrar novos fornecedores
void cadastrarFornecedor() {
    Fornecedor novo_fornecedor;
    novo_fornecedor.id = quantidadeForn > 0 ? fornecedores[quantidadeForn - 1].id + 1 : 1;

    printf("Digite o nome do fornecedor:\n> ");
    scanf(" %49[^\n]", novo_fornecedor.nome);

    printf("Digite o CNPJ do fornecedor:\n> ");
    scanf("%lld", &novo_fornecedor.cnpj);

    printf("Digite o endereço do fornecedor:\n> ");
    scanf(" %99[^\n]", novo_fornecedor.endereco);

    // Adiciona o novo fornecedor ao array
    fornecedores[quantidadeForn] = novo_fornecedor;
    quantidadeForn++;

    // Salva os fornecedores no arquivo CSV
    salvar_forn(fornecedores, quantidadeForn);

    printf("\nFornecedor cadastrado com sucesso!\n\n");
}


void removerFornecedor() {
    printf("\nLISTA DE FORNECEDORES:\n");
    visualizarListaFornecedor();  // Exibe a lista de fornecedores antes da remoção

    int id;
    printf("\nDigite o ID do fornecedor que deseja remover: ");
    scanf("%d", &id);

    for (int i = 0; i < quantidadeForn; i++) {
        if (fornecedores[i].id == id) {
            for (int j = i; j < quantidadeForn - 1; j++) {
                fornecedores[j] = fornecedores[j + 1];
            }
            quantidadeForn--;
            salvar_forn(fornecedores, quantidadeForn);
            printf("Fornecedor removido com sucesso!\n");
            return;
        }
    }
    printf("Fornecedor não encontrado.\n");
}
void alterarFornecedor() {
    int id;
    visualizarListaFornecedor();

    for (int j = 0; j < quantidadeForn; j++) {
        printf(" %d - Nome: %s | CNPJ: %lld | Endereço: %s\n",
        fornecedores[j].id, fornecedores[j].nome, fornecedores[j].cnpj, fornecedores[j].endereco);
    }
    printf("\t-----------------------------\t\n");

    // Solicita o ID do fornecedor que o usuário deseja alterar
    printf("\nDigite o ID do fornecedor que deseja alterar: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    // Verifica se o ID do fornecedor é válido
    int index = -1;
    for (int i = 0; i < quantidadeForn; i++) {
        if (fornecedores[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nID inválido!\n");
        return;
    }

    Fornecedor *fornecedor = &fornecedores[index]; // Pega o fornecedor correspondente

    // Modifica os dados do fornecedor
    printf("Digite o novo nome do fornecedor (ou pressione X para manter o atual): ");
    char novo_nome[50];
    scanf(" %[^\n]", novo_nome);
    if (strcmp(novo_nome, "X") != 0) {
        strcpy(fornecedor->nome, novo_nome);
    }

    printf("Digite o novo CNPJ (ou pressione X para manter o atual): ");
    char novo_cnpj[20];
    scanf(" %[^\n]", novo_cnpj);
    if (strcmp(novo_cnpj, "X") != 0) {
        fornecedor->cnpj = atoi(novo_cnpj);
    }

    printf("Digite o novo endereço (ou digite X para manter o atual): ");
    char novo_endereco[100];
    scanf(" %[^\n]", novo_endereco);
    if (strcmp(novo_endereco, "") != 0) {
        strcpy(fornecedor->endereco, novo_endereco);
    }
    printf("Fornecedor alterado com sucesso!\n");

    // Salva as alterações no arquivo CSV
    salvar_forn(fornecedores, quantidadeForn);
}



// Função para a rotina de compra/cadastro
void rotinaCadastroFornecedor() {
    while (1) {
        printf("Escolha uma opção:\n");
        printf("1 - Registrar novos fornecedores\n");
        printf("2 - Visualizar fornecedores\n");
        printf("3 - Alterar fornecedor\n");
        printf("4 - Remover fornecedor\n");
        printf("0 - Retornar ao menu anterior\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarFornecedor();
                break;
            case 2:
                visualizarListaFornecedor();
                break;
            case 3:
                alterarFornecedor();
                break;
            case 4:
                removerFornecedor();
                break;
            case 0:
                return;
            default:
                printf("Opção não encontrada, digite novamente.\n");
        }
    }
}
