#include <stdlib.h>
#include <string.h> 

_Bool isvogal(char c) {
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') 
        return 1; 
    
    return 0; 
} 

/* checks if the tested char pointer is near to the reference one according to the game logic */ 
_Bool isnear(char* reference, char* tested) {
    if(abs(reference - tested) == 4 || abs(reference - tested) == 1 || abs(reference - tested) == 5 || abs(reference - tested) == 3) 
        return 1; 

    return 0; 
} 

/* checks if adding the next character to the end of the word generates a vogal or consonant sequence too long */ 
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

/* returns the last pointer of the list passed and atualizes the length of the pointers sequence */
char * getLastCharP(char* word[10], int* len) {
    for(int i = 0; i < 10; ++i) { 
        if(word[i + 1] == NULL) { 
            *len = i + 1; 
            return word[i];
        } 
    } 
    
    return NULL; 
} 

/* binary search algorithim to search a word in a word array */ 
int stringBinarySearch(char ** dictionary, int a, int b, char * word) {
    if(b >= a) {
        int mid = a + (b - a) / 2; 

        int lenA = strlen(dictionary[mid]); 
        int lenB = strlen(word); 

        int n = lenA > lenB ? lenB : lenA;

        if(!strcmp(dictionary[mid], word)) 
            return mid;

        if(strncmp(word, dictionary[mid], n) < 0) {
            return stringBinarySearch(dictionary, a, mid - 1, word); 
        }

        return stringBinarySearch(dictionary, mid + 1, b, word); 
    } 

    return -1; 
} 

int stringSort(const void * a, const void * b) {
    const char* aa = (const char*)a;
    const char* bb = (const char*)b;
    return strcmp(aa, bb);
}