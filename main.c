#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

_Bool isvogal(char c) {
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') 
        return 1; 
    
    return 0; 
} 

_Bool isnear(char* reference, char* tested) {
    if(abs(reference - tested) == 4 || abs(reference - tested) == 1 || abs(reference - tested) == 5 || abs(reference - tested) == 3) 
        return 1; 

    return 0; 
} 

_Bool iscombinable(char* word[10], int len, char nextChar) {
    int vogalSum = 0, consonantSum = 0; 

    for(int i = 0; i < len; ++i) {
        if(isvogal(*word[i])) {
            vogalSum++; 
            consonantSum = 0; 
        } else {
            consonantSum++; 
            vogalSum = 0; 
        } 
    } 

    if((isvogal(nextChar) && vogalSum == 2) || (!isvogal(nextChar) && consonantSum == 2)) 
        return 0; 

    return 1; 
} 

char * getLastChar(char* word[10], int* len) {
    for(int i = 0; i < 10; ++i) { 
        if(word[i + 1] == NULL) { 
            *len = i + 1; 
            return word[i];
        } 
    } 
    
    return NULL; 
} 

void getNextWords(char *words[100000][10], char table[6][4], int index, int* listLen, int* wordLen) {
    char * lastChar = getLastChar(words[index], wordLen); 

    if(lastChar == NULL) return; 

    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(*listLen == 100000 - 100) return;

            char * t = &table[i][j];

            if(isnear(lastChar, t)) { 
                _Bool isPossible = 1; 

                for(int k = 0; k < *wordLen; ++k) 
                    if(words[index][k] == t) isPossible = 0;  

                isPossible = iscombinable(words[index], *wordLen, *t); 

                if(isPossible) {
                    for(int k = 0; k < *wordLen; ++k)
                        words[*listLen][k] = words[index][k];

                    words[*listLen][*wordLen] = &table[i][j]; 
                    *listLen = *listLen + 1; 
                }
            } 
        } 
    }
} 


void getAllWordsPointer(char * out[][10],  char table[][4], int y, int x) { 
    int index = 0, wordLen = 0, listLen = 0;

    out[0][0] = &table[y][x]; 

    while(wordLen < 10) {
        getNextWords(out, table, index, &listLen, &wordLen);

        index++;

        if(listLen >= 100000 - 100)
            break; 
    }

    return;
}

void getAllWordsString(char ** out, char table[][4], int y, int x) {
    char * pointersList[100000][10];

    getAllWordsPointer(pointersList, table, y, x); 
    
    for(int i = 0; i < 99000; ++i) {
        for(int j = 0; j < 10; ++j) {
            out[i][j] = *pointersList[i][j]; 
            if(pointersList[i][j + 1] == NULL) break; 
        } 
        if(pointersList[i + 1][0] == NULL) break; 
    } 
}

int stringBinarySearch(char ** dictionary, int a, int b, char * word) {
    if(b >= a) {
        int mid = a + (b - a) / 2; 

        int lenA = strlen(dictionary[mid]); 
        int lenB = strlen(word); 

        int n = lenA > lenB ? lenB : lenA;

        if(!strncmp(dictionary[mid], word, n))
            return mid; 

        if(strncmp(dictionary[mid], word, n) < 0) {
          printf("mid = %s, word = %s\n", dictionary[mid], word);
            return stringBinarySearch(dictionary, a, mid - 1, word); 
        }

       printf("mid = %s, word = %s\n", dictionary[mid], word);
        return stringBinarySearch(dictionary, mid + 1, b, word); 
    } 

    return -1; 
} 


int main() {
    char table[6][4] = {{"oid"}, {"sla"}, {"tir"}, {"beu"}, {"sde"}, {"ita"}};

    int firstLetterIndex = 0, finalLetterIndex = 0; 

    char **wordsPossibleByPos; 
    char **dictionaryWords;  

    char validWords[30][10]; 

    FILE* dictionary; 

    dictionary = fopen("dicionarioFinal.txt", "rt"); 

    if(dictionary == NULL) {
        printf("Erro na abertura do dicionário\n"); 
        return -1; 
    } 
 
    wordsPossibleByPos  = (char **) calloc(99000, sizeof(char *)); 
    dictionaryWords = (char **) calloc(110000, sizeof(char *)); 

    for(int i = 0; i < 99000; ++i)
        wordsPossibleByPos[i] = (char *) calloc(10, sizeof(char)); 
    
    for(int i = 0; i < 110000; ++i)
        dictionaryWords[i] = (char *) calloc(10, sizeof(char)); 

    getAllWordsString(wordsPossibleByPos, table, 1, 0); 
    
    int index = 0; 
    for(int i = 0; i < 99000; 
    while(fscanf(dictionary, "%s", dictionaryWords[index]) != EOF) {
        if(dictionaryWords[index][0] == table[1][0] && firstLetterIndex == 0)
            firstLetterIndex = index; 
        if(tolower(dictionaryWords[index][0]) != table[1][0] && firstLetterIndex && !finalLetterIndex)
            finalLetterIndex = index; 

        index++; 
    } 
    
 //   for(int i = 0; i < 99000; i++) {
//        int wordIndexDic = stringBinarySearch(dictionaryWords, 0, 100000, wordsPossibleByPos[i]); 

//        printf("index = %d\n", wordIndexDic);  
  //      if(wordIndexDic == -1) continue; 

        printf("%s\n", dictionaryWords[wordIndexDic]); 
  //  } 

    for(int i = 0; i < 99000; ++i) 
        free(wordsPossibleByPos[i]);
    for(int i = 0; i < 110000; ++i)
        free(dictionaryWords[i]); 
    free(wordsPossibleByPos); 
    free(dictionaryWords); 

    return 0; 
} 