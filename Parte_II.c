#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--> START ESTRUTURA DEFINIDAS <--*/
typedef struct
{
    int CPF;
    char Nome[50];
    char Profissao[30];
} Info;

typedef struct t_no
{
    Info *info;
    struct t_no *esq;
    struct t_no *dir;
} No;
/*--> END ESTRUTURA DEFINIDAS <--*/

No *arvoreCPF;
No *arvoreNome;

/*--> START INSERIR NO <--*/
No *inserir(No *raiz, Info *info)
{
    if (raiz == NULL)
    {
        No *novo = (No *)malloc(sizeof(No));/**/
        novo->info = info;
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(info->Nome, raiz->info->Nome) < 0)
        raiz->esq = inserir(raiz->esq, info);
    else if (strcmp(info->Nome, raiz->info->Nome) > 0)
        raiz->dir = inserir(raiz->dir, info);
    return raiz;
}
/*--> END INSERIR NO <--*/



/*--> START NO VALOR MINIMO <--*/
No *minValueNode(No *node)
{
    No *current = node;
    while (current && current->esq != NULL)/*--> basicamente a funcao para achar o menor valor de no na arvore, que logicamente ta a esquerda <--*/
        current = current->esq;
    return current; /*--> retorna esse valor minimo de 'no' na arvore, que logicamente ta a esquerda, o que esta a mais esquerda <--*/
}
/*--> END NO VALOR MINIMO <--*/



/*--> START DELETAR NO 'COM NOME NESSE CASO' <--*/
No *deleteNode(No *root, Info *info)
{
    if (root == NULL)
        return root;
    if (strcmp(info->Nome, root->info->Nome) < 0)
        root->esq = deleteNode(root->esq, info);
    else if (strcmp(info->Nome, root->info->Nome) > 0)
        root->dir = deleteNode(root->dir, info);
    else
    {
        if (root->esq == NULL)
        {
            No *temp = root->dir;
            free(root);
            return temp;
        }
        else if (root->dir == NULL)
        {
            No *temp = root->esq;
            free(root);
            return temp;
        }
        No *temp = minValueNode(root->dir);
        root->info = temp->info;
        root->dir = deleteNode(root->dir, temp->info);
    }
    return root;
}
/*--> END DELETE NO <--*/

/*--> START DELETAR NO 'COM CPF NESSE CASO' <--*/
No *deleteNodeCpf(No *root, Info *info)
{
    if (root == NULL)
        return root;
    if (info->CPF < root->info->CPF)
        root->esq = deleteNodeCpf(root->esq, info);
    else if (info->CPF > root->info->CPF)
        root->dir = deleteNodeCpf(root->dir, info);
    else
    {
        if (root->esq == NULL)
        {
            No *temp = root->dir;
            free(root);
            return temp;
        }
        else if (root->dir == NULL)
        {
            No *temp = root->esq;
            free(root);
            return temp;
        }
        No *temp = minValueNode(root->dir);
        root->info = temp->info;
        root->dir = deleteNode(root->dir, temp->info);
    }
    return root;
}
/*--> END DELETE NO CPF <--*/


/*--> START BUSCAR 'COM NOME NESSE CASO' <--*/
No *buscar(No *raiz, char *nome)
{
    if (raiz == NULL || strcmp(raiz->info->Nome, nome) == 0) /*--> compara o nome do nó na raiz com o nome a ser pesquisado<--*/
        return raiz;/*-->Se forem iguais, significa que o nó foi encontrado, então a função retorna o nó raiz. <--*/
    if (strcmp(raiz->info->Nome, nome) > 0) /*--> raiz for menor que o nome a ser pesquisado a função chama a si mesma recursivamente<--*/
        return buscar(raiz->esq, nome);
    return buscar(raiz->dir, nome);/*--> com o filho direito da raiz como a nova raiz. <--*/
}
/*--> END BUSCAR 'COM NOME NESSE CASO' <--*/

/*--> START BUSCAR 'COM CPF NESSE CASO' <--*/
No *buscarCpf(No *raiz, int *cpf)
{
    if (raiz == NULL || raiz->info->Nome == cpf)
        return raiz;
    if (raiz->info->CPF > cpf)
        return buscar(raiz->esq, cpf);
    return buscar(raiz->dir, cpf);
}
/*--> END BUSCAR 'COM CPF NESSE CASO' <--*/

/*--> START LIMPAR TELA <--*/
void limparTela()
{
    for (int i = 0; i < 40; i++)
    {
        printf(" \n");
    }
}
/*--> END 'LIMPAR TELA' <--*/

int main()
{
    arvoreCPF = NULL;
    arvoreNome = NULL;

    /*--> START TELA DE LOGIN' <--*/
    while (1)
    {
        printf("1. Inserir funcionário\n");   /*--> 'ta inserindo normalmente, isso nao precisa mudar no seu back' <--*/
        printf("2. Remover funcionário\n");   /*--> 'aqui, como dito acima, o remover estar atrelado ao nome' <--*/
        printf("3. Consultar funcionário\n"); /*--> 'tambem só esta consultado a partir de uma pesquisa por nome na arvore binaria criada' <--*/
        printf("4. Sair\n");
        int opcao;
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
        {
            limparTela();
            Info *info = (Info *)malloc(sizeof(Info));
            printf("Digite o nome do funcionário: ");
            scanf("%s", info->Nome);
            printf("Digite o CPF do funcionário: ");
            scanf("%d", &info->CPF);
            printf("Digite a profissão do funcionário: ");
            scanf("%s", info->Profissao);
            arvoreCPF = inserir(arvoreCPF, info);
            arvoreNome = inserir(arvoreNome, info);
            limparTela();
            break;
        }
        case 2:
        {
            limparTela();
            char nome[50];
            printf("Digite o nome do funcionário: ");
            scanf("%s", nome);
            arvoreCPF = deleteNode(arvoreCPF, buscar(arvoreNome, nome)->info);
            arvoreNome = deleteNode(arvoreNome, buscar(arvoreNome, nome)->info);
            limparTela();
            break;
        }
        case 3:
        {
            limparTela();
            char nome[50];
            printf("Digite o nome do funcionário: ");
            scanf("%s", nome);
            No *no = buscar(arvoreNome, nome);
            if (no != NULL)
            {
                printf("CPF: %d, Profissão: %s\n", no->info->CPF, no->info->Profissao);
            }
            else
            {
                printf("Funcionário não encontrado.\n");
            }
            break;
        }
        case 4:
            system("pause");
            return 0;
        default:
            printf("Opção inválida.\n");
        }
    }
    system("pause");
    return 0;
}