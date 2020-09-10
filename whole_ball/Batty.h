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
	void InitPlatform();
	list <Object> GetPlatform();
	void StatePlatform(const Vector2i& mousePos, const Event& event);
	void DestroyPlatform();
	void UpdatePosition(const Vector2i& mousePos);
	void Draw(RenderWindow& window);
};