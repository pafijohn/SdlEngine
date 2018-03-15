#pragma once

class Displayable
{
	bool displayed;
public:
	Displayable();
	
	bool IsDisplayed();
	void SetDisplayed( bool display );
	void Show();
	void Hide();
};