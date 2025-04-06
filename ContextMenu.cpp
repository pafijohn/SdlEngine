#include "Layers.h"
#include "TextManager.h"
#include "ContextMenu.h"
#include "CollisionLayers.h"

using namespace Pointers;
using namespace Containers;


const std::string ContextMenuItem::DEFAULT_IDENT = "C:\\WINDOWS\\Fonts\\cour.ttf|255|255|0|0|18";

ContextMenuItem::ContextMenuItem()
{
	//this->SetBackground(Text::BROWN);
	this->SetHasCollision(true);
}

void ContextMenuItem::AddText(const std::string& text, const std::string& identifier)
{
	const std::string& ident = identifier.empty() ? ContextMenuItem::DEFAULT_IDENT : identifier;
	Text::AddText(text, ident);
}

void ContextMenuItem::OnClicked()
{
	
}

void ContextMenuItem::Reset()
{
	
}

ContextMenu::ContextMenu()
{
	int width;
	int height;
	
	CollisionLayers::CreateLayer("ContextMenu");
	SdlGlobals::GetScreenDimentions(width, height);

	auto& collision = CollisionLayers::GetLayer("ContextMenu");
	collision.SetRect(SdlRect(0, 0, width, height));
	
	this->background = create<SdlTexture>();
	this->background->CreateRGBTexture(10, 10, SdlColor(128, 128, 128));
	this->background->isRelative = false;
	
	this->highlight = create<SdlTexture>();
	this->highlight->CreateRGBTexture(10, 10, SdlColor(100, 100, 100));
	this->highlight->isRelative = false;
	
	Layers::AddToLayer(this, Layers::HUD);
}

ContextMenuItem* ContextMenu::GetItemAt(const SDL_Point& point)
{
	Entity entity;
	EntitySet collisions;
	SdlRect point_(point.x, point.y, 1, 1);
	auto& collision = CollisionLayers::GetLayer("ContextMenu");
	ContextMenuItem* item = nullptr;
	
	entity.SetCollision(point_);
	collision.Insert(entity);

	if (collision.CheckCollision(entity, collisions))
	{
		auto collision_ = *collisions.begin(); // shouldnt be able to click more than 1 item
		Entity* entity_ = EntityRegistry::GetInst()->GetEntity(collision_);
		ContextMenuItem* item_ = dynamic_cast<ContextMenuItem*>(entity_);
		if (item_)
		{
			item = item_;
		}
	}
	
	collision.Remove(entity);
	
	return item;
}

void ContextMenu::AddOption(ContextMenuItem* item)
{
	if (item)
	{
		this->selections.PushBack(item);
		//this->CalculatePositions();
	}
}

void ContextMenu::CalculatePositions()
{
	const int PADDING = 10;
	int maxWidth = 0;
	int maxHeight = 0;
	auto& collision = CollisionLayers::GetLayer("ContextMenu");
	SDL_Point pos = this->GetPos();
	background->Move(pos);
	pos.x += PADDING; // for background padding
	
	collision.RemoveAll();
	
	// Add selections to the collision
	for (size_t i = 0; i < this->selections.Size(); i++)
	{
		SdlRect rect;
		
		auto& item = this->selections.At(i).GetRef();
		
		item.GetRect(rect); // width and height
		maxWidth = MAX(maxWidth, rect.w);
		maxHeight += rect.h;
		
		rect.x = pos.x; // relative position
		rect.y = pos.y;
		
		pos.y += rect.h;
		
		item.Move(rect.x, rect.y);
		item.SetCollision(rect);
		collision.Insert(item);
	}
	
	// normalize collisions
	for (size_t i = 0; i < this->selections.Size(); i++)
	{
		auto& item = this->selections.At(i).GetRef();
		SdlRect rect = item.GetCollision();
		rect.x -= PADDING;
		rect.w = maxWidth + (PADDING * 2);
		item.SetCollision(rect);
	}
	
	background->Scale(maxWidth + (PADDING * 2), maxHeight);
}

void ContextMenu::OnMove()
{
	this->CalculatePositions();
}

void ContextMenu::Reset()
{
	for (size_t i = 0; i < this->selections.Size(); i++)
	{
		auto& item = this->selections.At(i).GetRef();
		item.Reset();
	}
}

bool ContextMenu::Update()
{
	if (this->IsActiveConsumer())
	{
		for (auto it = this->cycleButton.begin(); it != this->cycleButton.end(); it++)
		{
			const SDL_MouseButtonEvent& event = (*it);
			SDL_Point point = {event.x, event.y};
			ContextMenuItem* item = this->GetItemAt(point);
			
			if (item)
			{
				item->OnClicked();
			}
			
			this->ClearAsActiveConsumer();
			this->Hide();
			
			break;
		}
		
		this->Clear();
	}
	
	return true;
}

void ContextMenu::Render()
{
	if (this->IsDisplayed())
	{
		SDL_Point point;
		SDL_GetMouseState(&point.x, &point.y);
		this->background->Render();
		
		ContextMenuItem* item = this->GetItemAt(point);
		if (item)
		{
			const SdlRect& collision = item->GetCollision();
			this->highlight->Move(collision.x, collision.y);
			this->highlight->Scale(collision.w, collision.h);
			this->highlight->Render();
		}
		
		for (size_t i = 0; i < this->selections.Size(); i++)
		{
			auto& item = this->selections.At(i).GetRef();
			item.Render();
		}
	}
}
