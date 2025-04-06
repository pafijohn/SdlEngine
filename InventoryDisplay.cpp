#include "Utils.h"
#include "Layers.h"
#include "Character.h"
#include "CollisionLayers.h"
#include "InventoryDisplay.h"

/**************************************************/

InventorySlot::InventorySlot()
{
	idx = -1;
	item = nullptr;
	this->SetHasCollision(true);
}

void InventorySlot::SetIdx(int idx)
{
	this->idx = idx;
}

int InventorySlot::GetIdx()
{
	return this->idx;
}

void InventorySlot::SetItem(Item* item)
{
	this->item = item;
}

Item* InventorySlot::GetItem()
{
	return this->item;
}

void InventorySlot::Swap(InventorySlot& other)
{
	Item* temp = other.item;
	other.item = this->item;
	this->item = temp;
	
}

/**************************************************/

InventoryDisplay::InventoryDisplay(Inventoried& inventoried):
	wrapped(inventoried)
{
	int width;
	int height;
	
	int swidth;
	int sheight;
	
	CollisionLayers::CreateLayer("InventoryDisplay");
	auto& collision = CollisionLayers::GetLayer("InventoryDisplay");
	
	this->contextMenu = new InventoryDisplayContextMenu();
	this->contextMenu->AddOption(new InventoryDisplayUseContextMenuItem());
	this->contextMenu->AddOption(new InventoryDisplayTransferContextMenuItem());
	this->contextMenu->AddOption(new InventoryDisplayMoveContextMenuItem());
		
	this->texture = new SdlTexture();
	this->texture->Load("resources\\inventory.png");
	this->texture->isRelative = false;
	
	SdlGlobals::GetScreenDimentions(swidth, sheight);
	
	this->texture->GetDimentions(&width, &height);
	this->texture->Move((swidth / 2) - (width / 2), (sheight / 2) - (height / 2));
	collision.SetRect(SdlRect(0, 0, swidth, sheight));
	
	this->Hide();
	
	Layers::AddToLayer(this, Layers::HUD);
}

InventoryDisplay::~InventoryDisplay()
{
	
}

void InventoryDisplay::Hide()
{
	Displayable::Hide();
	this->contextMenu->Reset();
}

bool InventoryDisplay::Update()
{
	if (this->IsDisplayed() && this->IsActiveConsumer())
	{
		for (KeySet::iterator it = this->cycleKeys.begin(); it != this->cycleKeys.end(); it++)
		{
			switch (*it)
			{
				case SDLK_ESCAPE:
				{
					this->Hide();
					this->ClearAsActiveConsumer();
					break;
				}
				default:
				{
					break;
				}
			}
		}
		
		for (auto it = this->cycleButton.begin(); it != this->cycleButton.end(); it++)
		{
			this->HandleClick((*it));
		}
		
		this->Clear();
	}
	else
	{
		this->contextMenu->Update();
	}
	this->contextMenu->Update();
	
	return true;
}

void InventoryDisplay::Render()
{
	if (this->IsDisplayed())
	{
		int x;
		int y;
		
		this->texture->Render();
		this->texture->GetLocation(&x, &y);
		
		x += 207;
		y += 3;
		
		auto& collision = CollisionLayers::GetLayer("InventoryDisplay");
		collision.RemoveAll();
		
		for (size_t i = 0; i < this->wrapped.inventory.Size(); i++)
		{
			int _x = x + ((i % 5) * 67);
			int _y = y + ((i / 5) * 67);
			int _w = Item::SPRITE_SIZE;
			int _h = _w;
			
			Item* item = this->wrapped.inventory.At(i);
			this->slots[i].SetIdx(i);
			this->slots[i].SetItem(item);
			
			SdlRect slotCollision(_x, _y, _w, _h);
			
			if (item)
			{
				item->Move(_x, _y);
				item->Render();
			}
			
			this->slots[i].SetCollision(slotCollision);
			collision.Insert(this->slots[i]); // don't need to worry about adding mutliple times
		}
	}
	
	this->contextMenu->Render();
}

void InventoryDisplay::HandleClick(const SDL_MouseButtonEvent& event)
{
	auto& collision = CollisionLayers::GetLayer("InventoryDisplay");
	
	InventorySlot* slot = nullptr;
	
	Entity entity;
	SdlRect _point(event.x, event.y, 1, 1);
	entity.SetCollision(_point);
	collision.Insert(entity);
	
	EntitySet collisions;
	
	if(collision.CheckCollision(entity, collisions))
	{
		EntityRegistry* registry = EntityRegistry::GetInst();
		for (auto it = collisions.begin(); it != collisions.end(); it++)
		{
			Entity* entity_ = registry->GetEntity((*it));
			slot = dynamic_cast<InventorySlot*>(entity_);
		}
	}
	
	collision.Remove(entity);
	
	if (event.button == SDL_BUTTON_RIGHT)
	{
		if (slot)
		{
			this->contextMenu->Move(_point.x, _point.y);
			this->contextMenu->SetSlot(&this->wrapped, slot);
			this->contextMenu->SetAsActiveConsumer();
			this->contextMenu->Show();
		}
	}
}

/**************************************************/

void InventoryDisplayContextMenu::SetSlot(Inventoried* container, InventorySlot* slot)
{
	for (size_t i = 0; i < this->selections.Size(); i++)
	{
		// ContextMenuItem
		ContextMenuItem* cmItem = this->selections.At(i).Get();
		auto* item_ = dynamic_cast<InventoryDisplayContextMenuItem*>(cmItem);
		if (item_)
		{
			item_->SetSlot(container, slot);
		}
	}
}

void InventoryDisplayContextMenu::Reset()
{
	ContextMenu::Reset();
}

/**************************************************/

InventoryDisplayContextMenuItem::InventoryDisplayContextMenuItem()
{
	this->container = nullptr;
	this->slot = nullptr;
}

void InventoryDisplayContextMenuItem::SetSlot(Inventoried* container, InventorySlot* slot)
{
	this->container = container;
	this->slot = slot;
}

void InventoryDisplayContextMenuItem::Reset()
{
	ContextMenuItem::Reset();
	this->container = nullptr;
	this->slot = nullptr;
}

/**************************************************/

InventoryDisplayUseContextMenuItem::InventoryDisplayUseContextMenuItem()
{
	this->AddText("Use");
}

void InventoryDisplayUseContextMenuItem::OnClicked()
{
	if (this->container && this->slot)
	{
		this->container->Use(this->slot->GetIdx());
	}
}

/**************************************************/

InventoryDisplayTransferContextMenuItem::InventoryDisplayTransferContextMenuItem()
{
	this->AddText("Take");
}

void InventoryDisplayTransferContextMenuItem::OnClicked()
{
	if (this->container && this->slot)
	{
		Item* item = this->slot->GetItem();
		if (item)
		{
			this->container->RemoveItem(item);
			character->AddItem(item);
		}
	}
}

/**************************************************/

InventoryDisplayMoveContextMenuItem::InventoryDisplayMoveContextMenuItem()
{
	this->fromSlot = nullptr;
	this->AddText("Move From");
}

void InventoryDisplayMoveContextMenuItem::OnClicked()
{
	if (this->fromSlot == nullptr)
	{
		this->Clear();
		this->AddText("Move To");
		this->fromSlot = this->slot;
	}
	else
	{
		this->container->inventory.Swap(this->fromSlot->GetIdx(), this->slot->GetIdx());
		this->Clear();
		this->AddText("Move From");
		this->fromSlot = nullptr;
	}
}

void InventoryDisplayMoveContextMenuItem::Reset()
{
	InventoryDisplayContextMenuItem::Reset();
	this->Clear();
	this->AddText("Move From");
	this->fromSlot = nullptr;
}

