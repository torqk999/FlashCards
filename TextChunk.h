#pragma once
#include <stdio.h>
#include <stdlib.h>

#ifndef TXT_CHNK_H
#define TXT_CHNK_H

typedef struct {
	char* _head;
	int _size_full;
	int _size_trim;
	int _currentIndex;
} TextChunk;

TextChunk TextChunk_New(char* head);

void TextChunk_Fresh(TextChunk* loc, char* head);

void TextChunk_PrintWord(TextChunk* loc);
int TextChunk_NextWordLength(TextChunk* loc);

int strlen(const char* str);

int whiteSpace(char c);

#endif // !TXT_CHNK_H

