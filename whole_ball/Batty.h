#pragma once
#include "Level.h"
#include <list>

class Batty
{
private:
	Object bat;
	b2Body* batBody;
	Texture platformImg;
	list <pair <Object , b2Body*>> platform;
public:
	Batty(b2World& World, const float& SCALE);
	list <pair <Object, b2Body*>> GetPlatform();
	void AddPlatform();
	void StatePlatform(const Vector2i& mousePos, const Event& event);
	void DestroyPlatform();
	void UpdatePosition(const Vector2i& mousePos, const float& SCALE);
	void Draw(RenderWindow& window);
};