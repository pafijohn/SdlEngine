#include <map>
#include <cctype>
#include <string>
#include <cstdio>

#include "Text.h"
#include "Utils.h"
#include "Layers.h"
#include "Character.h"
#include "CharacterChat.h"

ChatMessage::ChatMessage(const std::string& msg)
{
	this->msg = msg;
	this->fadeTimer.Start(5000);
}

const std::string& ChatMessage::GetMsg()
{
	if (this->fadeTimer.IsExpired())
	{
		this->msg = "";
	}
	
	return this->msg;
}

CharacterChat::CharacterChat()
{
	Layers::AddToLayer(this, Layers::HUD);
	this->activateTimer.Start(0);
	this->pipeTimer.Start(0);
	this->pipePos = 0;
	this->displayPipe = true;
}

bool CharacterChat::Update()
{
	if (!this->IsActiveConsumer())
	{
		return false;
	}
	
	static bool initted = false;
	static std::map<char, char> caps;
	
	if (!initted)
	{
		caps['0'] = '!';
		caps['1'] = '@';
		caps['2'] = '#';
		caps['3'] = '$';
		caps['4'] = '%';
		caps['5'] = '^';
		caps['6'] = '&';
		caps['7'] = '*';
		caps['8'] = '(';
		caps['9'] = ')';
		
		caps['-'] = '_';
		caps['='] = '+';
		caps['['] = '{';
		caps[']'] = '}';
		caps['\\'] = '|';
		caps[';'] = ':';
		caps['\''] = '"';
		caps[','] = '<';
		caps['.'] = '>';
		caps['/'] = '?';
		initted = true;
	}
	
	const bool hasShift = this->keysPressed.find(SDLK_LSHIFT) != this->keysPressed.end() || this->keysPressed.find(SDLK_RSHIFT) != this->keysPressed.end();
	KeySet local = this->cycleKeys;
	
	for (auto it = local.begin(); it != local.end(); it++)
	{
		int c = *it;
		char mappedChar = c & 0x7F;
		
		if (c <= 0x7F && isprint(c))
		{
			if (hasShift)
			{
				if (caps.count(mappedChar) > 0)
				{
					mappedChar = caps[ mappedChar ];
				}
				
				mappedChar = toupper(mappedChar);
			}
			
			char temp[] = { mappedChar, '\0' };
			this->chat.insert(this->pipePos, temp);
			this->pipePos++;
		}
		else
		{
			switch (c)
			{
				case SDLK_HOME:
				{
					this->pipePos = 0;
					break;
				}
				case SDLK_END:
				{
					this->pipePos = this->chat.size();
					break;
				}
				case SDLK_LEFT:
				{
					if (this->pipePos)
					{
						this->pipePos--;
					}
					break;
				}
				case SDLK_RIGHT:
				{
					if (this->pipePos < this->chat.size())
					{
						this->pipePos++;
					}
					break;
				}
				case SDLK_DELETE:
				{
					if (this->pipePos < this->chat.size())
					{
						this->chat.erase(this->chat.begin() + this->pipePos, this->chat.begin() + this->pipePos + 1);
					}
					break;
				}
				case SDLK_BACKSPACE:
				{
					if (this->pipePos)
					{
						this->pipePos--;
						this->chat = this->chat.substr(0, this->chat.size() - 1);
					}
					break;
				}
				case SDLK_RETURN:
				{
					this->Deactivate();
					break;
				}
			}
		}
	}
	
	this->Clear();
	
	return true;
}

void CharacterChat::Render()
{
	Text text;
	
	auto messagesProxy = this->messages;
	
	for (auto it = messagesProxy.begin(); it != messagesProxy.end(); it++)
	{
		std::string msg = (*it)->GetMsg();
		if (msg.empty())
		{
			this->messages.pop_front();
		}
		else
		{
			text.AddText(msg + "\r\n");
		}
	}
	
	std::string msg;
	if (this->IsActiveConsumer())
	{
		msg = this->chat + "\r\n";
		
		if (this->displayPipe)
		{
			msg.insert(this->pipePos, "|");
		}
		else
		{
			msg.insert(this->pipePos, " ");
		}
		
		if (this->pipeTimer.IsExpired())
		{
			this->displayPipe = !this->displayPipe;
			this->pipeTimer.Start(500);
		}
	}
	else
	{
		// just to hold the place of the typed text area
		msg = " \r\n";
	}
	
	text.AddText(msg);
	text.Move(50, 500);
	text.Render();
}

void CharacterChat::Activate()
{
	if (this->activateTimer.IsExpired())
	{
		this->SetAsActiveConsumer();
	}
}

void CharacterChat::Deactivate()
{
	if (this->IsActiveConsumer())
	{
		if (this->chat.size() > 0)
		{
			this->messages.push_back(new ChatMessage(this->chat));
			
			std::vector<std::string> split;
			splitString(this->chat, split);
			
			if (split.size() == 2)
			{
				if (split[ 0 ] == "damage")
				{
					int amount;
					int fields = sscanf_s(split[ 1 ].data(), "%d", &amount);
					
					if (fields == 1)
					{
						character->AddHealth(-amount);
					}
				}
			}
		}
		
		this->chat = "";
		this->pipePos = 0;
		this->displayPipe = false;
		this->activateTimer.Start(50);
		this->ClearAsActiveConsumer();
	}
}
