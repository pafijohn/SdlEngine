#pragma once


class Renderable
{
	bool deleteLater;
public:
	Renderable();
	virtual ~Renderable();
	void DeleteLater();
	bool IsPendingDelete();
	
	virtual bool Update() = 0;
	virtual void Render() = 0;
};