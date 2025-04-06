#include "Utils.h"
#include "Cloud.h"
#include "Layers.h"

Cloud::Cloud()
{
	this->Load("resources\\clouds.png");
	this->timer.Start(20);
	this->deleteTimer.Start(15000 + GetRandom(3000));
	
	this->Move(GetRandom(20), GetRandom(20));
	this->Crop(0, 0, 64, 64);
	this->Scale(256, 256);
	this->SetAlpha(0x40);
	
	Layers::AddToLayer(this, Layers::WEATHER);
}

bool Cloud::Update()
{
	if (this->timer.IsExpired())
	{
		const SdlRect& rect = this->renderRect;
		this->timer.Start(20);
		this->Move(rect.x + 1, rect.y);
	}
	
	if (this->deleteTimer.IsExpired())
	{
		this->DeleteLater();
	}
	
	return true;
}
