#pragma once

#include "Timer.h"
#include "Renderable.h"
#include "EventConsumer.h"
#include <list>

class ChatMessage
{
	Timer fadeTimer;
	std::string msg;
public:
	ChatMessage( const std::string& msg );
	
	std::string GetMsg();
};

class CharacterChat: public EventConsumer, public Renderable
{
	Timer activateTimer;
	Timer pipeTimer;
	std::string chat;
	int pipePos;
	bool displayPipe;
	std::list<ChatMessage*> messages;
public:
	CharacterChat();
	
	virtual bool Update();
	virtual void Render();
	
	void Activate();
	void Deactivate();
	std::string GetChat();
};