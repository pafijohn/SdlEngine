#pragma once

#include "Renderable.h"

class Page : public Renderable
{
public:
	virtual void OnPageLoad() = 0;
	virtual bool OnPageUnload() = 0;
	
	virtual void OnLeftClick( int x, int y ) = 0;
	virtual void OnScrollUp() = 0;
	virtual void OnScrollDown() = 0;
};
