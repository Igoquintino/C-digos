#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    char palavra1[50] = {"}na"};
    char palavra2[50] = {"bola"};

    printf("\n\nResultado: %d\n\n", strcmp(palavra1, palavra2));

    return 0;
}