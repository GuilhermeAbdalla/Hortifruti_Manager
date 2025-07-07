#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "menu.h"

#define MAX_USERS 100 //Tamanho máximo inicial do vetor de usuários
#define USERNAME_LENGTH 20 //Tamanho máximo do usuario
#define PASSWORD_LENGTH 20 //Tamanho máximo da senha

typedef struct{
    char name[40];
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
}User;

int verificarCredenciais(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);

User users[MAX_USERS];
int num_fun = 0;

void carregarFuncionarios(){
    FILE *bd_funcionarios = fopen("banco_dados/funcionarios.csv", "r");
    int i = 0;
    if(!bd_funcionarios){
        printf("Não há funcionarios cadastrados! Utilize o login de administrador!\n\n");
        strcpy(users[0].name,"Administrador");
        strcpy(users[0].username,"admin");
        strcpy(users[0].password,"admin123");
        i++;
    }
    else
    {
        char buffer_funcionario[1024];
        char *data;

        fgets(buffer_funcionario, sizeof(buffer_funcionario), bd_funcionarios);

        while(fgets(buffer_funcionario, sizeof(buffer_funcionario), bd_funcionarios)){
            data = strtok(buffer_funcionario, ",");
            strcpy(users[i].name,data);
            data = strtok(NULL,",");

            data = strtok(NULL,",");
            strcpy(users[i].username,data);

            data = strtok(NULL,",");
            strcpy(users[i].password,data);

            i++;
        }
    }
    num_fun = i;
    fclose(bd_funcionarios);
}

int userId = -1;

int main(void){
    setlocale(LC_ALL, "portuguese");

    carregarFuncionarios();

    char line[USERNAME_LENGTH], *result;

    char inputUsername[USERNAME_LENGTH]; //Username digitado pelo usuário
    char inputPassword[PASSWORD_LENGTH]; //Senha digitada pelo usuário
    int loggedIn = 0;

    printf("\t\t\t---------------------------------------------------------------------\n\n");
    printf("\t\t\t\t\tSEJA MUITO BEM VINDO(A) AO SISTEMA\n\n");
    printf("\t\t\t\t\t     ____    _    ___    _    \n");
    printf("\t\t\t\t\t    / ___|  / \\  |_ _|  / \\   \n");
    printf("\t\t\t\t\t   | |  _  / _ \\  | |  / _ \\  \n");
    printf("\t\t\t\t\t   | |_| |/ ___ \\ | | / ___ \\ \n");
    printf("\t\t\t\t\t    \\____/_/   \\_\\___/_/   \\_\\ \n");

    printf("\t\t\t---------------------------------------------------------------------\n");

    printf("\n\tDigite suas credenciais!\n\n");

    printf("\tDigite o nome de usuário: ");
    fgets(inputUsername, sizeof(inputUsername), stdin);
    inputUsername[strcspn(inputUsername, "\n")] = 0;

    printf("\tDigite a senha: ");
    fgets(inputPassword, sizeof(inputPassword), stdin);
    inputPassword[strcspn(inputPassword, "\n")] = 0;

    if(*inputUsername != NULL && *inputPassword != NULL){
        loggedIn = verificarCredenciais(inputUsername, inputPassword);
    }

    if(loggedIn){
        menu(userId);
    }
    else{
        printf("\n\tUsuário e senha incorretos! Digite novamente!\n\n");
        system("pause");
        limpar_tela();
        main();
    }

    system("pause");
    return 0;
}

int verificarCredenciais(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]){
    for(int i = 0; i < num_fun; i++){
        if(strcmp(users[i].username, username) == 0){
            if(strcmp(users[i].password, password) == 0){
                printf("\n\tLogin Verificado! Seja bem-vindo(a) ao sistema %s!\n\n", users[i].name);
                userId = i;
                system("pause");
                return 1;
           }
        }
    }
    return 0;
}
