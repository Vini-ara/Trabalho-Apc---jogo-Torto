#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

int main() {
    FILE* inp;
    FILE* out; 

    char ** dictionary;

    dictionary = (char **) calloc(1000000, sizeof(char *)); 

    for(int i = 0; i < 1000000; ++i) 
        dictionary[i] = (char *) calloc(50, sizeof(char)); 

    inp = fopen("dicionario.txt", "rt"); 

    if(inp == NULL) {
        printf("Incapaz de abrir arquivo de entrada\n"); 
        return -1;
    } 

    out = fopen("dicionarioFinal.txt", "wt"); 

    if(out == NULL) {
        fclose(inp); 
        printf("Incapaz de abrir arquivo de saida\n"); 
        return -1; 
    } 
    
    int index = 0; 

    while(fscanf(inp, "%s", dictionary[index]) != EOF) {
        int len = strlen(dictionary[index]);
        char word[len]; 

        if(len > 9 || len < 4) continue; 
        
        for(int i = 0; i < len; ++i) {
            if(word[i] == '-') 
            word[i] = tolower(dictionary[index][i]);
        }

        word[len] = '\0'; 

        fprintf(out, "%s\n", word); 
    } 

    fclose(inp); 
    fclose(out); 

    return 0; 
} 
