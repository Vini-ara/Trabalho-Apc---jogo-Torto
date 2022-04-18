#include <stdio.h> 
#include <ncurses.h> 
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include <stdlib.h>
#include "gamelogic.h"
#include "dicionario.h"

int ROW, COL, cont = 0; 
const int RUNNING = 0;
const int VICTORY = 1; 
const int EXIT = 2; 

char tabuleiro[6][4], **palavras_validas, **dicionario, **fields; 

typedef struct _cordenadas{
    int x;
    int y;
    char letra;
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
            pos_letra.letra = t[i][j];
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

void check_click(int x, int y){
    for(int j = 0; j < 18; j++){
        if(posicao[j].x == x && posicao[j].y == y){
            //printw("%c\n", toupper(posicao[j].letra));
            fields[0][cont] = toupper(posicao[j].letra);
            cont = cont < 9 ? cont+1 : 0;
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

    mousemask(BUTTON1_CLICKED, NULL);

    alloc(); 

    attron(A_BOLD | A_BLINK);
    attron(COLOR_PAIR(1));
    mvprintw((ROW * 0.35), (COL / 2)-3, "REGRAS");
    attroff(COLOR_PAIR(1));
    mvprintw((ROW * 0.35) + 2, (COL / 2)-26, "+ Deve-se formar palavras seguindo os mesmos sentidos");
    mvprintw((ROW * 0.35) + 3, (COL / 2)-26, "+ Proibido pular e cruzar letras");
    mvprintw((ROW * 0.35) + 4, (COL / 2)-26, "+ Proibido repetir a mesma letra que foi utilizada no click anterior");
    mvprintw((ROW * 0.35) + 5, (COL / 2)-26, "+ O jogador tem que encontrar as trinta palavras");
    mvprintw((ROW * 0.35) + 6, (COL / 2)-26, "+ Cada palavra tem de quatro a nove letras");    
    attroff(A_BOLD | A_BLINK);

    curs_set(0); 
    refresh();  
}

void press_enter(){
    keypad(stdscr, TRUE);

    attron(A_BOLD | A_BLINK); 
    mvprintw((ROW * 0.35)+10, (COL / 2)-7, "Pressione ENTER");
    attroff(A_BOLD | A_BLINK);

    int ch = getch();
    while(ch != 10){
    }
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

    press_enter();

    clear(); 
    curs_set(1); 
    
    while(estado != VICTORY && estado != EXIT) {
        print_t(tabuleiro, ROW, COL);
        print_fields(ROW, COL);

        char c = getch();
        MEVENT event;

        if(KEY_MOUSE){ 
            if(getmouse(&event) == OK) 
            { 
                if(event.bstate & BUTTON1_CLICKED)
                { 
                    check_click(event.x, event.y); 
                } 
            } 
        } 

        if(c == 27) estado = EXIT; 

        refresh(); 
    }

    close(); 

    return 0;  
}
