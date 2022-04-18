#include <stdio.h> 
#include <ncurses.h> 
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include <stdlib.h>
#include "gamelogic.h"
#include "dicionario.h"

int ROW, COL; 
const int RUNNING = 0;
const int VICTORY = 1; 
const int EXIT = 2; 

char tabuleiro[6][4], **palavras_validas, **dicionario, **fields; 

typedef struct _cordenadas{
    int x;
    int y;
} coordenada;

coordenada posicao[18]; 

void print_t(char t[6][4], int r, int c) {
    coordenada pos_letra;
    int indice = 0;
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 3; j++) {
            mvprintw((r/2) - 5 + i * 2, (c/4) - 5 + 4 * j, "(");
            attron(A_UNDERLINE | A_BOLD); 
            printw("%c", toupper(t[i][j]));
            pos_letra.y = (r/2) - 5 + i * 2;
            pos_letra.x = (c/4) - 4 + 4 * j;
            posicao[indice++] = pos_letra;
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
    for(int i = 0; i < 15; ++i) {
        mvprintw((r - 30)/2 + i * 2, c/2, "%d.", i + 1); 
        for(int j = 0; j < 10; ++j) {
            attron(A_UNDERLINE); 
            printw("%c", fields[i][j]); 
            attroff(A_UNDERLINE);
        } 
    }
    for(int i = 0; i < 15; ++i) {
        mvprintw((r - 30)/2 + i * 2, 3 * (c/4), "%d.", i + 16); 
        for(int j = 0; j < 10; ++j) {
            attron(A_UNDERLINE); 
            printw("%c", fields[i + 15][j]); 
            attroff(A_UNDERLINE); 
        }
    } 
} 

void alloc() {
    dicionario = (char **) calloc(110000, sizeof(char *)); 
    for(int i = 0; i < 110000; ++i)
        dicionario[i] = (char *) calloc(10, sizeof(char)); 

    palavras_validas = (char **) calloc(100, sizeof(char *));
    for(int i = 0; i < 100; ++i) 
        palavras_validas[i] = (char *) calloc(10, sizeof(char));

    fields = (char **) calloc(30, sizeof(char *)); 
    for(int i = 0; i < 30; ++i) {
        fields[i] = (char *) calloc(10, sizeof(char)); 
        memset(fields[i], ' ', 10); 
    } 
}

void free_alloc() {
    for(int i = 0; i < 110000; ++i)
        free(dicionario[i]);
    free(dicionario);

    for(int i = 0; i < 100; ++i)
        free(palavras_validas[i]);
    free(palavras_validas);

    for(int i = 0; i < 30; ++i)
        free(fields[i]); 
    free(fields);
}  

void init() {
    initscr(); 

    getmaxyx(stdscr, ROW, COL);

    srand(time(NULL));

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    noecho(); 
    keypad(stdscr, TRUE); 

    alloc(); 

    mvprintw(ROW / 2, COL / 2, "HEY"); 
    curs_set(0); 
    refresh();  
}


void close() {
    getch(); 

    free_alloc(); 

    endwin(); 
} 

int main() {
    int dicionario_tamanho = 0, palavras_validas_qnt; 
    int estado = RUNNING; 

    init();

    int err = getDictionary("./dicionario.txt", dicionario, &dicionario_tamanho);

    if(err) close(); 

    do { 
        createTable(tabuleiro); 
        palavras_validas_qnt = getAllValidWords(palavras_validas, dicionario, tabuleiro, dicionario_tamanho);
    } while(palavras_validas_qnt < 30); 

    clear(); 
    curs_set(1); 
    
    while(estado != VICTORY && estado != EXIT) {
        print_t(tabuleiro, ROW, COL);
        print_fields(ROW, COL);

        char c = getch(); 

        if(c == 27) estado = EXIT; 

        refresh(); 
    }

    close(); 

    return 0;  
}
