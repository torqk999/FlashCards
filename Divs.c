#include <Divs.h>


char rTolkens[RESERVED_TOLKEN_CNT] = {
	'*',	// Entry
	'/',	// Terminate-Entry
	'<',	// Open
	'>',	// Close
	':',	// Label
	'#',	// Width
	' '		// Empty
};

char labelBin[DIV_LBL_BIN_SIZE];

Div divs[DIV_MAX_CNT] = {
	{
		'q',
		"Question",
		1,
		12
	}
};

Div nullDiv = {
	' ',
	"Null",
	0,
	0
};

TextChunk* divChunks = NULL;

char* mem = NULL;

int divCount = 0;
int entryCount = 0;

void Div_ctor(Div* loc, char symbol)
{
	loc->_symbol = symbol;
	loc->_name = NULL;
	loc->_active = 1;
	loc->_colWidth = DEF_DIV_COL_WIDTH;
}

void Div_clear(Div* loc)
{
	*loc = nullDiv;
	//printf("%c\n", loc->_symbol);
}

void Divs_Init() {
	for (int i = 1; i < DIV_MAX_CNT; i++)			// skip the 'q' div
		Div_clear(&(divs[i]));
}

int Divs_AllocateChunks()
{
	int count = divCount * entryCount;

	if (count < 1) {
		printf("No chunks!\n");
		return 0;
	}

	divChunks = malloc(sizeof(TextChunk) * count);
	if (!divChunks) {
		printf("Chunks malloc failed!\n");
		return 0;
	}

	return 1;
}

Div* Divs_Aquire(char symbol) {

	for (int i = 0; i < DIV_MAX_CNT; i++) {
		if (divs[i]._symbol == symbol) {
			return &(divs[i]);						// return existing
		}

		if (divs[i]._symbol == rTolkens[EMPTY]) {	// add and return new 
			Div_ctor(&(divs[i]), symbol);
			return &(divs[i]);
		}
	}
	return NULL;									// end of possible divs
}

void Divs_PrintHeader(int index, int active) {

	if (active && !divs[index]._active)
		return;

	int len = strlen(divs[index]._name);
	printf("[%s", divs[index]._name);
	for (int i = 0; i < divs[index]._colWidth - (2 + len); i++) // brackets plus string length
		printf(" ");
	printf("]");

}

void Divs_PrintHeaders(int active, int mode) {
	for (int i = (mode == -1 || mode == 0 ? 0 : 1); i < (mode == -1 ? 1 : divCount); i++)
		Divs_PrintHeader(i, active);
}


void Divs_PrintInfo(int index, int mode, int active) {

	int remaining = 1;

	while (remaining) {
		remaining = 0;

		for (int i = (mode == -1 || mode == 0 ? 0 : 1); i < (mode == -1 ? 1 : divCount); i++) {
			TextChunk* targetChunk = &(divChunks[(index * divCount) + i]);

			if (active && !divs[i]._active)
				continue;

			int accumulator = 0;
			if (targetChunk->_currentIndex < targetChunk->_size_trim) {
				remaining = 1;

				printf(" ");															// Left-side border

				accumulator += (1 + TextChunk_NextWordLength(targetChunk));				// Initial accumulator value with space
				TextChunk_PrintWord(targetChunk);										// Forced word push on first
				int nextPlusSpace = (1 + TextChunk_NextWordLength(targetChunk));		// Next char range

				while (accumulator + nextPlusSpace < divs[i]._colWidth - 2 &&				// Continue to print while room remains
					targetChunk->_currentIndex < targetChunk->_size_trim) {					// And there is words left to print			
					TextChunk_PrintWord(targetChunk);
					accumulator += nextPlusSpace;
					nextPlusSpace = (1 + TextChunk_NextWordLength(targetChunk));
				}

				accumulator += 1;														// Right-side border
			}

			for (int j = 0; j < divs[i]._colWidth - accumulator; j++)					// Right-side/Line padding
				printf(" ");
		}

		printf("\n");
	}

	for (int i = (mode == -1 || mode == 0 ? 0 : 1); i < (mode == -1 ? 1 : divCount); i++) {
		TextChunk* targetChunk = &(divChunks[(index * divCount) + i]);

		targetChunk->_currentIndex = 0;
	}
}

void Divs_PrintBreakLine() {
	for (int i = 0; i < divCount; i++)
		for (int j = 0; j < divs[i]._colWidth; j++)
			printf("=");
	printf("\n");
}

int Divs_Index(char symbol) {

	for (int i = 0; i < DIV_MAX_CNT; i++)
		if (divs[i]._symbol == symbol)
			return i;

	return -1;
}

TextChunk* Divs_PullChunk(char symbol, int entryIndex) {

	int divIndex = Divs_Index(symbol);
	if (divIndex < 0)
		return NULL;

	return &(divChunks[(divCount * entryIndex) + divIndex]);
}

int Divs_Count() {
	int count;
	for (count = 0; count < DIV_MAX_CNT; count++)
		if (divs[count]._symbol == rTolkens[EMPTY])
			break;

	return count;
}

int Divs_GetTolken(char symbol) {
	for (int i = 0; i < RESERVED_TOLKEN_CNT; i++)
		if (rTolkens[i] == symbol)
			return i;

	return -1;
}

void Divs_Parse(FILE* f) {

	int check = 0;
	int label = 0;
	int width = 0;
	int widthBuffer;
	int newEntry = 0;
	entryCount = 0;

	char* lblPtr = labelBin;
	char* next = mem;
	*next = fgetc(f);
	Div* targetDiv = NULL;

	while (*next != EOF) {

		int tolken = Divs_GetTolken(*next);
		switch (tolken) {

		case OPEN:
			check = 1;
			//printf("div open\n");
			break;

		case CLOSE:
			if (label) {
				*lblPtr = '\0';
				lblPtr++;
				label = 0;
			}
			if (width) {
				targetDiv->_colWidth = widthBuffer;
				width = 0;
			}
			check = 0;
			//printf("div closed\n");
			break;

		case ENTRY:
			if (check) {
				newEntry = 1;
			}
			break;

		case TERM:
			if (check && newEntry) {
				//printf("entry %d complete\n", entryCount);
				entryCount++;
				newEntry = 0;
			}
			break;

		case LABEL:
			if (check && targetDiv && !label) {
				//printf("div labelPtr %d targeted!\n", (size_t)lblPtr);
				targetDiv->_name = lblPtr;
				label = 1;
				width = 0;
			}
			break;

		case WIDTH:
			if (check && targetDiv && !width) {
				widthBuffer = 0;
				label = 0;
				width = 1;
			}
			break;

		default:
			if (check) {

				if (!label && !width) {
					//printf("div symbol: %c\n", *next);
					targetDiv = Divs_Aquire(*next);
					//printf("div aquired: ");
					//if (targetDiv)
					//	printf("true\n");
					//else
					//	printf("false\n");
				}
				if (label) {
					*lblPtr = *next;
					lblPtr++;
				}
				if (width) {
					widthBuffer *= 10;
					widthBuffer += *next - '0';
				}
			}
			break;
		}



		next++;
		*next = fgetc(f);
	}

	divCount = Divs_Count();

	return 1;
}

void Divs_BuildChunks() {
	char* next = mem;

	int check = 0;
	int newEntry = 0;
	int head = 0;

	int entrySize = 0;
	int entryIndex = 0;

	TextChunk* chunk = NULL;

	while (*next != EOF)							// scanning for next codon
	{
		int tolken = Divs_GetTolken(*next);

		switch (tolken) {

		case OPEN:
			check = 1;
			chunk = NULL;
			//printf("div open\n");
			break;

		case CLOSE:
			check = 0;
			head = 1;
			//printf("div closed\n");
			break;

		case ENTRY:
			if (check) {
				newEntry = 1;
			}
			break;

		case TERM:
			if (check && newEntry) {
				//printf("entry %d built\n", entryIndex);
				entryIndex++;
				newEntry = 0;
			}
			break;

		default:
			if (check)
				chunk = Divs_PullChunk(*next, entryIndex);
			else if (chunk) {
				if (head) {
					TextChunk_Fresh(chunk, next);
					head = 0;
					//printf("chunk built (head:%d)\n", (size_t)next);
				}
				chunk->_size_full++;
				if (!whiteSpace(*next))
					chunk->_size_trim = chunk->_size_full;
			}
		}
		next++;
	}

	return 1;
}

void Divs_Debug() {

	Divs_PrintBreakLine();
	Divs_PrintHeaders(1, 0);
	Divs_PrintBreakLine();

	for (int i = 0; i < entryCount; i++) {
		Divs_PrintInfo(i, 0, 0);
		Divs_PrintBreakLine();
	}

}

int Divs_Setup(FILE* f, size_t size) {

	if (!f) {
		printf("Null fileStream!\n");
		return 0;
	}
	mem = malloc(size);
	if (!mem) {
		printf("FileBuffer malloc failed!\n");
		return 0;
	}

	Divs_Init();
	Divs_Parse(f);

	if (!Divs_AllocateChunks()) {
		printf("ChunkBuffer malloc failed!\n");
		return 0;
	}

	Divs_BuildChunks();
}




