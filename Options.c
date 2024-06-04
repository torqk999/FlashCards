#include <Options.h>



int Option_State(int opt) {
	return _globalOptions[opt]._state;
}

//int Option_GetState(char opt)
//{
//	int index = 0;
//	while (_globalOptions[index]._char != opt)
//		index++;
//	return Option_State(index);
//}

void Option_Event(int opt)
{
	switch (_globalOptions[opt]._type)
	{

	case 0:
		_globalOptions[opt]._state = !_globalOptions[opt]._state;
		break;

	case 1:
		_globalOptions[opt]._action();
		break;

	default:
		break;
	}
}

void Option_GetEvent(char opt)
{
	int index = 0;
	while (_globalOptions[index]._char != opt) {
		if (_globalOptions[index]._char == 0)
			return;
		index++;
	}
		
	Option_Event(index);
}
