#include <stdio.h> 
#include <ncurses.h> 
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include <stdlib.h>
#include "gamelogic.h"
#include "dicionario.h"

int ROW, COL, pos = 0, reload = 0, confirm = 0, pontos = 0, caractere = 0, anteriorx, anteriory, palavras_validas_qnt = 0, gabarit = 0;; 
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
coordenada usadas[18];
char gabarito[10];

void print_t(char t[6][4], int r, int c) {
    int indice = 0;
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 3; j++) {
            posicao[indice].y = (r/2) - 5 + i * 2;
            posicao[indice].x = (c/4) - 4 + 4 * j;
            posicao[indice].letra = t[i][j];

            mvprintw((r/2) - 5 + i * 2, (c/4) - 5 + 4 * j, "(");

            int ja_foi = 0;
            for(int k = 0; k < 18; k++){
                if(usadas[k].x == posicao[indice].x && usadas[k].y == posicao[indice].y && usadas[k].letra == t[i][j]){
                        ja_foi = 1;
                }
            }
            
            attron(A_UNDERLINE | A_BOLD);
            ja_foi == 1 ? attron(COLOR_PAIR(1)) : 0; 
            printw("%c", toupper(t[i][j]));
            attroff(A_UNDERLINE | A_BOLD | COLOR_PAIR(1)); 
        
            printw(")");

            indice++;
            if(j != 2) printw("-");
        }
        if(i != 5) {
            mvprintw((r/2) - 4 + i * 2, c/4 - 4, "| X | X |");
        }
        else{
            attron(A_DIM | COLOR_PAIR(3));
            mvprintw((r/2) + i + 4, (c/7) + 3, "REINICIAR");
            attroff(COLOR_PAIR(3));
            attron(COLOR_PAIR(2));
            mvprintw((r/2) + i + 4, (c/7) + 15, "CONFIRMAR");
            attroff(A_DIM | COLOR_PAIR(2));
        }
   }
}

void print_fields(int r, int c) {
    for(int i = 0; i < 15; ++i) {
        attron(A_BOLD);
        mvprintw((r - 30)/2 + i * 2, c/2, i < 9 ? "%d.  " : "%d. ", i + 1); 
        attroff(A_BOLD);

        pontos > i ? attron(COLOR_PAIR(2)) : 0;

        for(int j = 0; j < 10; ++j) {
            printw("%c", fields[i][j]); 
        }

        attroff(COLOR_PAIR(2)); 
    }

    for(int i = 0; i < 15; ++i) {
        attron(A_BOLD);
        mvprintw((r - 30)/2 + i * 2, (c/2)+(c/4)-6, "%d. ", i + 16);
        attroff(A_BOLD);

        pontos > i + 15 ? attron(COLOR_PAIR(2)) : 0;

        for(int j = 0; j < 10; ++j) {
            printw("%c", fields[i + 15][j]);  
        }

        attroff(COLOR_PAIR(2));
    }
} 

void check_click(int x, int y, int linhas, int colunas){
    if((y == (linhas/2) + 9) && x >= (colunas/7) + 3 && x <= (colunas/7) + 11){
        reload = 1;
    }

    else if(y == (linhas/2) + 9 && x >= (colunas/7) + 15 && x <= (colunas/7) + 23){
        confirm = 1;
    }
    
    else if(caractere == 0){
        for(int j = 0; j < 18; j++){
            if(posicao[j].x == x && posicao[j].y == y){
                fields[pontos][caractere++] = toupper(posicao[j].letra);
                gabarito[gabarit++] = posicao[j].letra; 
                usadas[pos].x = x;
                usadas[pos].y = y;
                usadas[pos++].letra = posicao[j].letra;
                anteriorx = x;
                anteriory = y;
            } 
        }
    }
    else{
        int usado = 0;
        for(int j = 0; j < 18; j++){
            for(int k = 0; k < 18; k++){
                x == usadas[k].x && y == usadas[k].y ? usado = 1 : 0;
            } 
            if(usado == 0 && posicao[j].x == x && posicao[j].y == y && abs(y - anteriory) < 3 && abs(x - anteriorx ) < 5){
                    fields[pontos][caractere++] = toupper(posicao[j].letra);
                    gabarito[gabarit++] = posicao[j].letra;
                    usadas[pos].x = x;
                    usadas[pos].y = y;
                    usadas[pos++].letra = posicao[j].letra;
                    anteriorx = x;
                    anteriory = y;
                }
        }
    }
}

void gera_anagrama(int dicionario_tamanho){
    do { 
        for(int i = 0; i < 100; i++){
            memset(palavras_validas[i], ' ', 10);
        }
        
        createTable(tabuleiro); 
        palavras_validas_qnt = getAllValidWords(palavras_validas, dicionario, tabuleiro, dicionario_tamanho);
    } while(palavras_validas_qnt < 30); 
}

void press_enter(){
    keypad(stdscr, TRUE);

    attron(A_BOLD | A_BLINK); 
    mvprintw((ROW * 0.35)+10, (COL / 2) - 7, "Pressione");
    attron(COLOR_PAIR(2));
    mvprintw((ROW * 0.35)+10, (COL / 2) + 3, "ENTER");
    attroff(A_BOLD | A_BLINK | COLOR_PAIR(2));

    int ch = getch();
    while(ch != 10){
        ch = getch();
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
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    noecho(); 
    keypad(stdscr, TRUE); 

    mousemask(BUTTON1_CLICKED, NULL);

    alloc(); 

    attron(A_BOLD);

    attron(COLOR_PAIR(1));
    mvprintw((ROW * 0.35), (COL / 2)-3, "REGRAS");
    attroff(COLOR_PAIR(1));

    mvprintw((ROW * 0.35) + 2, (COL / 2)-30, "+ Deve-se formar palavras seguindo os mesmos sentidos");
    mvprintw((ROW * 0.35) + 3, (COL / 2)-30, "+ Proibido pular e cruzar letras");
    mvprintw((ROW * 0.35) + 4, (COL / 2)-30, "+ Proibido repetir a mesma letra que foi utilizada no click anterior");
    mvprintw((ROW * 0.35) + 5, (COL / 2)-30, "+ O jogador tem que encontrar as trinta palavras");
    mvprintw((ROW * 0.35) + 6, (COL / 2)-30, "+ Cada palavra tem de quatro a nove letras");
    mvprintw((ROW * 0.35) + 8, (COL / 2)-13, "Estamos carregando seu jogo...");    
    attroff(A_BOLD);

    curs_set(0); 
    refresh();  
}

void close() {
    getch(); 

    free_alloc(); 

    endwin(); 
}

int main() {
    int dicionario_tamanho = 0;
    int estado = RUNNING; 

    init();

    int err = getDictionary("./dicionario.txt", dicionario, &dicionario_tamanho);

    if(err) close(); 

    gera_anagrama(dicionario_tamanho);

    press_enter();

    clear(); 
    curs_set(1); 
    
    while(estado != VICTORY && estado != EXIT) {
        print_t(tabuleiro, ROW, COL);
        print_fields(ROW, COL);

        char c = getch();
        MEVENT event;

        //verifica onde foi o click do mouse
        if(KEY_MOUSE){ 
            if(getmouse(&event) == OK) 
            { 
                if(event.bstate & BUTTON1_CLICKED)
                { 
                    check_click(event.x, event.y, ROW, COL); 
                } 
            } 
        }

        //caso de reinicio no game
        if(reload == 1){
            clear();
            refresh();
            
            attron(A_BOLD);
            mvprintw((ROW/2), (COL/2) - 24, "Nao conseguiu desvendar as palavras anteriores?");
            attron(COLOR_PAIR(3));
            mvprintw((ROW/2) + 2, (COL/2) - 23, "Fique CALMO, iremos gerar um novo anagrama...");
            attroff(A_BOLD | COLOR_PAIR(3));

            refresh();

            for(int n = 0; n < pos; n++){
                usadas[n].x = 0;
                usadas[n].y = 0;
                usadas[n].letra = ' ';
            }

            for(int m = 0; m < 30; m++){
                memset(fields[m],' ', 10);    
            }

            gera_anagrama(dicionario_tamanho);

            memset(gabarito, '\0', gabarit);
            gabarit = 0;

            reload = 0;
            confirm = 0;
            pontos = 0;
            caractere = 0;
            pos = 0;

            clear();
            refresh();
        } 

        //caso de confirmar a palvra
        else if(confirm == 1){
            int deubom = 0;
            for(int n = 0; n < pos; n++){
                usadas[n].x = 0;
                usadas[n].y = 0;
                usadas[n].letra = ' ';
            }

            for(int j = 0; j < palavras_validas_qnt; j++){
                if(!strcmp(palavras_validas[j], gabarito)){
                    deubom = 1;
                    memset(palavras_validas[j], '\0', 10);  
                }  
            }

            for(int j = 0; j < dicionario_tamanho; j++){
                if(!strcmp(dicionario[j], gabarito)){
                    deubom = 1;
                    memset(dicionario[j], '\0', 10);  
                }  
            }

            memset(gabarito, '\0', gabarit);
            gabarit = 0;

            confirm = 0;
            caractere = 0;
            pos = 0;
            deubom == 1 ? pontos++ : 0;
            deubom == 0 ? memset(fields[pontos], ' ', 10) : 0;

        }
        
        if(pontos == 29){
            estado = VICTORY;

            clear();
            refresh();

            attron(A_BOLD);
            mvprintw((ROW/2), (COL/2) - 22, "Muito Bom! Voce conseguiu concluir o");
            attron(COLOR_PAIR(3));
            mvprintw((ROW/2), (COL/2) + 15, "DESAFIO");
            attroff(A_BOLD | COLOR_PAIR(3));
            
            attron(A_BOLD);
            mvprintw((ROW/2) + 2, (COL/2) - 25, "Caso queira desafiar-se novamente, pressione");
            attron(COLOR_PAIR(2));
            mvprintw((ROW/2) + 2, (COL/2) + 20, "ENTER");
            attroff(A_BOLD | COLOR_PAIR(2));

            int ch = getch();
            if(ch == 10){
                estado = RUNNING;
                reload = 1;

                for(int n = 0; n < pos; n++){
                usadas[n].x = 0;
                usadas[n].y = 0;
                usadas[n].letra = ' ';
                }

                for(int m = 0; m < 30; m++){
                    memset(fields[m],' ', 10);    
                }

                gera_anagrama(dicionario_tamanho);

                memset(gabarito, '\0', gabarit);
                gabarit = 0;

                reload = 0;
                confirm = 0;
                pontos = 0;
                caractere = 0;
                pos = 0;
            }

            clear();
            refresh();
        }

        else if(c == 27) estado = EXIT; 

        refresh(); 
    }

    close();
 
    return 0;  
}
