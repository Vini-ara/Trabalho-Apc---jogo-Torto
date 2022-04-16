#ifndef GAMELOGIC_H
#define GAMELOGIC_H

void createTable(char out[][4]);
int getAllValidWords(char ** validWords, char ** dictionaryWords, char table[][4], int dictionaryLen);

#endif