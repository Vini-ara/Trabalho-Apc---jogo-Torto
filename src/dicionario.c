#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 

int getDictionary(char * filename,char ** out, int * outLen) {
  FILE *inp; 

  inp = fopen(filename, "rt");

  if(inp == NULL) {
    printf("Incapaz de abrir arquivo de entrada\n"); 
    return -1;
  }

  char word[256]; 

  while(fscanf(inp, "%s", word) != EOF) {
    int len = strlen(word), isValid = 1;

    if(len > 8 || len < 4) continue; 
        
    for(int i = 0; i < len; ++i) {
      if(word[i] == '-' || word[i] == 'w' || word[i] == 'x' || word[i] == 'y' || word[i] == 'z' || word[i] == 'k') 
        isValid = 0;  
      word[i] = tolower(word[i]);
    }

    word[len] = '\0'; 
    
    if(isValid) {
      sprintf(out[*outLen], "%s", word); 
      *outLen = *outLen + 1;
    }
  } 

  fclose(inp);

  return 0; 
}
