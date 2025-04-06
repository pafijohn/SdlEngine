#include "Utils.h"
#include "Bindings.h"

#include <regex>
#include <fstream>
#include <iostream>

static Bindings* inst = nullptr;
static std::unordered_map<int, std::string> keycodes;
std::unordered_map< std::string, std::vector< std::string > > Bindings::possibleActions;
std::unordered_map< std::string, std::unordered_map< std::string, std::string > > Bindings::bindings;

static std::unordered_map<std::string, int> keynames(
	{
		{"0", SDLK_0},
		{"1", SDLK_1},
		{"2", SDLK_2},
		{"3", SDLK_3},
		{"4", SDLK_4},
		{"5", SDLK_5},
		{"6", SDLK_6},
		{"7", SDLK_7},
		{"8", SDLK_8},
		{"9", SDLK_9},
		{"A", SDLK_a},
		{"AC_BACK", SDLK_AC_BACK},
		{"AC_BOOKMARKS", SDLK_AC_BOOKMARKS},
		{"AC_FORWARD", SDLK_AC_FORWARD},
		{"AC_HOME", SDLK_AC_HOME},
		{"AC_REFRESH", SDLK_AC_REFRESH},
		{"AC_SEARCH", SDLK_AC_SEARCH},
		{"AC_STOP", SDLK_AC_STOP},
		{"AGAIN", SDLK_AGAIN},
		{"ALTERASE", SDLK_ALTERASE},
		{"QUOTE", SDLK_QUOTE},
		{"APPLICATION", SDLK_APPLICATION},
		{"AUDIOMUTE", SDLK_AUDIOMUTE},
		{"AUDIONEXT", SDLK_AUDIONEXT},
		{"AUDIOPLAY", SDLK_AUDIOPLAY},
		{"AUDIOPREV", SDLK_AUDIOPREV},
		{"AUDIOSTOP", SDLK_AUDIOSTOP},
		{"B", SDLK_b},
		{"BACKSLASH", SDLK_BACKSLASH},
		{"BACKSPACE", SDLK_BACKSPACE},
		{"BRIGHTNESSDOWN", SDLK_BRIGHTNESSDOWN},
		{"BRIGHTNESSUP", SDLK_BRIGHTNESSUP},
		{"C", SDLK_c},
		{"CALCULATOR", SDLK_CALCULATOR},
		{"CANCEL", SDLK_CANCEL},
		{"CAPSLOCK", SDLK_CAPSLOCK},
		{"CLEAR", SDLK_CLEAR},
		{"CLEARAGAIN", SDLK_CLEARAGAIN},
		{"COMMA", SDLK_COMMA},
		{"COMPUTER", SDLK_COMPUTER},
		{"COPY", SDLK_COPY},
		{"CRSEL", SDLK_CRSEL},
		{"CURRENCYSUBUNIT", SDLK_CURRENCYSUBUNIT},
		{"CURRENCYUNIT", SDLK_CURRENCYUNIT},
		{"CUT", SDLK_CUT},
		{"D", SDLK_d},
		{"DECIMALSEPARATOR", SDLK_DECIMALSEPARATOR},
		{"DELETE", SDLK_DELETE},
		{"DISPLAYSWITCH", SDLK_DISPLAYSWITCH},
		{"DOWN", SDLK_DOWN},
		{"E", SDLK_e},
		{"EJECT", SDLK_EJECT},
		{"END", SDLK_END},
		{"EQUALS", SDLK_EQUALS},
		{"ESCAPE", SDLK_ESCAPE},
		{"EXECUTE", SDLK_EXECUTE},
		{"EXSEL", SDLK_EXSEL},
		{"F", SDLK_f},
		{"F1", SDLK_F1},
		{"F10", SDLK_F10},
		{"F11", SDLK_F11},
		{"F12", SDLK_F12},
		{"F13", SDLK_F13},
		{"F14", SDLK_F14},
		{"F15", SDLK_F15},
		{"F16", SDLK_F16},
		{"F17", SDLK_F17},
		{"F18", SDLK_F18},
		{"F19", SDLK_F19},
		{"F2", SDLK_F2},
		{"F20", SDLK_F20},
		{"F21", SDLK_F21},
		{"F22", SDLK_F22},
		{"F23", SDLK_F23},
		{"F24", SDLK_F24},
		{"F3", SDLK_F3},
		{"F4", SDLK_F4},
		{"F5", SDLK_F5},
		{"F6", SDLK_F6},
		{"F7", SDLK_F7},
		{"F8", SDLK_F8},
		{"F9", SDLK_F9},
		{"FIND", SDLK_FIND},
		{"G", SDLK_g},
		{"BACKQUOTE", SDLK_BACKQUOTE},
		{"H", SDLK_h},
		{"HELP", SDLK_HELP},
		{"HOME", SDLK_HOME},
		{"I", SDLK_i},
		{"INSERT", SDLK_INSERT},
		{"J", SDLK_j},
		{"K", SDLK_k},
		{"KBDILLUMDOWN", SDLK_KBDILLUMDOWN},
		{"KBDILLUMTOGGLE", SDLK_KBDILLUMTOGGLE},
		{"KBDILLUMUP", SDLK_KBDILLUMUP},
		{"KP_0", SDLK_KP_0},
		{"KP_00", SDLK_KP_00},
		{"KP_000", SDLK_KP_000},
		{"KP_1", SDLK_KP_1},
		{"KP_2", SDLK_KP_2},
		{"KP_3", SDLK_KP_3},
		{"KP_4", SDLK_KP_4},
		{"KP_5", SDLK_KP_5},
		{"KP_6", SDLK_KP_6},
		{"KP_7", SDLK_KP_7},
		{"KP_8", SDLK_KP_8},
		{"KP_9", SDLK_KP_9},
		{"KP_A", SDLK_KP_A},
		{"KP_AMPERSAND", SDLK_KP_AMPERSAND},
		{"KP_AT", SDLK_KP_AT},
		{"KP_B", SDLK_KP_B},
		{"KP_BACKSPACE", SDLK_KP_BACKSPACE},
		{"KP_BINARY", SDLK_KP_BINARY},
		{"KP_C", SDLK_KP_C},
		{"KP_CLEAR", SDLK_KP_CLEAR},
		{"KP_CLEARENTRY", SDLK_KP_CLEARENTRY},
		{"KP_COLON", SDLK_KP_COLON},
		{"KP_COMMA", SDLK_KP_COMMA},
		{"KP_D", SDLK_KP_D},
		{"KP_DBLAMPERSAND", SDLK_KP_DBLAMPERSAND},
		{"KP_DBLVERTICALBAR", SDLK_KP_DBLVERTICALBAR},
		{"KP_DECIMAL", SDLK_KP_DECIMAL},
		{"KP_DIVIDE", SDLK_KP_DIVIDE},
		{"KP_E", SDLK_KP_E},
		{"KP_ENTER", SDLK_KP_ENTER},
		{"KP_EQUALS", SDLK_KP_EQUALS},
		{"KP_EQUALSAS400", SDLK_KP_EQUALSAS400},
		{"KP_EXCLAM", SDLK_KP_EXCLAM},
		{"KP_F", SDLK_KP_F},
		{"KP_GREATER", SDLK_KP_GREATER},
		{"KP_HASH", SDLK_KP_HASH},
		{"KP_HEXADECIMAL", SDLK_KP_HEXADECIMAL},
		{"KP_LEFTBRACE", SDLK_KP_LEFTBRACE},
		{"KP_LEFTPAREN", SDLK_KP_LEFTPAREN},
		{"KP_LESS", SDLK_KP_LESS},
		{"KP_MEMADD", SDLK_KP_MEMADD},
		{"KP_MEMCLEAR", SDLK_KP_MEMCLEAR},
		{"KP_MEMDIVIDE", SDLK_KP_MEMDIVIDE},
		{"KP_MEMMULTIPLY", SDLK_KP_MEMMULTIPLY},
		{"KP_MEMRECALL", SDLK_KP_MEMRECALL},
		{"KP_MEMSTORE", SDLK_KP_MEMSTORE},
		{"KP_MEMSUBTRACT", SDLK_KP_MEMSUBTRACT},
		{"KP_MINUS", SDLK_KP_MINUS},
		{"KP_MULTIPLY", SDLK_KP_MULTIPLY},
		{"KP_OCTAL", SDLK_KP_OCTAL},
		{"KP_PERCENT", SDLK_KP_PERCENT},
		{"KP_PERIOD", SDLK_KP_PERIOD},
		{"KP_PLUS", SDLK_KP_PLUS},
		{"KP_PLUSMINUS", SDLK_KP_PLUSMINUS},
		{"KP_POWER", SDLK_KP_POWER},
		{"KP_RIGHTBRACE", SDLK_KP_RIGHTBRACE},
		{"KP_RIGHTPAREN", SDLK_KP_RIGHTPAREN},
		{"KP_SPACE", SDLK_KP_SPACE},
		{"KP_TAB", SDLK_KP_TAB},
		{"KP_VERTICALBAR", SDLK_KP_VERTICALBAR},
		{"KP_XOR", SDLK_KP_XOR},
		{"L", SDLK_l},
		{"LALT", SDLK_LALT},
		{"LCTRL", SDLK_LCTRL},
		{"LEFT", SDLK_LEFT},
		{"LEFTBRACKET", SDLK_LEFTBRACKET},
		{"LGUI", SDLK_LGUI},
		{"LSHIFT", SDLK_LSHIFT},
		{"M", SDLK_m},
		{"MAIL", SDLK_MAIL},
		{"MEDIASELECT", SDLK_MEDIASELECT},
		{"MENU", SDLK_MENU},
		{"MINUS", SDLK_MINUS},
		{"MODE", SDLK_MODE},
		{"MUTE", SDLK_MUTE},
		{"N", SDLK_n},
		{"NUMLOCKCLEAR", SDLK_NUMLOCKCLEAR},
		{"O", SDLK_o},
		{"OPER", SDLK_OPER},
		{"OUT", SDLK_OUT},
		{"P", SDLK_p},
		{"PAGEDOWN", SDLK_PAGEDOWN},
		{"PAGEUP", SDLK_PAGEUP},
		{"PASTE", SDLK_PASTE},
		{"PAUSE", SDLK_PAUSE},
		{"PERIOD", SDLK_PERIOD},
		{"POWER", SDLK_POWER},
		{"PRINTSCREEN", SDLK_PRINTSCREEN},
		{"PRIOR", SDLK_PRIOR},
		{"Q", SDLK_q},
		{"R", SDLK_r},
		{"RALT", SDLK_RALT},
		{"RCTRL", SDLK_RCTRL},
		{"RETURN", SDLK_RETURN},
		{"RETURN2", SDLK_RETURN2},
		{"RGUI", SDLK_RGUI},
		{"RIGHT", SDLK_RIGHT},
		{"RIGHTBRACKET", SDLK_RIGHTBRACKET},
		{"RSHIFT", SDLK_RSHIFT},
		{"S", SDLK_s},
		{"SCROLLLOCK", SDLK_SCROLLLOCK},
		{"SELECT", SDLK_SELECT},
		{"SEMICOLON", SDLK_SEMICOLON},
		{"SEPARATOR", SDLK_SEPARATOR},
		{"SLASH", SDLK_SLASH},
		{"SLEEP", SDLK_SLEEP},
		{"SPACE", SDLK_SPACE},
		{"STOP", SDLK_STOP},
		{"SYSREQ", SDLK_SYSREQ},
		{"T", SDLK_t},
		{"TAB", SDLK_TAB},
		{"THOUSANDSSEPARATOR", SDLK_THOUSANDSSEPARATOR},
		{"U", SDLK_u},
		{"UNDO", SDLK_UNDO},
		{"UNKNOWN", SDLK_UNKNOWN},
		{"UP", SDLK_UP},
		{"V", SDLK_v},
		{"VOLUMEDOWN", SDLK_VOLUMEDOWN},
		{"VOLUMEUP", SDLK_VOLUMEUP},
		{"W", SDLK_w},
		{"WWW", SDLK_WWW},
		{"X", SDLK_x},
		{"Y", SDLK_y},
		{"Z", SDLK_z}
	}
);

Bindings::Bindings()
{
	this->LoadActions();
	this->LoadConfig();
}

void Bindings::LoadActions()
{
	std::string line;
	std::ifstream myfile("cfg\\all_bindings.cfg");
	
	std::regex e("(sub)(.*)");
	
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			std::vector<std::string> splitted;
			splitString(line, splitted);
			
			if (splitted.size() == 2)
			{
				const std::string& object = splitted.at(0);
				const std::string& action = splitted.at(1);
				
				this->possibleActions[object].push_back(action);
			}
		}
	}
}

void Bindings::LoadConfig()
{
	std::string line;
	std::ifstream myfile("cfg\\bindings.txt");
	
	for (auto it = keynames.begin(); it != keynames.end(); it++)
	{
		keycodes.emplace(it->second, it->first);
	}
	
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			std::vector<std::string> splitted;
			splitString(line, splitted);
			
			if (splitted.size() >= 1)
			{
				const std::string& command = splitted.at(0);
				if (command == "bind")
				{
					if (splitted.size() >= 3)
					{
						//const std::string& key = splitted.at(1);
						//const std::string& action = splitted.at(2);
						
						// TODO this needs to figure out which object this key is for
						//this->bindings[key] = action;
					}
				}
			}
		}
	}
}

Bindings* Bindings::GetInst()
{
	if (inst == nullptr)
	{
		inst = new Bindings();
	}
	
	return inst;
}

bool GetBoundAction(std::string&& object, int keyCode, std::string& action)
{
	/*
	this->bindings[object][]
	bool hasCode = keycodes.count(keyCode);
	bool got = hasCode;
	
	if (hasCode)
	{
		bool hasBinding = this->bindings.count(keyname);
		const std::string& keyname = keycodes[keyCode];
		got = got && hasBinding;
		
		if (hasBinding)
		{
			action = this->bindings[keyname];
		}
	}
	
	return got;
	*/
	return false;
}
