#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

typedef struct {
    char CPF[12];  // Armazena o CPF como string
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

No *removerPorNome(No *raiz, char *nome);

bool verificaComando(char *entrada) {
    if (entrada[0] != 'i') {
        return false;
    }

    if (entrada[1] != ' ') {
        return false;
    }

    if (entrada[2] == '\0') {
        return false;
    }

    char nome[50];
    char cpf[12];
    char profissao[30];

    // Tente ler três argumentos usando o formato esperado
    if (sscanf(entrada + 2, "%49s %11s %29s", nome, cpf, profissao) == 3) {
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

No *removerPorCPF(No *raiz, char *cpf) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(cpf, raiz->info.CPF) < 0) {
        raiz->esq = removerPorCPF(raiz->esq, cpf);
    } else if (strcmp(cpf, raiz->info.CPF) > 0) {
        raiz->dir = removerPorCPF(raiz->dir, cpf);
    } else {
        if (raiz->esq == NULL) {
            No *temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No *temp = raiz->esq;
            free(raiz);
            return temp;
        }

        No *temp = raiz->dir;
        while (temp->esq != NULL) {
            temp = temp->esq;
        }

        raiz->info = temp->info;
        raiz->dir = removerPorCPF(raiz->dir, temp->info.CPF);

        // Remover o mesmo nó da árvore do nome
        raiz->esq = removerPorNome(raiz->esq, temp->info.Nome);
    }

    return raiz;
}

No *removerPorNome(No *raiz, char *nome) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcasecmp(nome, raiz->info.Nome) < 0) {
        raiz->esq = removerPorNome(raiz->esq, nome);
    } else if (strcasecmp(nome, raiz->info.Nome) > 0) {
        raiz->dir = removerPorNome(raiz->dir, nome);
    } else {
        if (raiz->esq == NULL) {
            No *temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No *temp = raiz->esq;
            free(raiz);
            return temp;
        }

        No *temp = raiz->dir;
        while (temp->esq != NULL) {
            temp = temp->esq;
        }

        raiz->info = temp->info;
        raiz->dir = removerPorNome(raiz->dir, temp->info.Nome);
    }

    return raiz;
}

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

void listarFuncionarios(No *raiz) {
    if (raiz != NULL) {
        listarFuncionarios(raiz->esq);
        printf("Nome: %s\tCPF: %s\tProfissão: %s\n", raiz->info.Nome, raiz->info.CPF, raiz->info.Profissao);
        listarFuncionarios(raiz->dir);
    }
}

int countNodes(No *root) {
    if (root == NULL)
        return 0;
    else
        return (countNodes(root->esq) + countNodes(root->dir) + 1);
}

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void help() {
    printf("*****HELP*********\n");
    printf("* 'i' para introduzir novos FUNCIONÁRIOS     *\n");
    printf("* 'r n' remover pelo NOME                    *\n");
    printf("* 'r c' remover pelo CPF                     *\n");
    printf("* 'b n' buscar pelo NOME                     *\n");
    printf("* 'b c' buscar pelo CPF                      *\n");
    printf("* 'l' para listar todos os funcionários      *\n");
    printf("* 's' para sair do programa                  *\n");
    printf("****************\n");
}

void exemplos() {
    printf("*****Exemplos*********\n");
    printf("* i + <nome_sem_espaco><CPF><Profissao_sem_espaco> *\n");
    printf("* r n + <nome_sem_espaco>                          *\n");
    printf("* r c + <CPF_sem_espaco>                           *\n");
    printf("* b n + <nome_sem_espaco>                          *\n");
    printf("* b c + <CPF_sem_espaco>                           *\n");
    printf("* l + digite somente 'l'                           *\n");
    printf("* s + digite somente 's'                           *\n");
    printf("*****************\n");
}

No *buscarPorNome(No *raiz, char *nome) {
    if (raiz == NULL) {
        return NULL;
    }

    if (strcasecmp(nome, raiz->info.Nome) < 0) {
        return buscarPorNome(raiz->esq, nome);
    } else if (strcasecmp(nome, raiz->info.Nome) > 0) {
        return buscarPorNome(raiz->dir, nome);
    } else {
        return raiz;
    }
}

No *buscarPorCPF(No *raiz, char *cpf) {
    if (raiz == NULL || strcmp(raiz->info.CPF, cpf) == 0) {
        return raiz;
    }

    if (strcmp(cpf, raiz->info.CPF) < 0) {
        return buscarPorCPF(raiz->esq, cpf);
    } else {
        return buscarPorCPF(raiz->dir, cpf);
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    char entrada[100];

    while (1) {
        printf("\nDigite <h> para help e <x> para ver exemplos de como usar os comandos:");
        printf("\ncomandos+>:");

        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if (entrada[0] == 's') {
            break;
        } else if (verificaComando(entrada)) {
            char nome[50];
            char cpf[12];
            char profissao[30];

            if (sscanf(entrada + 2, "%49s %11s %29s", nome, cpf, profissao) == 3) {
                Info novoFuncionario;
                strcpy(novoFuncionario.CPF, cpf);
                strcpy(novoFuncionario.Nome, nome);
                strcpy(novoFuncionario.Profissao, profissao);

                arvoreNome = inserirFuncionario(arvoreNome, &novoFuncionario);
                arvoreCPF = inserirFuncionario(arvoreCPF, &novoFuncionario);

                printf("Funcionário adicionado com sucesso!\n");
                limparTela();

            } else {
                printf("Erro na leitura dos dados. Tente novamente.\n");
            }
        } else if (entrada[0] == 'r' && entrada[2] == 'n') {
            limparTela();
            char nome[50];
            if (sscanf(entrada + 4, "%49s", nome) == 1) {
                arvoreNome = removerPorNome(arvoreNome, nome);
                arvoreCPF = removerPorNome(arvoreCPF, nome);
                printf("Funcionário removido!\n");

            } else {
                printf("Erro na leitura do nome. Tente novamente.\n");
            }
        } else if (entrada[0] == 'r' && entrada[2] == 'c') {
            limparTela();
            char cpf[12];
            if (sscanf(entrada + 4, "%11s", cpf) == 1) {
                arvoreNome = removerPorCPF(arvoreNome, cpf);
                arvoreCPF = removerPorCPF(arvoreCPF, cpf);
                printf("Funcionário removido!\n");

            } else {
                printf("Erro na leitura do CPF. Tente novamente.\n");
            }
        } else if (strcmp(entrada, "l") == 0) {
            limparTela();
            listarFuncionarios(arvoreNome);
            printf("quantidade de nó na arvore nome é: %d\n", countNodes(arvoreNome));
            printf("quantidade de nó na arvore CPF é: %d\n", countNodes(arvoreCPF));

        } else if (strcmp(entrada, "h") == 0) {
            limparTela();
            help();

        } else if (strcmp(entrada, "x") == 0) {
            limparTela();
            exemplos();
        } else if (entrada[0] == 'b' && entrada[2] == 'n') {
            limparTela();
            char nome[50];
            if (sscanf(entrada + 4, "%49s", nome) == 1) {
                No *resultado = buscarPorNome(arvoreNome, nome);
                if (resultado != NULL) {
                    printf("Funcionário encontrado:\n");
                    printf("Nome: %s\tCPF: %s\tProfissão: %s\n", resultado->info.Nome, resultado->info.CPF, resultado->info.Profissao);
                } else {
                    printf("Funcionário não encontrado!\n");
                }

            }
        } else if (entrada[0] == 'b' && entrada[2] == 'c') {
            limparTela();
            char cpf[12];
            if (sscanf(entrada + 4, "%11s", cpf) == 1) {
                No *resultado = buscarPorCPF(arvoreCPF, cpf);
                if (resultado != NULL) {
                    printf("Funcionário encontrado:\n");
                    printf("Nome: %s\tCPF: %s\tProfissão: %s\n", resultado->info.Nome, resultado->info.CPF, resultado->info.Profissao);
                } else {
                    printf("Funcionário não encontrado!\n");
                }
            }
        } else {
            printf("Comando inválido! Tente novamente.\n");
        }
    }

    liberarMemoria();
    return 0;
}