#include "Utils.h"
#include "EventConsumer.h"

std::stack< EventConsumer* > EventConsumer::consumers;

// Static Methods

void EventConsumer::Push( EventConsumer* consumer )
{
	if ( consumers.size() > 0 )
	{
		consumers.top()->isActive = false;
	}
	
	consumer->keysPressed.clear();
	consumer->isActive = true;
	consumers.push( consumer );
}

void EventConsumer::Pop()
{
	if ( consumers.size() > 0 )
	{
		consumers.top()->isActive = false;
		consumers.top()->keysPressed.clear();
		consumers.pop();
	}
	
	if ( consumers.size() > 0 )
	{
		consumers.top()->isActive = true;
		consumers.top()->keysPressed.clear();
	}
}

void EventConsumer::KeyPressed( int keyCode )
{
	if ( consumers.size() > 0 )
	{
		consumers.top()->OnKeyDown( keyCode );
	}
}

void EventConsumer::KeyReleased( int keyCode )
{
	if ( consumers.size() > 0 )
	{
		consumers.top()->OnKeyUp( keyCode );
	}
}

void EventConsumer::ButtonPressed( const SDL_MouseButtonEvent& event )
{
	if ( consumers.size() > 0 )
	{
		consumers.top()->OnButtonPressed( event );
	}
}

// Instance Methods

EventConsumer::EventConsumer()
{
	this->isActive = false;
}

void EventConsumer::SetAsActiveConsumer()
{
	if ( !this->isActive )
	{
		EventConsumer::Push( this );
	}
}

void EventConsumer::OnKeyDown( int keyCode )
{
	this->keysPressed.insert( keyCode );
	this->cycleKeys.insert( keyCode );
}

void EventConsumer::OnKeyUp( int keyCode )
{
	KeySet::iterator it = this->keysPressed.find( keyCode );
	if ( it != this->keysPressed.end() )
	{
		this->keysPressed.erase( it );
	}
}

void EventConsumer::OnButtonPressed( const SDL_MouseButtonEvent& event )
{
	this->cycleButton.push_back( event );
}

void EventConsumer::Clear()
{
	this->cycleButton.clear();
	this->cycleKeys.clear();
}
