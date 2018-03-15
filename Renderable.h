#pragma once


class Renderable
{
public:
	virtual bool Update() = 0;
	virtual void Render() = 0;
};