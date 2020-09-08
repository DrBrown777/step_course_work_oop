#include "GameObject.h"

Ball::Ball(b2World& World, Object _playerBall, const float& SCALE)
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

Object Ball::GetObj()
{
    return obj;
}

b2Body* Ball::GetBody()
{
    return body;
}

void Ball::Draw(RenderWindow& window)
{
    window.draw(obj.sprite);
}

Enemy::Enemy(b2World& World, Object _enemy, Vector2i _tileSize, const float& SCALE)
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

Object Enemy::GetObj()
{
    return obj;
}

b2Body* Enemy::GetBody()
{
    return body;
}

void Enemy::Draw(RenderWindow& window)
{
    window.draw(obj.sprite);
}
