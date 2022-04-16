#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include "util.h"

/* gets all the sequences possible to be formed considering the last char pointer from the given pointer array
    and appends them into a bigger array */ 
void getNextWords(char *words[100000][10], char table[6][4], int index, int* listLen, int* wordLen) {
    char * lastChar = getLastCharP(words[index], wordLen); 

    if(lastChar == NULL) return; 

    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(*listLen == 100000 - 100) return;

            char * t = &table[i][j];

            if(isnear(lastChar, t)) { 
                _Bool isPossible = 1; 

                isPossible = iscombinable(words[index], *wordLen, *t); 

                for(int k = 0; k < *wordLen; ++k) 
                    if(words[index][k] == t) isPossible = 0;  

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

/* generates all the pointer sequences possible for an especific character in the table, util the 
    word length reaches 9 or the array gets full */ 
void getAllWordsPointer(char * out[][10],  char table[][4], int y, int x) { 
    int index = 0, wordLen = 0, listLen = 0;
    
    for(int i = 0; i <= 99000; ++i) {
        for(int j = 0; j <= 10; ++j) 
            out[i][j] = NULL; 
    } 

    out[0][0] = &table[y][x]; 

    while(wordLen < 9) {
        getNextWords(out, table, index, &listLen, &wordLen);

        index++;

        if(listLen >= 100000 - 100)
            break; 
    }
}

/* transforms the an all the possible pointers combinations from the table char especified
    and converts it to a string array, returning the number of strings generated*/ 
int getAllWordsString(char ** out, char table[][4], int y, int x) {
    char * pointersList[100000][10];
    int totalWords = 0; 

    getAllWordsPointer(pointersList, table, y, x); 
    
    for(int i = 0; i < 99000; ++i) {
        for(int j = 0; j < 10; ++j) {
            out[i][j] = *pointersList[i][j]; 
            if(pointersList[i][j + 1] == NULL) break; 
        } 
        totalWords++; 
        if(pointersList[i + 1][0] == NULL) 
            break; 
    } 
    return totalWords; 
}

/* gets all the possible strings given the especific table char, and searches them in the array to chck if they are
    valid in the given dictionary, appending them into an output array */ 
void getValidWords(char ** out, char ** dictionary, char table[][4], int y, int x, int searchEnd, int * validWordsCounter) {
    int validLen = 0; 
    char ** possibleWords;

    possibleWords  = (char **) calloc(99000, sizeof(char *));
    for(int i = 0; i < 99000; ++i)
        possibleWords[i] = (char *) calloc(10, sizeof(char)); 

    int totalWords = getAllWordsString(possibleWords, table, y, x); 

    for(int i = 0; i < totalWords; i++) {
        if(*validWordsCounter == 100) break;

        if(strlen(possibleWords[i]) < 4) continue; 

        int wordIndexDic = stringBinarySearch(dictionary, 0, searchEnd, possibleWords[i]); 

        if(wordIndexDic == -1) continue; 

        strcpy(out[*validWordsCounter], dictionary[wordIndexDic]);

        *validWordsCounter += 1;
    }

    for(int i = 0; i < 99000; ++i) 
        free(possibleWords[i]);
    free(possibleWords); 
} 

/* gets all the valid words of the table, removes the diplicated ones and returns the amount of valid words */ 
int getAllValidWords(char ** validWords, char ** dictionaryWords, char table[][4], int dictionaryLen) {
    int validWordsCounter = 0, repeatedWords = 0;

    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(validWordsCounter == 100) break;
            getValidWords(validWords, dictionaryWords, table, i, j, dictionaryLen, &validWordsCounter); 
        }
    }

    for(int i = 1; i < validWordsCounter; ++i) {
        for(int j = i - 1; j >= 0; --j) {
            if(!strcmp(validWords[i], validWords[j])) {
                memset(validWords[j], '\0', 10); 
                repeatedWords++;
            } 
        } 
    } 

    for(int i = 0; i < validWordsCounter; ++i) {
        if(validWords[i][0] == '\0') {
            for(int j = i + 1; j < validWordsCounter; ++j) {
                if(validWords[j][0] != '\0') {
                    strcpy(validWords[i], validWords[j]); 
                    memset(validWords[j], '\0', 10);
                    break;
                }
            }
        }
    }

    return validWordsCounter - repeatedWords;
}

/* generates the game table */ 
void createTable(char out[][4]) {
    char vogals[] = {'a', 'e', 'i', 'o', 'u'}; 

    for(int i = 0; i < 6; i++) {
        char line[4]; 
        line[3] = '\0'; 

        int vogalCount = 0; 

        for(int j = 0; j < 3; ++j) {
            int vogal; 
            char c; 

            vogal = (rand() % 10) < 5 ? 1 : 0;

            if(vogal && vogalCount < 2) {
                c = vogals[rand() % 4]; 
                vogalCount++; 
            } else {
                do { 
                    c = (rand() % 26) + 97; 
                } while(isvogal(c) || c == out[i - 1][j] || c == 'w' || c == 'x' || c == 'y' || c == 'k' || c == 'z'); 
            } 

            if(!isvogal(c)) {
                int repeated = 0, repeatedSameLine = 0; 
                for(int k = 0; k < i; ++k) {
                    for(int n = 0; n < 3; n++) {
                        if(out[k][n] == c) repeated++; 
                    } 
                } 
                for(int k = 0; k < j; k++) 
                    if(line[k] == c) repeatedSameLine = 1; 

                if(repeated >= 2 || repeatedSameLine) {
                    do { 
                        c = (rand() % 26) + 97; 
                    } while(isvogal(c) || c == out[i - 1][j] || c == 'w' || c == 'x' || c == 'y' || c == 'k' || c == 'z'); 
                }  
            } else {
                int repeated = 0; 
                for(int k = 0; k < i; ++k) {
                    for(int n = 0; n < 3; n++) {
                        if(out[k][n] == c) repeated++; 
                    } 
                } 
                for(int k = 0; k < j; k++) 
                    if(line[k] == c) repeated++; 

                if(repeated > 4)
                    c = vogals[rand() % 4]; 
            } 

            if(j == 2 && !vogalCount)
                c = vogals[rand() % 4];

            line[j] = c; 
        } 

        strcpy(out[i], line); 
    } 
} 