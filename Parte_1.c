#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// DefiniÃ§Ã£o da estrutura do nó da árvore binária
struct TreeNode {
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Função para criar um novo n na árvore
struct TreeNode* createNode(char value) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// FunÃÂ§ÃÂ£o para criar uma Ã¡rvore binÃ¡ria a partir de uma sequÃªnncia de caracteres
struct TreeNode* createBinaryTree(const char* sequence, int* index) {
    if (sequence[*index] == '\0' || sequence[*index] == '.') {
        (*index)++;  // Avance para o prÃ³ximo caractere na sequÃ©ncia
        return NULL;
    }

    struct TreeNode* newNode = createNode(sequence[*index]);
    (*index)++;  // Avance para o prÃÂ³ximo caractere na sequÃÂªncia
    newNode->left = createBinaryTree(sequence, index);
    newNode->right = createBinaryTree(sequence, index);

    return newNode;
}

// Algoritmo de caminhamento prÃÂ©-fixo (preorder)
void preorderTraversal(struct TreeNode* root) {
    if (root) {
        printf("%c", root->data);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

// Algoritmo de caminhamento em ordem (inorder)
void inorderTraversal(struct TreeNode* root) {
    if (root) {
        inorderTraversal(root->left);
        printf("%c", root->data);
        inorderTraversal(root->right);
    }
}

// Algoritmo de caminhamento pÃÂ³s-fixo (postorder)
void postorderTraversal(struct TreeNode* root) {
    if (root) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%c", root->data);
    }
}

int main() {
     setlocale(LC_ALL, "Portuguese");
    const char* sequence = "ABD..EH...CFI..J..G.K..";
    int index = 0;

    struct TreeNode* root = createBinaryTree(sequence, &index);

    printf("Caminhamento Pré-Fixo: ");
    preorderTraversal(root);
    printf("\n");

    printf("Caminhamento em Ordem: ");
    inorderTraversal(root);
    printf("\n");

    printf("Caminhamento Pré-Fixo: ");
    postorderTraversal(root);
    printf("\n");

    return 0;
}