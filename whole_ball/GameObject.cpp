#include "GameObject.h"

Object GameObject::GetObj()
{
    return obj;
}

b2Body* GameObject::GetBody()
{
    return body;
}

void GameObject::SetPosObj(float x, float y)
{
    obj.sprite.setPosition(x, y);
}

void GameObject::Draw(RenderWindow& window)
{
    window.draw(obj.sprite);
}

Ball::Ball(b2World& World, const Object& _playerBall, const float& SCALE)
{
    obj = _playerBall;

    b2BodyDef bodyDef;
    b2CircleShape shape;
    b2FixtureDef fixtureDef;

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(obj.rect.left / SCALE, obj.rect.top / SCALE);
    body = World.CreateBody(&bodyDef);
    shape.m_radius = ((obj.rect.height / 2)) / SCALE;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    body->CreateFixture(&fixtureDef);
    body->SetUserData(&obj.name);
}

Enemy::Enemy(b2World& World, const Object& _enemy, const Vector2i& _tileSize, const float& SCALE)
{
    obj = _enemy;

    b2BodyDef bodyDef;
    b2CircleShape shape;

    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((obj.rect.left +
        _tileSize.x / 2 * (obj.rect.width / _tileSize.x - 1)) / SCALE,
        (obj.rect.top + _tileSize.y / 2 * (obj.rect.height / _tileSize.y - 1)) / SCALE);
    body = World.CreateBody(&bodyDef);
    shape.m_radius = ((obj.rect.height / 2) - 5) / SCALE;
    body->CreateFixture(&shape, 1.0f);
    body->SetUserData(&obj.name);
}

SpeedUp::SpeedUp(b2World& World, const Object& _speed, const Vector2i& _tileSize, const float& SCALE)
{
    obj = _speed;
    
    b2BodyDef bodyDef;
    b2PolygonShape shape;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((obj.rect.left +
        _tileSize.x / 2 * (obj.rect.width / _tileSize.x - 1)) / SCALE,
        (obj.rect.top + _tileSize.y / 2 * (obj.rect.height / _tileSize.y - 1)) / SCALE);
    body = World.CreateBody(&bodyDef);
    shape.SetAsBox(obj.rect.width / 2 / SCALE, obj.rect.height / 2 / SCALE);
    body->CreateFixture(&shape, 5.0f);
    body->SetUserData(&obj.name);
    body->SetEnabled(false);
}

Teleport::Teleport(b2World& World, const Object& _teleport, const Vector2i& _tileSize, const float& SCALE)
{
    obj = _teleport;

    b2BodyDef bodyDef;
    b2PolygonShape shape;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((obj.rect.left +
        _tileSize.x / 2 * (obj.rect.width / _tileSize.x - 1)) / SCALE,
        (obj.rect.top + _tileSize.y / 2 * (obj.rect.height / _tileSize.y - 1)) / SCALE);
    body = World.CreateBody(&bodyDef);
    shape.SetAsBox(obj.rect.width / 2 / SCALE, obj.rect.height / 2 / SCALE);
    body->CreateFixture(&shape, 5.0f);
    body->SetUserData(&obj.name);
    body->SetEnabled(false);
}
