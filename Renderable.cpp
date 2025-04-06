#include "Renderable.h"

Renderable::Renderable()
{
	this->deleteLater = false;
}

Renderable::~Renderable()
{
	
}

void Renderable::DeleteLater()
{
	this->deleteLater = true;
}

bool Renderable::IsPendingDelete()
{
	return this->deleteLater;
}