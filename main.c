#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main() {
    char * breno = "Coração Oxalá, Clarão da Essência";
    printf("%s\n", breno);

    char* top = (char*)malloc(sizeof(char)*1024); 
    scanf("%[^\n]", top);

    printf("%s\n", top);
}