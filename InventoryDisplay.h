#pragma once

#include "Entity.h"
#include "Renderable.h"
#include "ContextMenu.h"
#include "Inventoried.h"
#include "Displayable.h"
#include "EventConsumer.h"


/**************************************************/

class InventorySlot:
	public Entity
{
	int idx;
	Item* item;
public:
	InventorySlot();
	void SetIdx(int idx);
	int GetIdx();
	void SetItem(Item* item);
	Item* GetItem();
	void Swap(InventorySlot& other);
};

/**************************************************/

class InventoryDisplayContextMenu:
	public ContextMenu
{
public:
	void SetSlot(Inventoried* container, InventorySlot* slot);
	virtual void Reset();
};

/**************************************************/

class InventoryDisplayContextMenuItem:
	public ContextMenuItem
{
protected:
	Inventoried* container;
	InventorySlot* slot;

public:
	InventoryDisplayContextMenuItem();
	void SetSlot(Inventoried* container, InventorySlot* slot);
	
	virtual void Reset();
};

/**************************************************/

class InventoryDisplayUseContextMenuItem:
	public InventoryDisplayContextMenuItem
{
public:
	InventoryDisplayUseContextMenuItem();
	virtual void OnClicked();
};

/**************************************************/

class InventoryDisplayTransferContextMenuItem:
	public InventoryDisplayContextMenuItem
{
public:
	InventoryDisplayTransferContextMenuItem();
	virtual void OnClicked();
};

/**************************************************/

class InventoryDisplayMoveContextMenuItem:
	public InventoryDisplayContextMenuItem
{
protected:
	InventorySlot* fromSlot;

public:
	InventoryDisplayMoveContextMenuItem();
	virtual void OnClicked();
	virtual void Reset();
};

/**************************************************/

class InventoryDisplay:
	public Renderable,
	public Displayable,
	public EventConsumer
{
	SP<SdlTexture> texture;
	SP<InventoryDisplayContextMenu> contextMenu;
	Inventoried& wrapped;
	
	InventorySlot slots[Inventory::MAX_ITEMS];
	
public:
	InventoryDisplay(Inventoried& inventoried);
	virtual ~InventoryDisplay();
	
	virtual void Hide();
	
	virtual bool Update();
	virtual void Render();
	
	void HandleClick(const SDL_MouseButtonEvent& point);
};

/**************************************************/
