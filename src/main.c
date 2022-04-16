#include <stdio.h> 
#include <ncurses.h> 
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include <stdlib.h>
#include "gamelogic.h"
#include "dicionario.h"

char tabuleiro [6][4], **validWords; //= {{"rsi"}, {"epn"}, {"ega"}, {"idj"}, {"aoa"}, {"dir"}};

typedef struct _cordenadas{
    int x;
    int y;
}coordenada;
coordenada posicao[18]; 

void print_t(char t[6][4], int r, int c) {
    coordenada posletra;
    int indice = 0;
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 3; j++) {
            mvprintw((r/2) - 5 + i * 2, (c/4) - 5 + 4 * j, "(");
            attron(A_UNDERLINE | A_BOLD); 
            printw("%c", toupper(t[i][j]));
            posletra.y = (r/2) - 5 + i * 2;
            posletra.x = (c/4) - 4 + 4 * j;
            posicao[indice++] = posletra;
            attroff(A_UNDERLINE | A_BOLD); 
            printw(")"); 
            if(j != 2) printw("-"); 
        }
       
       if(i != 5) {
           mvprintw((r/2) - 4 + i * 2, c/4 - 4, "| X | X |");
       }
   }
}

void print_fields(int r, int c) {
    for(int i = 0; i < 15; ++i)
        mvprintw((r - 30)/2 + i * 2, c/2, "%d.", i + 1); 
    for(int i = 0; i < 15; ++i)
        mvprintw((r - 30)/2 + i * 2, 3 * (c/4), "%d.", i + 16); 
} 

int main() {
    int row, col, dictionaryLen = 0, validWordsn; 
    char **dictionaryWords;

    srand(time(NULL));

    initscr(); 

    start_color(); 
    init_pair(1, COLOR_RED, COLOR_BLACK);

    raw(); 
    noecho();

    getmaxyx(stdscr,row,col);

    if(!has_colors()) {
        endwin(); 
        printf("Your terminal does not support color\n");
        return -1; 
    }

    dictionaryWords = (char **) calloc(110000, sizeof(char *)); 
    for(int i = 0; i < 110000; ++i)
        dictionaryWords[i] = (char *) calloc(10, sizeof(char)); 

    validWords = (char **) calloc(100, sizeof(char *));
    for(int i = 0; i < 100; ++i) 
        validWords[i] = (char *) calloc(10, sizeof(char));

    //dictionary = fopen("dicionarioFinal.txt", "rt"); 

    // if(dictionary == NULL) {
    //     printf("Erro na abertura do dicionário\n"); 
    //     return -1;
    // } 
    // while(fscanf(dictionary, "%s", dictionaryWords[dictionaryLen]) != EOF)
    //     dictionaryLen++; 

    int err = getDictionary("../dicionario.txt",dictionaryWords, &dictionaryLen);

    if(err){
        for(int i = 0; i < 110000; ++i)
            free(dictionaryWords[i]);
        free(dictionaryWords);

        for(int i = 0; i < 100; ++i)
            free(validWords[i]);
        free(validWords);

        return -1; 
    }

    do { 
        for(int i = 0; i < 100; ++i) {
            for(int j = 0; j < 10; ++j)
                validWords[i][j] = '\0'; 
        } 
        createTable(tabuleiro); 
        validWordsn = getAllValidWords(validWords, dictionaryWords, tabuleiro, dictionaryLen);
    } while(validWordsn < 30); 

    print_t(tabuleiro, row, col);
    print_fields(row, col);


    getch();

    endwin(); 

for(int i = 0; i < validWordsn; ++i) 
    printf("%s\n", validWords[i]);
// for(int x = 0; x < 18; x++){
//     printf("y = %d x = %d\n", posicao[x].y, posicao[x].x);
// }

    for(int i = 0; i < 110000; ++i)
        free(dictionaryWords[i]);
    free(dictionaryWords);

    for(int i = 0; i < 100; ++i)
        free(validWords[i]);
    free(validWords);
    
    return 0;  
}
