#pragma once

#include <set>
#include <stack>
#include <SDL.h>
#include <vector>

typedef std::set<int> KeySet;

class EventConsumer
{
	static std::stack< EventConsumer* > consumers;
	
protected:
	KeySet keysPressed;
	KeySet cycleKeys;
	
	std::vector< SDL_MouseButtonEvent > cycleButton;
	
public:
	static void Push( EventConsumer* consumer );
	static void Pop();
	
	static void KeyPressed( int keyCode );
	static void KeyReleased( int keyCode );
	static void ButtonPressed( const SDL_MouseButtonEvent& event );

	EventConsumer();
	
	void SetAsActiveConsumer();
	void ClearAsActiveConsumer();
	void OnKeyDown( int keyCode );
	void OnKeyUp( int keyCode );
	void OnButtonPressed( const SDL_MouseButtonEvent& event );
	void Clear();
	
	bool isActive;
};