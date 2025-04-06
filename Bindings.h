#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <unordered_map>

class Bindings
{
private:
	Bindings();
	void LoadConfig();
	// Object to key to action
	static std::unordered_map< std::string, std::unordered_map< std::string, std::string > > bindings;
	// Object to list of actions
	static std::unordered_map< std::string, std::vector< std::string > > possibleActions;
	
public:
	static Bindings* GetInst();
	
	void LoadActions();
	bool GetBoundAction(std::string&& container, int keyCode, std::string& action);
};