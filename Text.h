#pragma once

#include <SDL.h>
#include <string>
#include <vector>

#include "SdlRect.h"
#include "Pointers.h"
#include "Renderable.h"
#include "Positionable.h"

using namespace Pointers;

struct TextChunk
{
	std::string text;
	std::string identifier;
};

class Text:
	public Renderable,
	public Positionable
{
protected:
	std::vector< SP< TextChunk > > chunks;
	int background;
	
public:
	enum
	{
		WHITE,
		BEIGE,
		GREY,
		BROWN,
		NONE
	};
	
	Text();
	~Text();
	
	void Clear();
	void SetBackground(int background);
	
	void GetRect(SdlRect& rect);
	
	virtual bool Update();
	virtual void Render();
	
	virtual void AddText(const std::string& text, const std::string& identifier = "");
};
