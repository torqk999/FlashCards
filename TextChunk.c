#include <TextChunk.h>

TextChunk TextChunk_New(char* head)
{
	TextChunk result;
	TextChunk_Fresh(&result, head);
	return result;
}

void TextChunk_Fresh(TextChunk* loc, char* head)
{
	loc->_head = head;
	loc->_size_full = 0;
	loc->_size_trim = 0;
	loc->_currentIndex = 0;
}

void TextChunk_PrintWord(TextChunk* loc)
{
	while (!whiteSpace(loc->_head[loc->_currentIndex]) &&
		loc->_currentIndex < loc->_size_full) {
		printf("%c", loc->_head[loc->_currentIndex]);
		loc->_currentIndex++;
	}

	printf(" ");
	loc->_currentIndex++;
}

int TextChunk_NextWordLength(TextChunk* loc)
{
	int size = 0;
	while (!whiteSpace(loc->_head[loc->_currentIndex + size]))
		size++;
	return size;
}

int strlen(const char* str)
{
	int size = 0;
	while (str[size])
		size++;
	return size;
}

int whiteSpace(char c)
{
	return c == 32 ||		// space
		c == EOF ||			// End of File
		(c > 8 && c < 14);	// others
}
