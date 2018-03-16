#pragma once

class Displayable
{
	bool displayed;
public:
	Displayable();
	
	bool IsDisplayed();
	void ToggleDisplayed();
	void SetDisplayed( bool display );
	void Show();
	void Hide();
};