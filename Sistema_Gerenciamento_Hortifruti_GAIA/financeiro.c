#include <stdio.h>
#include "financeiro.h"

int financeiro;

void rotinaFinanceiro(int id){
    int idUser = -1;
    idUser = id;
    while (1) {
        printf("Escolha uma opção:\n");
        printf("1- Emitir relatório financeiro\n");
        printf("2- Realizar pagamento\n");
        printf("3- Retornar ao menu anterior\n");
        scanf("%d", &financeiro);

        if (financeiro == 1) {
            printf("Digite o mês e ano de referência (em desenvolvimento)...\n");
            // Implementar lógica para emitir relatório financeiro
        } else if (financeiro == 2) {
            printf("1- Pagamento de funcionário\n");
            printf("2- Pagamento de fornecedor\n");
            printf("3- Despesas (Aluguel, luz, água, etc)\n");
            printf("4- Pagar clientes (Reembolso/Devolução)\n");
            printf("5- Retornar ao menu principal\n");
            scanf("%d", &financeiro);

            if (financeiro >= 1 && financeiro <= 4) {
                printf("Escolha a opção %d (em desenvolvimento)...\n", financeiro);
                // Implementar lógica para realizar pagamento
            } else if (financeiro == 5) {
                break; // Retorna ao menu anterior
            } else {
                printf("Opção não encontrada, digite novamente.\n");
            }
        } else if (financeiro == 3) {
            break; // Retorna ao menu anterior
        } else {
            printf("Opção não encontrada, digite novamente.\n");
        }
    }
}
