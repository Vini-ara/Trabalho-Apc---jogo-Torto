#include <stdio.h> 
#include <ncurses.h> 
#include <string.h>

char tabuleiro [6][4] = {{"rsi"}, {"epn"}, {"ega"}, {"idj"}, {"aoa"}, {"dir"}}; 

void print_t(char t[6][4], int r, int c) {
   for(int i = 0; i < 6; i++) {
       for(int j = 0; j < 3; j++) {
           mvprintw((r/2) - 5 + i * 2, (c/4) - 5 + 4 * j, "(");
           attron(A_UNDERLINE | A_BOLD); 
           printw("%c", toupper(t[i][j])); 
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
    int row, col; 

    initscr(); 

    if(!has_colors()) {
        endwin(); 
        printf("Your terminal does not support color\n");
        return -1; 
    }

    start_color(); 
    init_pair(1, COLOR_RED, COLOR_BLACK);

    raw(); 
    noecho();

    getmaxyx(stdscr,row,col);

    print_t(tabuleiro, row, col);
    print_fields(row, col);

    getch();

    endwin(); 
    return 0;  
}
