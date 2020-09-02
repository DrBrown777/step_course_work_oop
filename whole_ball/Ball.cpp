#include "Ball.h"

Ball::Ball(b2World& World, Object _playerBall, const float& SCALE)
{
	playerBall = _playerBall;

    b2BodyDef bodyDef;
    b2CircleShape shape;
    b2FixtureDef fixtureDef;

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(playerBall.rect.left / SCALE, playerBall.rect.top / SCALE);
    pBodyBall = World.CreateBody(&bodyDef);
    shape.m_radius = ((playerBall.rect.height / 2)) / SCALE;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    pBodyBall->CreateFixture(&fixtureDef);
    pBodyBall->SetUserData(&playerBall.name);

    speed = make_pair(0, 0);

    directionFlag = false;
}

void Ball::SetLinVel(b2Vec2 _linearVelocity)
{
    pBodyBall->SetLinearVelocity(_linearVelocity);
}

void Ball::SetSpeed(double _speedMin, double _speedMax)
{
    SetLinVel(b2Vec2(0.5f, 0.0f));
    speed.first = _speedMin;
    speed.second = _speedMax;
}

void Ball::SetDirection()
{
    b2Vec2 vel = GetLinVel();
    pair <double, double> spd = GetSpeed();

    if (GetDirFlag() == false)
    {
        if (vel.x < 0)
        {
            if (vel.x >= -spd.first) { vel.x = -spd.first; }
            if (vel.x <= -spd.second) { vel.x = -spd.second; }
            vel.y = 0;
        }
        else
        {
            if (vel.x <= spd.first) { vel.x = spd.first; }
            if (vel.x >= spd.second) { vel.x = spd.second; }
            vel.y = 0;
        }
    }
    if (GetDirFlag() == true)
    {
        if (vel.y < 0)
        {
            if (vel.y >= -spd.first) { vel.y = -spd.first; }
            if (vel.y <= -spd.second) { vel.y = -spd.second; }
            vel.x = 0;
        }
        else
        {
            if (vel.y <= spd.first) { vel.y = spd.first; }
            if (vel.y >= spd.second) { vel.y = spd.second; }
            vel.x = 0;
        }
    }
    SetLinVel(vel);

    vel.Normalize();
}

void Ball::UpdatePosition(const float& SCALE)
{
    b2Vec2 pos = pBodyBall->GetPosition();
    playerBall.sprite.setPosition(pos.x * SCALE, pos.y * SCALE);
}

void Ball::SetDirFlag(bool _flag)
{
    directionFlag = _flag;
}

b2Vec2 Ball::GetLinVel()
{
    return pBodyBall->GetLinearVelocity();
}

pair<double, double> Ball::GetSpeed()
{
    return speed;
}

bool Ball::GetDirFlag()
{
    return directionFlag;
}

void Ball::CheckCollision(vector<Object>& enemy, vector<b2Body*>& enemyBody, const list <Object>& platform)
{
    for (b2ContactEdge* ce = pBodyBall->GetContactList(); ce; ce = NULL)
    {
        b2Contact* c = ce->contact;

        for (int i = 0; i < enemyBody.size(); i++)
            if (c->GetFixtureB() == enemyBody[i]->GetFixtureList())
            {
                enemyBody[i]->DestroyFixture(enemyBody[i]->GetFixtureList());
                enemy.erase(enemy.begin() + i);
                enemyBody.erase(enemyBody.begin() + i);
            }
    }

    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        if (it->sprite.getGlobalBounds().intersects(playerBall.sprite.getGlobalBounds()))
        {
            b2Vec2 dir = GetLinVel();

            if (it->sprite.getRotation() == 45 || it->sprite.getRotation() == 225)
            {
                if (dir.x < 0 && dir.y == 0)
                {
                    SetLinVel(b2Vec2(0.0f, 0.5f));
                    SetDirFlag(true);
                }
                else if (dir.x > 0 && dir.y == 0)
                {
                    SetLinVel(b2Vec2(0.0f, -0.5f));
                    SetDirFlag(true);
                }
                else if (dir.y < 0 && dir.x == 0)
                {
                    SetLinVel(b2Vec2(0.5f, 0.0f));
                    SetDirFlag(false);
                }
                else if (dir.y > 0 && dir.x == 0)
                {
                    SetLinVel(b2Vec2(-0.5f, 0.0f));
                    SetDirFlag(false);
                }
            }
            else if (it->sprite.getRotation() == 315 || it->sprite.getRotation() == 135)
            {
                if (dir.x < 0 && dir.y == 0)
                {
                    SetLinVel(b2Vec2(0.0f, -0.5f));
                    SetDirFlag(true);
                }
                else if (dir.x > 0 && dir.y == 0)
                {
                    SetLinVel(b2Vec2(0.0f, 0.5f));
                    SetDirFlag(true);
                }
                else if (dir.y < 0 && dir.x == 0)
                {
                    SetLinVel(b2Vec2(-0.5f, 0.0f));
                    SetDirFlag(false);
                }
                else if (dir.y > 0 && dir.x == 0)
                {
                    SetLinVel(b2Vec2(0.5f, 0.0f));
                    SetDirFlag(false);
                }
            }
        }
    }
}

void Ball::Draw(RenderWindow& window)
{
    window.draw(playerBall.sprite);
}
