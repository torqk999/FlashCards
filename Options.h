#pragma once



typedef struct {
	const char* _name;
	char _char;
	int _type; // 0 = toggle, 1 = trigger
	int _state;
	void(*_action)();
} Option;

Option* _globalOptions;

int Option_State(int opt);

//int Option_GetState(char opt);

void Option_Event(int opt);

void Option_GetEvent(char opt);