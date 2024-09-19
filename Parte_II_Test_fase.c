#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

typedef struct {
    int CPF;
    char Nome[50];
    char Profissao[30];
} Info;

typedef struct t_no {
    Info info;
    struct t_no *esq;
    struct t_no *dir;
} No;

No *arvoreCPF = NULL;
No *arvoreNome = NULL;

bool verificaComando(char *entrada) {
    if (entrada[0] != 'i' && entrada[0] != 'r') {
        return false;
    }

    if (entrada[1] != ' ') {
        return false;
    }

    if (entrada[2] == '\0') {
        return false;
    }

    char nome[50];
    int cpf;
    char profissao[30];

    // Tente ler três argumentos usando o formato esperado
    if (sscanf(entrada + 2, "%49s %d %29s", nome, &cpf, profissao) == 3) {
        return true;
    }

    return false;
}

No *inserirFuncionario(No *raiz, Info *info) {
    if (raiz == NULL) {
        No *novo = (No *)malloc(sizeof(No));
        if (novo == NULL) {
            fprintf(stderr, "Falha na alocação de memória\n");
            exit(EXIT_FAILURE);
        }
        novo->info = *info;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcasecmp(info->Nome, raiz->info.Nome) < 0)
        raiz->esq = inserirFuncionario(raiz->esq, info);
    else if (strcasecmp(info->Nome, raiz->info.Nome) > 0)
        raiz->dir = inserirFuncionario(raiz->dir, info);
    return raiz;
}

/*Start remover com ambos*/
No* minValueNodeNome(No* node) {
   No* current = node;
   while (current && current->esq != NULL)
       current = current->esq;
   return current;
}

No* minValueNodeCPF(No* node) {
   No* current = node;
   while (current && current->esq != NULL)
       current = current->esq;
   return current;
}

No* removeFuncionario(No* raiz, char* chave, int tipo) {
   if (raiz == NULL) {
       return raiz;
   }
   // Chave a ser removida é menor que a chave da raiz
   if (tipo == 1 && strcasecmp(chave, raiz->info.Nome) < 0) {
       raiz->esq = removeFuncionario(raiz->esq, chave, tipo);
   }
   else if (tipo == 2 && atoi(chave) < raiz->info.CPF) {
       raiz->esq = removeFuncionario(raiz->esq, chave, tipo);
   }
   // Chave a ser removida é maior que a chave da raiz
   else if (tipo == 1 && strcasecmp(chave, raiz->info.Nome) > 0) {
       raiz->dir = removeFuncionario(raiz->dir, chave, tipo);
   }
   else if (tipo == 2 && atoi(chave) > raiz->info.CPF) {
       raiz->dir = removeFuncionario(raiz->dir, chave, tipo);
   }
   // Chave a ser removida é igual à chave da raiz
   else {
       // Nó com um ou nenhum filho
       if (raiz->esq == NULL) {
           No* temp = raiz->dir;
           free(raiz);
           return temp;
       }
       else if (raiz->dir == NULL) {
           No* temp = raiz->esq;
           free(raiz);
           return temp;
       }
       // Nó com dois filhos
       No* temp;
       if (tipo == 1) {
           temp = minValueNodeNome(raiz->dir);
           raiz->info = temp->info;
           raiz->dir = removeFuncionario(raiz->dir, temp->info.Nome, tipo);
       }
       else {
           temp = minValueNodeCPF(raiz->dir);
           raiz->info = temp->info;
           char bun[12];
           sprintf(bun,"%d", temp->info.CPF);
           raiz->dir = removeFuncionario(raiz->dir, bun, tipo);
       }
   }
   return raiz;
}
No* buscarFuncionario(No* raiz, char* chave, int tipo) {
    if (raiz == NULL) {
        return NULL;
    }
    if (tipo == 1 && strcasecmp(chave, raiz->info.Nome) == 0) {
        return raiz;
    } else if (tipo == 2 && atoi(chave) == raiz->info.CPF) {
        return raiz;
    } else if (tipo == 1 && strcasecmp(chave, raiz->info.Nome) < 0) {
        return buscarFuncionario(raiz->esq, chave, tipo);
    } else if (tipo == 2 && atoi(chave) < raiz->info.CPF) {
        return buscarFuncionario(raiz->esq, chave, tipo);
    } else if (tipo == 1 && strcasecmp(chave, raiz->info.Nome) > 0) {
        return buscarFuncionario(raiz->dir, chave, tipo);
    } else if (tipo == 2 && atoi(chave) > raiz->info.CPF) {
        return buscarFuncionario(raiz->dir, chave, tipo);
    }
    return NULL;
}

/*End remover com ambos*/


void liberarArvore(No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

void liberarMemoria() {
    liberarArvore(arvoreNome);
    liberarArvore(arvoreCPF);
}

void listarFuncionarios(No *raiz_1, No *raiz_2) {
    if (raiz_1 && raiz_2 != NULL) {
        listarFuncionarios(raiz_1->esq,raiz_2->esq);
        printf("ArvoreNome Nome: %s\tCPF: %d\tProfissão: %s\n", raiz_1->info.Nome, raiz_1->info.CPF, raiz_1->info.Profissao);
        printf("ArvoreCPF Nome: %s\tCPF: %d\tProfissão: %s\n", raiz_2->info.Nome, raiz_2->info.CPF, raiz_2->info.Profissao);
        listarFuncionarios(raiz_1->dir, raiz_2->dir);
    }
}

int countNodes(No *root) {
    if (root == NULL)
        return 0;
    else
        return (countNodes(root->esq) + countNodes(root->dir) + 1);
}

int main() {
    
    setlocale(LC_ALL, "Portuguese");
    char entrada[100];

    //inserir dados testes
    Info func1 = {12345678, "lucas", "cantor"};
    Info func2 = {87654321, "minato", "Programador"};
    arvoreNome = inserirFuncionario(arvoreNome, &func2);
    arvoreCPF = inserirFuncionario(arvoreCPF, &func2);
    arvoreNome = inserirFuncionario(arvoreNome, &func1);
    arvoreCPF = inserirFuncionario(arvoreCPF, &func1);

    printf("********************Legenda**********************\n");
    printf("*    'i' para introduzir novos FUNCIONÁRIOS     *\n");
    printf("*    'r n' remover pelo NOME                    *\n");
    printf("*    'r c' remover pelo CPF                     *\n");
    printf("*    'b n' buscar pelo NOME                     *\n");
    printf("*    'b c' buscar pelo CPF                      *\n");
    printf("*    'l' para listar todos os funcionários      *\n");
    printf("*    's' para sair do programa                  *\n");
    printf("*************************************************\n");

    while (1) {
        printf("\nDigite um comando: ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if (entrada[0] == 's') {
            break;
        } else if (verificaComando(entrada)) {
            char nome[50];
            int cpf;
            char cpf_buffer[12];
            char profissao[30];

            // Utilize sscanf diretamente para ler os três argumentos
            sscanf(entrada + 2, "%49s %d %29s", nome, &cpf, profissao);
            sprintf(cpf_buffer, "%d", cpf);

            Info novoFuncionario;
            novoFuncionario.CPF = cpf;
            strcpy(novoFuncionario.Nome, nome);
            strcpy(novoFuncionario.Profissao, profissao);

            arvoreNome = inserirFuncionario(arvoreNome, &novoFuncionario);
            arvoreCPF = inserirFuncionario(arvoreCPF, &novoFuncionario);

            printf("Funcionario adicionado com sucesso!\n");
        } else if (entrada[0] == 'r') {
            char chave[50];
            int tipo;

            if (entrada[2] == 'n') {
                sscanf(entrada + 4, "%49s", chave);
                tipo = 1; // 1 para nome
            } else if (entrada[2] == 'c') {
                sscanf(entrada + 4, "%49s", chave);
                tipo = 2; // 2 para CPF
            } else {
                printf("Comando invalido! Tente novamente.\n");
                continue;
            }

            arvoreNome = removeFuncionario(arvoreNome, chave, tipo);
            arvoreCPF = removeFuncionario(arvoreCPF, chave, tipo);

            printf("Funcionario removido com sucesso!\n");
        
        }else if (entrada[0] == 'b') {
            char chave[50];
            int tipo;

            if (entrada[2] == 'n') {
                sscanf(entrada + 4, "%49s", chave);
                tipo = 1; // 1 para nome
            } else if (entrada[2] == 'c') {
                sscanf(entrada + 4, "%49s", chave);
                tipo = 2; // 2 para CPF
            } else {
                printf("Comando inválido! Tente novamente.\n");
                continue;
            }

            No* resultado = buscarFuncionario(arvoreNome, chave, tipo);
            if (resultado != NULL) {
                printf("Funcionário encontrado: %s\tCPF: %d\tProfissão: %s\n", resultado->info.Nome, resultado->info.CPF, resultado->info.Profissao);
            } else {
                printf("Funcionário não encontrado!\n");
            }
        }
 
        else if (strcmp(entrada, "l") == 0) {
            listarFuncionarios(arvoreNome, arvoreCPF);
            printf("quantidade de nó na arvore nome é: %d\n", countNodes(arvoreNome));
            printf("quantidade de nó na arvore CPF é: %d\n", countNodes(arvoreCPF));
        } else {
            printf("Comando inválido! Tente novamente.\n");
        }
    }

    liberarMemoria();

    return 0;
}