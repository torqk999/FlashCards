/*
	FlashCards v0.1 - Author: Torqk - 2024

	CLS_SIZE = Set the number of lines to space out each flash card on the console.

	[ ========================================== .txt legend: ========================================== ]

	<a:label#size>	=	New div. single character is the div symbol. string after ':' is the label.
						number after '#' is the column width. eg: <a:Answer#20>

	<*>				=	New entry. can be combined with the first div block. eg: <*q>
	
	</>				=	Finish entry. Nothing more will get populated for that flash card

	[ ================================================================================================== ]

	Check Source directory for sample .txt files.
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <IndexArray.h>
#include <Options.h>
#include <Divs.h>

#define DEBUG_ON
#define INDEX_ON
#define CLS_SIZE 20

#pragma region CODE

#define OPT_CNT 7
#define TOGGLE_CNT 3

#define DEBUG(str, fmt)
#define DEBUGx(str, fmt)

#ifdef DEBUG_ON
	#define DEBUG(str, fmt) printf(str, fmt)
	#ifdef INDEX_ON
		#define DEBUGx(str, fmt) printf(str, fmt)
	#endif
#endif


typedef enum {
	RANDOM = 0,
	FOCUS = 1,
	REVERSE = 2,
	PRINT = 3,
	MASK = 4,
	TRANSFER = 5,
	QUIT = 6
} OPTION;


void transfer();
void app_exit();
void attr_mask();

Option options[] = {
	{
		"Randomize",
		'r',
		0,
		0,
		NULL
	},
	{
		"Toggle Lists",
		'l',
		0,
		0,
		NULL
	},
	{
		"Reverse Directions",
		'd',
		0,
		0,
		NULL
	},
	{
		"Print All",
		'p',
		1,
		0,
		Divs_Debug
	},
	{
		"Attribute Mask",
		'm',
		1,
		0,
		attr_mask
	},
	{
		"Transfer Item",
		't',
		1,
		0,
		transfer
	},
	{
		"Quit Application",
		'q',
		1,
		0,
		app_exit
	},
	{
		"Null Optiion",
		NULL,
		0,0,NULL
	}
};

IndexArray list;
IndexArray focus;
int index;

void app_exit() {
	printf("exit!\n");
	exit(0);
}

void transfer() {
	IndexArray* trg = Option_State(FOCUS) ? &list : &focus;
	IndexArray* src = Option_State(FOCUS) ? &focus : &list;

	if (IndexArray_Insert(trg, index))
		IndexArray_Remove(src, index);
}

void attr_mask() {

	int mask = input_dec();

	for (int i = divCount - 1; i > -1; i--) {
		divs[i]._active = mask & 1;
		mask >> 1;
	}

}

int input_dec() {

	int output = 0;
	char next = getchar();
	while (next != '\n') {
		output *= 10;
		output += next - '0';
	}

	return output;
}

void input_cmd() {

	Option_GetEvent(getchar());
}

void cmd_info() {

	for (int i = 0; i < TOGGLE_CNT; i++) {
		printf("[ %s (%c)%s ]", options[i]._name, options[i]._char, options[i]._type ? "" : (":%s", options[i]._state ? ": true" : ": false"));
	}

	printf("\n");

	for (int i = TOGGLE_CNT; i < OPT_CNT; i++) {
		printf("[ %s (%c) ]", options[i]._name, options[i]._char);
	}

	printf("\n");
}


void operation() {

	IndexArray* targetArray;

	while (1)
	{
		targetArray = Option_State(FOCUS) ? &focus : &list;

		cmd_info();

		if (targetArray->_count < 1) {
			printf("No elements in this list!\n");
		}

		else {

			index = IndexArray_Current(targetArray, Option_State(RANDOM));

			printf("[COUNTER] [%d/%d]", targetArray->_current, targetArray->_count);

			DEBUGx("[INDEX] [%d]\n", index);
			printf("\n");

			// Question
			Divs_PrintHeader(0, 1);
			printf("\n");
			Divs_PrintInfo(index, -1, 1);

			input_cmd();

			// Answer
			Divs_PrintHeaders(1, 1);
			printf("\n");
			Divs_PrintInfo(index, 1, 1);
		}

		input_cmd();

		Option_State(REVERSE) ? 
			IndexArray_Reverse(targetArray) :
			IndexArray_Advance(targetArray);

		// Screen flush
		for (int i = 0; i < CLS_SIZE; i++)
			printf("\n");
	}
}




int setup() {
	char fileNameBuffer[64];
	fileNameBuffer[63] = '\0';
	printf("enter file 'dir/name.txt': ");
	gets_s(fileNameBuffer, 63);

	struct stat fStat;
	if (stat(fileNameBuffer, &fStat)){
		printf("Failed to get file size!\n");
		return 0;
	}

	FILE* f = fopen(fileNameBuffer, "r");
	if (!f) {
		printf("Failed to open file!\n");
		return 0;
	}

	if (!Divs_Setup(f, fStat.st_size)) {
		printf("Failed to generate divs!\n");
		return 0;
	}
	

	list = IndexArray_Create(entryCount, 1);
	focus = IndexArray_Create(entryCount, 0);

	_globalOptions = options;

	printf("setup complete!\n");
	return 1;
}

int main()
{
	if (!setup()) {
		printf("setup failed!\n");
		return 1;
	}
		
	operation();
	
	return 0;
}

#pragma endregion
