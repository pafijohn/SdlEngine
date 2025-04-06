#pragma once

#include "Text.h"
#include "Pointers.h"
#include "SdlColor.h"
#include "SdlTexture.h"
#include "Containers.h"
#include "Displayable.h"
#include "Positionable.h"
#include "EntityTexture.h"
#include "EventConsumer.h"

#include <string>
#include <unordered_map>

using namespace Pointers;
using namespace Containers;

class ContextMenuItem:
	public Entity,
	public Text
{
protected:
	static const std::string DEFAULT_IDENT;
public:
	ContextMenuItem();
	virtual void AddText(const std::string& text, const std::string& identifier = "");
	virtual void OnClicked();
	virtual void Reset();
};

class ContextMenu:
	public Renderable,
	public Positionable,
	public EventConsumer,
	public Displayable
{
protected:
	Vector< SP< ContextMenuItem > > selections;
	
	SP<SdlTexture> background;
	SP<SdlTexture> highlight;
	
	ContextMenuItem* GetItemAt(const SDL_Point& point);
public:
	ContextMenu();
	
	void AddOption(ContextMenuItem* item);
	void CalculatePositions();
	
	virtual void OnMove();
	virtual void Reset();
	
	virtual bool Update();
	virtual void Render();
};