#ifndef UTIL_H
#define UTIL_H

_Bool isvogal(char c);
_Bool isnear(char* reference, char* tested);
_Bool iscombinable(char* word[10], int len, char nextChar);
char * getLastCharP(char* word[10], int* len);
int stringBinarySearch(char ** dictionary, int a, int b, char * word);
int stringSort(const void *a, const void *b);

#endif