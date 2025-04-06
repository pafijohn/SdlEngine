#pragma once

#include "Timer.h"
#include "Pointers.h"
#include "Renderable.h"
#include "EventConsumer.h"

#include <list>

using namespace Pointers;

class ChatMessage
{
	Timer fadeTimer;
	std::string msg;
public:
	ChatMessage(const std::string& msg);
	
	const std::string& GetMsg();
};

class CharacterChat:
	public EventConsumer,
	public Renderable
{
	Timer activateTimer;
	Timer pipeTimer;
	std::string chat;
	size_t pipePos;
	bool displayPipe;
	std::list< SP< ChatMessage > > messages;
	
	std::map<char, char> caps;
	
public:
	CharacterChat();
	
	virtual bool Update();
	virtual void Render();
	
	void Activate();
	void Deactivate();
	std::string GetChat();
};