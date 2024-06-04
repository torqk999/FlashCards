#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <TextChunk.h>

#ifndef DIVS_H
#define DIVS_H

#define DIV_MAX_CNT 16
#define DIV_LBL_BIN_SIZE 128
#define RESERVED_TOLKEN_CNT 7
#define DEF_DIV_COL_WIDTH 32

typedef enum {
	ENTRY,
	TERM,
	OPEN,
	CLOSE,
	LABEL,
	WIDTH,
	EMPTY
} ReservedTolkens;

typedef struct {
	char _symbol;
	char* _name;
	int _active;
	int _colWidth;
} Div;

char rTolkens[];
char labelBin[];

Div divs[];
Div nullDiv;

TextChunk* divChunks;
char* mem;

int divCount;
int entryCount;

Div* Divs_Aquire(char symbol);

int Divs_Index(char symbol);

TextChunk* Divs_PullChunk(char symbol, int entryIndex);

int Divs_Count();

void Divs_Parse(FILE* f);

void Divs_BuildChunks();

int Divs_AllocateChunks(int count);

void Divs_Init();

void Div_ctor(Div* loc, char symbol);

void Div_clear(Div* loc);

int Divs_Setup(FILE* f, size_t size);

// mode: -1 = question, 0 = all, 1 = answers
void Divs_PrintInfo(int i, int mode, int active);

void Divs_PrintHeader(int i, int active);

void Divs_PrintHeaders(int active, int mode);

void Divs_Debug();

#endif // !DIVS_H