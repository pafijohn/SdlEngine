#include "Displayable.h"

Displayable::Displayable()
{
	this->displayed = false;
}

bool Displayable::IsDisplayed()
{
	return this->displayed;
}

void Displayable::SetDisplayed( bool display )
{
	this->displayed = display;
}

void Displayable::Show()
{
	this->displayed = true;
}

void Displayable::Hide()
{
	this->displayed = false;
}
