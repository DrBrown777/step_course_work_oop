#pragma once
#include <box2d/box2d.h>
#include "Level.h"

class GameObject {
protected:
    Object obj;
    b2Body* body;
public:
    virtual Object GetObj() = 0;
    virtual b2Body* GetBody() = 0;
    virtual void Draw(RenderWindow& window) = 0;
};

class Ball : public GameObject
{
public:
    Ball(b2World& World, Object _playerBall, const float& SCALE);
    Object GetObj() override;
    b2Body* GetBody() override;
    void Draw(RenderWindow& window) override;
};

class Enemy : public GameObject
{
public:
    Enemy(b2World& World, Object _enemy, Vector2i _tileSize, const float& SCALE);
    Object GetObj() override;
    b2Body* GetBody() override;
    void Draw(RenderWindow& window) override;
};
