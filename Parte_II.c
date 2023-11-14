#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--> START ESTRUTURA DEFINIDAS <--*/
typedef struct {
    int CPF;
    char Nome[50];
    char Profissao[30];
} Info;

typedef struct t_no {
    Info *info;
    struct t_no *esq;
    struct t_no *dir;
} No;

No *arvoreCPF;
No *arvoreNome;
/*--> END ESTRUTURA DEFINIDAS <--*/


/*--> START INSERIR NO NOME<--*/
No *inserirNome(No *raiz, Info *info) {
    if (raiz == NULL) {
        No *novo = (No *)malloc(sizeof(No)); /**/
        novo->info = info;
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(info->Nome, raiz->info->Nome) < 0)
        raiz->esq = inserirNome(raiz->esq, info);
    else if (strcmp(info->Nome, raiz->info->Nome) > 0)
        raiz->dir = inserirNome(raiz->dir, info);
    return raiz;
}

/*--> START INSERIR NO COM CPF<--*/
No *inserirCpf(No *raiz, Info *info) {
    if (raiz == NULL) {
        No *novoCpf = (No *)malloc(sizeof(No));
        if (novoCpf == NULL) {
            fprintf(stderr, "Falha na alocação de memória\n");
            exit(EXIT_FAILURE);
        }
        novoCpf->info = info;
        novoCpf->esq = novoCpf->dir = NULL;
        return novoCpf;
    }
    if (info->CPF < raiz->info->CPF)
        raiz->esq = inserirCpf(raiz->esq, info);
    else if (info->CPF > raiz->info->CPF)
        raiz->dir = inserirCpf(raiz->dir, info);
    return raiz;
}


/*--> START NO VALOR MINIMO <--*/
No *minValueNode(No *node) {
    No *current = node;
    while (current && current->esq != NULL) /*--> basicamente a funcao para achar o menor valor de no na arvore, que logicamente ta a esquerda <--*/
        current = current->esq;
    return current; /*--> retorna esse valor minimo de 'no' na arvore, que logicamente ta a esquerda, o que esta a mais esquerda <--*/
}
/*--> END NO VALOR MINIMO <--*/


/*--> START DELETAR NO 'COM NOME NESSE CASO' <--*/
No *deleteNodeNome(No *root, Info *info) {
    if (root == NULL)
        return root;
    if (strcmp(info->Nome, root->info->Nome) < 0)
        root->esq = deleteNodeNome(root->esq, info);
    else if (strcmp(info->Nome, root->info->Nome) > 0)
        root->dir = deleteNodeNome(root->dir, info);
    else {
        if (root->esq == NULL) {
            No *temp = root->dir;
            free(root);
            return temp;
        }
        else if (root->dir == NULL) {
            No *temp = root->esq;
            free(root);
            return temp;
        }
        No *temp = minValueNode(root->dir);
        root->info = temp->info;
        root->dir = deleteNodeNome(root->dir, temp->info);
    }
    return root;
}


/*--> START DELETAR NO 'COM CPF NESSE CASO' <--*/
No *deleteNodeCpf(No *root, Info *info){
    if (root == NULL)
        return root;
    if (info->CPF < root->info->CPF)
        root->esq = deleteNodeCpf(root->esq, info);
    else if (info->CPF > root->info->CPF)
        root->dir = deleteNodeCpf(root->dir, info);
    else {
        if (root->esq == NULL) {
            No *temp = root->dir;
            free(root);
            return temp;
        }
        else if (root->dir == NULL) {
            No *temp = root->esq;
            free(root);
            return temp;
        }
        No *temp = minValueNode(root->dir);
        root->info = temp->info;
        root->dir = deleteNodeCpf(root->dir, temp->info);
    }
    return root;
}


/*--> START BUSCAR 'COM NOME NESSE CASO' <--*/
No *buscar(No *raiz, char *nome) {
    if (raiz == NULL || strcmp(raiz->info->Nome, nome) == 0) /*--> compara o nome do nó na raiz com o nome a ser pesquisado<--*/
        return raiz;                                         /*-->Se forem iguais, significa que o nó foi encontrado, então a função retorna o nó raiz. <--*/
    if (strcmp(raiz->info->Nome, nome) > 0)                  /*--> raiz for menor que o nome a ser pesquisado a função chama a si mesma recursivamente<--*/
        return buscar(raiz->esq, nome);
    return buscar(raiz->dir, nome); /*--> com o filho direito da raiz como a nova raiz. <--*/
}


/*--> START BUSCAR 'COM CPF NESSE CASO' <--*/
No *buscarCpf(No *raiz, int cpf) {
    if (raiz == NULL || raiz->info->CPF == cpf)
        return raiz;
    if (raiz->info->CPF > cpf)
        return buscarCpf(raiz->esq, cpf);
    return buscarCpf(raiz->dir, cpf);
}


/*--> START LISTAR <--*/
void exibirFuncionarios(No *raiz, int cont) {
    if (raiz != NULL) {
        exibirFuncionarios(raiz->esq, cont);
        printf("func : %s\t%d\t%s\n", raiz->info->Nome, raiz->info->CPF, raiz->info->Profissao);
        exibirFuncionarios(raiz->dir,cont);
    }
}
/*--> END LISTAR <--*/

int countNodes(No *root) {
    if (root == NULL)
        return 0;
    else
        return (countNodes(root->esq) + countNodes(root->dir) + 1);
}



void freeTree(No *root);
Info *info = NULL;

void freeTree(No *root) {
        if (root == NULL) return;
        freeTree(root->esq);
        freeTree(root->dir);
        free(root->info);
        free(root);
    }


  void freeMemory() {
    freeTree(arvoreCPF);
    freeTree(arvoreNome);
    free(info);  // Free the memory allocated for info
    }

/*--> ************************************************************************************************** <--*/
/*--> ************************************************************************************************** <--*/

/*--> START LIMPAR TELA <--*/
void limparTela() {
    for (int i = 0; i < 40; i++) {
        printf(" \n");
    }
}
/*--> END 'LIMPAR TELA' <--*/


/*--> ************************************************************************************************** <--*/
/*--> ************************************************************************************************** <--*/
int main() {
    /*--> Dados testes <--*/
    // Criação de dados de teste
    Info *teste1 = (Info *)malloc(sizeof(Info));
    teste1->CPF = 123456;
    strcpy(teste1->Nome, "João");
    strcpy(teste1->Profissao, "Engenheiro");

    Info *teste2 = (Info *)malloc(sizeof(Info));
    teste2->CPF = 789012;
    strcpy(teste2->Nome, "Maria");
    strcpy(teste2->Profissao, "Médica");

    Info *teste3 = (Info *)malloc(sizeof(Info));
    teste3->CPF = 345678;
    strcpy(teste3->Nome, "Pedro");
    strcpy(teste3->Profissao, "Professor");

    // Inserção dos dados de teste nas árvores
    arvoreCPF = inserirCpf(arvoreCPF, teste1);
    arvoreNome = inserirNome(arvoreNome, teste1);

    arvoreCPF = inserirCpf(arvoreCPF, teste2);
    arvoreNome = inserirNome(arvoreNome, teste2);

    arvoreCPF = inserirCpf(arvoreCPF, teste3);
    arvoreNome = inserirNome(arvoreNome, teste3);
/*--> End dados testes <--*/
    
    char command[2];
    char name[50];
    char cpf[12];
    char profession[50];

    printf("****************Legenda************************\n");
    printf("*   'i' para introduzir novos FUNCIONARIOS    *\n");
    printf("*   'r n' remover pelo NOME                   *\n");
    printf("*   'r c' remover pelo CPF                    *\n");
    printf("*   'b n' buscar pelo NOME                    *\n");
    printf("*   'b c' buscar pelo CPF                     *\n");
    printf("*   'S' para sair do programa                 *\n");
    printf("***********************************************\n");
    printf(" \n");
    printf("***************Exemplo de Funcionamento********************\n");
    printf("*   i <nome_sem_espacos> <CPF> <profissão_sem_espacos>    *\n");
    printf("*   r n <nome_sem_espacos>                                *\n");
    printf("*   r c <CPF>                                             *\n");
    printf("*   b n <Nome>                                            *\n");
    printf("*   b c <CPF>                                             *\n");
    printf("*   l <listar com Cpf>                                    *\n");
    printf("***********************************************************\n");
    printf(" \n");  

   while (1) {
    printf("Escreva o seu comando\n");
    scanf("%s", command);
    limparTela();
    int count = 0;
    int countCPF = 0;
    switch (command[0]) {
    case 'i':
        printf("add um funcionario: ");
        scanf("%s %s %s", name, cpf, profession);
        info = (Info *)malloc(sizeof(Info));  // Allocate memory for info
        info->CPF = atoi(cpf);
        strcpy(info->Nome, name);
        strcpy(info->Profissao, profession);
        arvoreCPF = inserirCpf(arvoreCPF, info);
        arvoreNome = inserirNome(arvoreNome, info);
        printf("Funcionário adicionado com sucesso!\n");
        limparTela();
        break;

    case 'r':
    printf("remover\n");
    scanf("%s", name);
    if (command[1] == 'n') {
        printf("remover com nome\n");
        Info *infoNome = (Info *)malloc(sizeof(Info));
        strcpy(infoNome->Nome, name);
        arvoreCPF = deleteNodeNome(arvoreCPF, infoNome);
        arvoreNome = deleteNodeNome(arvoreNome, infoNome);
        free(infoNome);
        printf("Funcionário removido com sucesso!\n");
    }
    else if (command[1] == 'c') {
        printf("remover com cpf\n");
        scanf("%s", cpf);
        Info *infoCpf = (Info *)malloc(sizeof(Info));
        infoCpf->CPF = atoi(cpf);
        arvoreCPF = deleteNodeCpf(arvoreCPF, infoCpf);
        arvoreNome = deleteNodeCpf(arvoreNome, infoCpf);
        free(infoCpf);
        printf("Funcionário removido com sucesso!\n");
    }
    break;
    case 'b':
        scanf("%s", name);
        if (command[1] == 'n') {
            printf("buscar com nome\n");
            strcpy(info->Nome, name);
            No *result = buscar(arvoreNome, info->Nome);
            if(result != NULL) {
                printf("Funcionário encontrado: %s, %d, %s\n", result->info->Nome, result->info->CPF, result->info->Profissao);
            } else {
                printf("Funcionário não encontrado!\n");
            }
        }
        else if (command[1] == 'c') {
            printf("buscar com cpf\n");
            scanf("%s", cpf);
            info->CPF = atoi(cpf);
            No *result = buscarCpf(arvoreCPF, info->CPF);
            if(result != NULL) {
                printf("Funcionário encontrado: %s, %d, %s\n", result->info->Nome, result->info->CPF, result->info->Profissao);
            } else {
                printf("Funcionário não encontrado!\n");
            }
        }
        break;

    case 'l':
        printf("Listar funcionários\n");
        printf("\tNome\tCPF\tProfissão\n");
        exibirFuncionarios(arvoreNome, count); count++;
        // exibirFuncionarios(arvoreCPF, countCPF); countCPF++;
        printf("Total de funcionários: %d\n", countNodes(arvoreNome));
        break;


    case 'S':
        freeMemory();
        printf("Programa finalizado com sucesso!\n");
        return 0;

    default:
        printf("Comando inválido!\n");
        break;
    }
}
    return 0;
}

    /*o sistema estar com bug nos casos */
    /*teste os caso antes*/