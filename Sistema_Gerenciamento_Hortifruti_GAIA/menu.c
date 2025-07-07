#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "compracadastro.h"
#include "vendas.h"
#include "financeiro.h"
#include "administrativo.h"

// Função para limpar a tela
void limpar_tela() {
    system("clear || cls");
}

// Função para exibir o layout inicial
void exibir_layout() {
    limpar_tela();
}

// Função principal
int menu(int id) {
    setlocale(LC_ALL, "Portuguese");

    int idUser = -1;

    if(id > -1){
        idUser = id;
        printf("IDUSER: %d\n", idUser);
    }
    int opcao, opcaovenda, opcaocc, financeiro, financeiro1;

    while (1) {
        exibir_layout();
        printf("Bem-vindo ao menu de seleção de módulo! Digite o número do módulo que você deseja acessar:\n");
        printf("1 - Vendas\n");
        printf("2 - Compra / Cadastro\n");
        printf("3 - Financeiro\n");
        printf("4 - Administrativo\n");
        printf("0 - Sair\n");
        scanf("%d", &opcao);

        switch (opcao){
        case 1:
            limpar_tela();
            rotinaVendas(idUser);
            break;
        case 2:
            limpar_tela();
            carregarProdutos();
            rotinaCompraCadastro(idUser);
            break;
        case 3:
            limpar_tela();
            rotinaFinanceiro(idUser);
            break;
        case 4:
            limpar_tela();
            rotinaAdministrativo(idUser);
        case 0:
            limpar_tela();
            printf("\nSaindo do sistema... Até mais!\n");
            getchar();
            getchar();
            limpar_tela();
            main();
        default:
            printf("Opção inválida, tente novamente.\n");
        }
    }
    return 0;
}
