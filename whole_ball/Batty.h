#pragma once
#include "Level.h"
#include <list>

class Batty
{
private:
	Object bat;
	Texture platformImg;
	list <Object> platform;
public:
	Batty();
	list <Object> GetPlatform();
	void InitPlatform(const Vector2i& mousePos, const Event& event);
	void UpdatePosition(const Vector2i& mousePos);
	void Draw(RenderWindow& window);
};