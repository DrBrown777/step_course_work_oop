#pragma once
#include <box2d/box2d.h>
#include "Level.h"

class GameObject {
protected:
    Object obj;
    b2Body* body;
public:
    Object GetObj();
    b2Body* GetBody();
    void SetPosObj(float x, float y);
    void Draw(RenderWindow& window);
};

class Ball : public GameObject
{
public:
    Ball(b2World& World, const Object& _playerBall, const float& SCALE);
};

class Enemy : public GameObject
{
public:
    Enemy(b2World& World, const Object& _enemy, const Vector2i& _tileSize, const float& SCALE);
};
