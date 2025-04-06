#pragma once

class Displayable
{
	bool displayed;
public:
	Displayable();
	
	bool IsDisplayed();
	void ToggleDisplayed();
	virtual void SetDisplayed(bool display);
	virtual void Show();
	virtual void Hide();
};