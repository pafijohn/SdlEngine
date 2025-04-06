#include <cstdio>

#include "Hud.h"
#include "Text.h"
#include "Layers.h"
#include "Character.h"
#include "SdlGlobals.h"
#include "TextManager.h"

using namespace Pointers;

Hud* hud = nullptr;

Hud::Hud()
{
	Layers::AddToLayer(this, Layers::HUD);
	
	int width;
	int height;
	
	int swidth;
	int sheight;
	
	SdlGlobals::GetScreenDimentions(swidth, sheight);

	this->backpack = new SdlTexture();
	this->backpack->isRelative = false;
	this->backpack->Load("resources\\Items\\backpack.png");
	this->backpack->GetDimentions(&width, &height);
	this->backpack->Move(swidth - width, sheight - height);
	
	this->inventoryDisplay = new InventoryDisplay(*character);
}

Hud::~Hud()
{
	delete this->inventoryDisplay;
}

void Hud::OnClick(const SDL_MouseButtonEvent& event)
{
	SDL_Rect result;
	SDL_Point _point = { event.x, event.y };
	
	if (this->inventoryDisplay->IsDisplayed())
	{
		this->inventoryDisplay->HandleClick(event);
	}
	else if (SDL_EnclosePoints(&_point, 1, &this->backpack->renderRect, &result) && event.button == SDL_BUTTON_LEFT)
	{
		this->inventoryDisplay->ToggleDisplayed();
	}
}

bool Hud::HideInventory()
{
	bool canHide = this->inventoryDisplay->IsDisplayed();
	
	if (canHide)
	{
		this->inventoryDisplay->Hide();
	}
	
	return canHide;
}

bool Hud::Update()
{
	return true;
}

void Hud::Render()
{
	char buff[256];
	
	Text status;
	
	sprintf_s(buff, "Health: %.02f", character->health);
	status.AddText(buff, TextManager::MakeIdentifier(TextManager::COURIER_NEW_B, SdlColor(0xFF, 0, 0), 36));
	status.SetBackground(Text::GREY);
	
	sprintf_s(buff, "Stamina: %.02f", character->stamina);
	status.AddText(buff, TextManager::MakeIdentifier(TextManager::COURIER_NEW_B, SdlColor(0, 0xFF, 0), 36));
	
	sprintf_s(buff, "Mana: %.02f", character->mana);
	status.AddText(buff, TextManager::MakeIdentifier(TextManager::COURIER_NEW_B, SdlColor(0, 0, 0xFF), 36));
	
	status.Move(50, (4.0 / 5.0) * 1080);
	
	status.Render();
	
	this->backpack->Render();
}
