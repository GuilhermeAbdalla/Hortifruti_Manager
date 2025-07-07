#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "administrativo.h"
#include "menu.h"

#define MAX_USERS 100
#define NOME_MAX 40
#define USERNAME_LENGTH 20 //Tamanho máximo do usuario
#define PASSWORD_LENGTH 20 //Tamanho máximo da senha

typedef struct{
    char pais[NOME_MAX];
    char uf[3];
    char cidade[NOME_MAX];
    char bairro[NOME_MAX];
    char rua[NOME_MAX];
}endereco;

typedef struct{
    char nome[NOME_MAX];
    int idade;
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    endereco end;
}funcionario; funcionario f[MAX_USERS];

int idUser = -1;
int quantidadeFunc = -1;

char *removeSpaces(char *str){
    int i, j = 0;
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] != ' '){
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void lerFuncionarios(funcionario func[MAX_USERS]){
    FILE *bd_funcionarios = fopen("banco_dados/funcionarios.csv", "r");
    FILE *bd_end_funcionarios = fopen("banco_dados/end_funcionarios.csv", "r");
    if(!bd_funcionarios){
        printf("Não há funcionários cadastrados\n");
        return 0;
    }
    char buffer_funcionario[1024]; //buffer para armazenar dados da linha
    char buffer_endereco[1024];
    char *data;

    //Lê as primeira linha para pular
    fgets(buffer_funcionario, sizeof(buffer_funcionario), bd_funcionarios);
    fgets(buffer_endereco, sizeof(buffer_endereco), bd_end_funcionarios);

    int i = 0; //contará a quantidade de funcionários

    //Lê o resto das linhas
    while(fgets(buffer_funcionario, sizeof(buffer_funcionario), bd_funcionarios)){ //Confere se ainda tem dados nas linhas

        //Nome
        data = strtok(buffer_funcionario, ",");
        strcpy(func[i].nome, data);

        //Idade
        data = strtok(NULL, ",");
        func[i].idade = atoi(data);

        //Usuário
        data = strtok(NULL, ",");
        strcpy(func[i].username, data);

        //Senha
        data = strtok(NULL, ",");
        strcpy(func[i].password, data);

        //Lê os endereços
        fgets(buffer_endereco, sizeof(buffer_endereco), bd_end_funcionarios);

        //País
        data = strtok(buffer_endereco, ","); //Lê o primeiro dado até a ","
        strcpy(func[i].end.pais, data);

        //UF
        data = strtok(NULL, ","); //Lê o dado seguinte até a ","
        strcpy(func[i].end.uf, data);

        //Cidade
        data = strtok(NULL, ",");
        strcpy(func[i].end.cidade, data);

        //Bairro
        data = strtok(NULL, ",");
        strcpy(func[i].end.bairro, data);

        //Rua
        data = strtok(NULL, ",");
        strcpy(func[i].end.rua, data);
        i++;
    }
    //fecha o arquivo
    fclose(bd_funcionarios);
    fclose(bd_end_funcionarios);

    quantidadeFunc = i;
}

void removerFuncionario(){
    printf("Seja bem-vindo a aba de remoção de funcionário\n\n");
    lerFuncionarios(f);
    if(quantidadeFunc == 0){
        printf("Não há funcionários cadastrados\n");
        printf("Pressione ENTER para voltar");
        getchar();
        getchar();
        return 0;
    }
    int i = 0, pos;
    for(i = 0; i < quantidadeFunc; i++){
        printf("%d - %s\n",i,f[i].nome);
    }
    printf("\nDigite o ID do funcionário que deseja remover\n");
    scanf("%d", &pos);

    if(pos >= quantidadeFunc){
        printf("=====================================================================\n\n");
        printf("Opção não encontrada, pressione ENTER para retornar");
        printf("\n\n=====================================================================\n\n");
    }else{
        for(i = pos; i < quantidadeFunc - 1; i++){
            f[i] = f[i+1];
        }
        salvarRemocaoFuncionario();
    }
    getchar();
    getchar();
    limpar_tela();
}

void salvarAlteracaoFunc(){
    FILE *bd_funcionarios = fopen("banco_dados/funcionarios.csv", "w");
    FILE *bd_end_funcionarios = fopen("banco_dados/end_funcionarios.csv", "w");

     if(!bd_funcionarios| !bd_end_funcionarios){
        printf("Erro ao abrir arquivo.\n");
        return 0;
    }
    else
    {
        fseek(bd_funcionarios, 0, SEEK_END); //Coloca o ponteiro no final do arquivo;
        int size = ftell(bd_funcionarios); //Define o tamanho da tabela baseado na posição do ponteiro;
        if(size == 0){ //Caso o tamanho da tabela = 0;
            fprintf(bd_funcionarios, "NOME,IDADE,USUARIO,SENHA\n"); //Escreve cabeçalho na tabela;
            fprintf(bd_end_funcionarios, "PAIS,UF,CIDADE,BAIRRO,RUA\n");
        }
    }

    for(int i = 0;i < quantidadeFunc;i++){
                fprintf(bd_funcionarios, "%s,%i,%s,%s,\n",
                f[i].nome,
                f[i].idade,
                f[i].username,
                f[i].password);
        fprintf(bd_end_funcionarios, "%s,%s,%s,%s,%s,\n",
                f[i].end.pais,
                f[i].end.uf,
                f[i].end.cidade,
                f[i].end.bairro,
                f[i].end.rua);
    }

    fclose(bd_funcionarios);
    fclose(bd_end_funcionarios);
}

void alterarFuncionario(){
    lerFuncionarios(f);

    if(quantidadeFunc <= 0){
        printf("==============================\n");
        printf("Pressione ENTER para voltar\n");
        getchar();
        getchar();
        return 0;
    }

    int i;

    printf("=====================================================================\n\n");
    printf("Seja bem-vindo a rotina de alteração de cadastro de Funcionários");
    printf("\n\n=====================================================================\n\n");

    for(i = 0; i < quantidadeFunc; i++){
        printf("ID: %d - %s\n",i,f[i].nome);
        printf("Idade: %d\n", f[i].idade);
        printf("Nome de Usuário: %s\n", f[i].username);
        printf("País: %s\n", f[i].end.pais);
        printf("UF: %s\n", f[i].end.uf);
        printf("Cidade: %s\n", f[i].end.cidade);
        printf("Bairro: %s\n", f[i].end.bairro);
        printf("Rua: %s", f[i].end.rua);
        printf("\n\n==============================\n\n");
    }
    printf("\nSelecione o funcionário que deseja alterar: ");
    int opcao;
    scanf("%d", &opcao);
    limpar_tela();

    if(opcao >= quantidadeFunc){
        printf("Não é possível alterar o funcionário desejado.");
        printf("\n\n=====================================================================\n\n");
        printf("Pressione ENTER para retornar");
        return;
    }

    printf("ID: %d - %s\n",opcao,f[opcao].nome);
    printf("Idade: %d\n", f[opcao].idade);
    printf("Nome de Usuário: %s\n", f[opcao].username);
    printf("País: %s\n", f[opcao].end.pais);
    printf("UF: %s\n", f[opcao].end.uf);
    printf("Cidade: %s\n", f[opcao].end.cidade);
    printf("Bairro: %s\n", f[opcao].end.bairro);
    printf("Rua: %s", f[opcao].end.rua);
    printf("\n\n==============================\n\n");

    printf("1 - NOME\n");
    printf("2 - IDADE\n");
    printf("3 - NOME DE USUÁRIO\n");
    printf("4 - SENHA\n");
    printf("5 - PAÍS\n");
    printf("6 - UF\n");
    printf("7 - CIDADE\n");
    printf("8 - BAIRRO\n");
    printf("9 - RUA\n");
    printf("0 - Cancelar\n");
    printf("\n\nO que deseja alterar?\n");

    int opcao2;
    scanf("%d", &opcao2);
    switch (opcao2){
        case 0:
            printf("Nenhuma Alteração foi feita");
            break;
        case 1:
            printf("Digite o NOME [Max 40 Caracteres] para o qual deseja alterar: ");
            char novo_nome[NOME_MAX];
            scanf(" %39[^\n]", novo_nome);
            printf("\nO NOME passará de %s para %s\n", f[opcao].nome, novo_nome);
            strcpy(f[opcao].nome, novo_nome);
            break;
        case 2:
            printf("Digite a IDADE para o qual deseja alterar: ");
            int novo_idade;
            scanf("%d", &novo_idade);
            printf("\nA IDADE passará de %d para %d\n", f[opcao].idade, novo_idade);
            f[opcao].idade = novo_idade;
            break;
        case 3:
            printf("Digite o USUÁRIO [Max 20 Caracteres] para o qual deseja alterar: ");
            char novo_usuario[USERNAME_LENGTH];
            scanf(" %19[^\n]", novo_usuario);
            printf("\nO Usuário passará de %s para %s\n", f[opcao].username, novo_usuario);
            strcpy(f[opcao].username, novo_usuario);
            break;
        case 4:
            if(opcao == idUser){
                printf("\nDigite a SENHA [Max 20 Caracteres] para a qual deseja alterar: ");
                char novo_senha[PASSWORD_LENGTH];
                scanf(" %19[^\n]", novo_senha);
                strcpy(f[opcao].password, novo_senha);
                printf("\nA senha foi alterada com sucesso!\n");
            }else{
                printf("\nVocê não pode alterar a SENHA de uma conta que não é sua!\n\n");
                printf("Contate o USUÁRIO dessa conta para que ele possa alterar a própria SENHA!\n");
            }
            break;
        case 5:
            printf("Digite o PAíS [Max 40 Caracteres] para o qual deseja alterar: ");
            char novo_pais[NOME_MAX];
            scanf(" %39[^\n]", novo_pais);
            printf("\nO PAÍS passará de %s para %s", f[opcao].end.pais, novo_pais);
            strcpy(f[opcao].end.pais, novo_pais);
            break;
        case 6:
            printf("\nDigite o UF [Max 2 Caracteres] para o qual deseja alterar: ");
            char novo_uf[NOME_MAX];
            scanf(" %2[^\n]", novo_uf);
            printf("\nO UF passará de %s para %s\n", f[opcao].end.uf, novo_uf);
            strcpy(f[opcao].end.uf, novo_uf);
            break;
        case 7:
            printf("Digite a CIDADE [Max 40 Caracteres] para o qual deseja alterar: ");
            char novo_cidade[NOME_MAX];
            scanf(" %39[^\n]", novo_cidade);
            printf("\nA CIDADE passará de %s para %s\n", f[opcao].end.cidade, novo_cidade);
            strcpy(f[opcao].end.cidade, novo_cidade);
            break;
        case 8:
            printf("Digite o BAIRRO [Max 40 Caracteres] para o qual deseja alterar: ");
            char novo_bairro[NOME_MAX];
            scanf(" %39[^\n]", novo_bairro);
            printf("\nO BAIRRO passará de %s para %s\n", f[opcao].end.bairro, novo_bairro);
            strcpy(f[opcao].end.bairro, novo_bairro);
            break;
        case 9:
            printf("Digite a RUA [Max 40 Caracteres] para o qual deseja alterar: ");
            char novo_rua[NOME_MAX];
            scanf(" %39[^\n]", novo_rua);
            printf("\nA RUA passará de %s para %s\n", f[opcao].end.rua, novo_rua);
            strcpy(f[opcao].end.rua, novo_rua);
            break;
        default:
            printf("Operação Cancelada! Essa opção não existe!");
            break;
    }

    salvarAlteracaoFunc();

    printf("\n\nPressione ENTER para continuar");
    getchar();
    getchar();
    limpar_tela();
}

void rotinaFunc(){
    int opcao = 0;
    printf("Escolha uma opção:\n");
    printf("1 - Cadastrar funcionário\n");
    printf("2 - Listar funcionários\n");
    printf("3 - Alterar cadastro de funcionário\n");
    printf("4 - Remover cadastro de funcionário\n");
    printf("0 - Cancelar\n");

    scanf("%d", &opcao);

    switch (opcao){
    case 1:
        cadastrarFuncionario();
        limpar_tela();
        break;
    case 2:
        limpar_tela();
        lerFuncionarios(f);
        listarFuncionarios();
        rotinaFunc();
        break;
    case 3:
        limpar_tela();
        alterarFuncionario();
        break;
    case 4:
        removerFuncionario();
        break;
    default:
        printf("teste");
        limpar_tela();
    }
}

void listarFuncionarios(){
    int i;
    printf("==============================================\n\n");
    printf("Lista de Funcionários Cadastrados");
    printf("\n\n==============================================\n\n");

    for(i = 0; i < quantidadeFunc; i++){
        printf("ID: %d - %s\n",i,f[i].nome);
        printf("Idade: %d\n", f[i].idade);
        printf("Nome de Usuário: %s\n", f[i].username);
        printf("País: %s\n", f[i].end.pais);
        printf("UF: %s\n", f[i].end.uf);
        printf("Cidade: %s\n", f[i].end.cidade);
        printf("Bairro: %s\n", f[i].end.bairro);
        printf("Rua: %s", f[i].end.rua);
        printf("\n\n==============================\n\n");
    }

    printf("Pressione ENTER para voltar");
    getchar();
    getchar();
    limpar_tela();
}

void salvarRemocaoFuncionario(){
    FILE *bd_funcionarios = fopen("banco_dados/funcionarios.csv", "w");
    FILE *bd_end_funcionarios = fopen("banco_dados/end_funcionarios.csv", "w");

    if(!bd_funcionarios || !bd_end_funcionarios){
        printf("Erro ao abrir arquivo.\n");
        return 0;
    }
    fseek(bd_funcionarios, 0, SEEK_END); //Coloca o ponteiro no final do arquivo;
        int size = ftell(bd_funcionarios); //Define o tamanho da tabela baseado na posição do ponteiro;
        if(size == 0){ //Caso o tamanho da tabela = 0;
            fprintf(bd_funcionarios, "NOME,IDADE,USUARIO,SENHA\n"); //Escreve cabeçalho na tabela;
            fprintf(bd_end_funcionarios, "PAIS,UF,CIDADE,BAIRRO,RUA\n");
        }

    quantidadeFunc--;

    for(int i = 0;i < quantidadeFunc;i++){
            fprintf(bd_funcionarios, "%s,%i,%s,%s,\n",
            f[i].nome,
            f[i].idade,
            f[i].username,
            f[i].password);
    fprintf(bd_end_funcionarios, "%s,%s,%s,%s,%s,\n",
            f[i].end.pais,
            f[i].end.uf,
            f[i].end.cidade,
            f[i].end.bairro,
            f[i].end.rua);
    }
    fclose(bd_funcionarios);
    fclose(bd_end_funcionarios);
}

void cadastrarFuncionario(){
    funcionario novo_funcionario;

    printf("Seja Bem-Vindo a rotina de cadastro de funcionário\n\n");
    printf("NOME (MAX 40 CARACTERES): ");
    scanf(" %39[^\n]", novo_funcionario.nome);
    printf("IDADE: ");
    scanf(" %d", &novo_funcionario.idade);
    printf("USERNAME (MAX 20 CARACTERES): ");
    scanf(" %19[^\n]", novo_funcionario.username);
    printf("PASSWORD (MAX 20 CARACTERES): ");
    scanf(" %19[^\n]", novo_funcionario.password);
    printf("PAÍS (MAX 40 CARACTERES): ");
    scanf(" %39[^\n]", novo_funcionario.end.pais);
    printf("UF (MAX 2 CARACTERES): ");
    scanf(" %2[^\n]", novo_funcionario.end.uf);
    printf("CIDADE (MAX 40 CARACTERES): ");
    scanf(" %39[^\n]", novo_funcionario.end.cidade);
    printf("BAIRRO (MAX 40 CARACTERES): ");
    scanf(" %39[^\n]", novo_funcionario.end.bairro);
    printf("RUA (MAX 40 CARACTERES): ");
    scanf(" %39[^\n]", novo_funcionario.end.rua);


    FILE *bd_funcionarios = fopen("banco_dados/funcionarios.csv", "a+");
    FILE *bd_end_funcionarios = fopen("banco_dados/end_funcionarios.csv", "a+");

    if(!bd_funcionarios| !bd_end_funcionarios){
        printf("Erro ao abrir arquivo.\n");
        return 0;
    }
    else
    {
        fseek(bd_funcionarios, 0, SEEK_END); //Coloca o ponteiro no final do arquivo;
        int size = ftell(bd_funcionarios); //Define o tamanho da tabela baseado na posição do ponteiro;
        if(size == 0){ //Caso o tamanho da tabela = 0;
            fprintf(bd_funcionarios, "NOME,IDADE,USUARIO,SENHA\n"); //Escreve cabeçalho na tabela;
            fprintf(bd_end_funcionarios, "PAIS,UF,CIDADE,BAIRRO,RUA\n");
        }
    }
    fprintf(bd_funcionarios, "%s,%i,%s,%s,\n",
            novo_funcionario.nome,
            novo_funcionario.idade,
            novo_funcionario.username,
            novo_funcionario.password);
    fprintf(bd_end_funcionarios, "%s,%s,%s,%s,%s,\n",
            novo_funcionario.end.pais,
            novo_funcionario.end.uf,
            novo_funcionario.end.cidade,
            novo_funcionario.end.bairro,
            novo_funcionario.end.rua);
    fclose(bd_funcionarios);
    fclose(bd_end_funcionarios);
}

void rotinaAdministrativo(int id){
    idUser = id;
    lerFuncionarios(f);
    if(quantidadeFunc > 0){
        printf("Quantidade de funcionários cadastrados: %d\n",quantidadeFunc);
    }

    printf("==============================================\n");

    while(1){
        int opcao = 0;
        printf("Escolha uma opção:\n");
        printf("1 - Funcionários\n"); //Aqui o usuário poderá escolher entre cadastrar, alterar ou remover um funcionario
        printf("2 - Fornecedores\n"); //Aqui o usuário poderá escolher entre cadastrar, alterar ou remover um fornecedor
        printf("0 - Retornar ao menu anterior\n");

        scanf("%d", &opcao);

        switch (opcao){
        case 1:
            limpar_tela();
            rotinaFunc();
            break;
        case 2:
            //cadastrarFornecedor();
            limpar_tela();
            carregarFornecedores();
            rotinaCadastroFornecedor();
            break;
        case 0:
            menu(idUser);
            break;
        default:
            limpar_tela();
            printf("Opção não encontrada, pressione ENTER para retornar\n");
            getchar();
            getchar();
            limpar_tela();
            break;
        }
    }
}
