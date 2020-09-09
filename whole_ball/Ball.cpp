//#include "Ball.h"

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
    UP.Set(0.0f, -0.5f); DOWN.Set(0.0f, 0.5f);
    LEFT.Set(-0.5f, 0.0f); RIGHT.Set(0.5f, 0.0f);
}

void Ball::SetDirection()
{
    b2Vec2 vel = pBodyBall->GetLinearVelocity();

    if (directionFlag == false)
    {
        if (vel.x < 0)
        {
            if (vel.x >= -speed.first) { vel.x = -speed.first; }
            if (vel.x <= -speed.second) { vel.x = -speed.second; }
            vel.y = 0;
        }
        else
        {
            if (vel.x <= speed.first) { vel.x = speed.first; }
            if (vel.x >= speed.second) { vel.x = speed.second; }
            vel.y = 0;
        }
    }
    if (directionFlag == true)
    {
        if (vel.y < 0)
        {
            if (vel.y >= -speed.first) { vel.y = -speed.first; }
            if (vel.y <= -speed.second) { vel.y = -speed.second; }
            vel.x = 0;
        }
        else
        {
            if (vel.y <= speed.first) { vel.y = speed.first; }
            if (vel.y >= speed.second) { vel.y = speed.second; }
            vel.x = 0;
        }
    }
    pBodyBall->SetLinearVelocity(vel);

    vel.Normalize();
}

void Ball::SetSpeed(double _speedMin, double _speedMax)
{
    speed.first = _speedMin;
    speed.second = _speedMax;
}

void Ball::UpdatePosition(const float& SCALE)
{
    b2Vec2 pos = pBodyBall->GetPosition();
    playerBall.sprite.setPosition(pos.x * SCALE, pos.y * SCALE);
}

void Ball::CheckCollision(Enemy& energyPills, Batty& _platform)
{
    for (b2ContactEdge* ce = pBodyBall->GetContactList(); ce; ce = NULL)
    {
        b2Contact* c = ce->contact;

        vector <Object> enemy = energyPills.GetEnemy();
        vector <b2Body*> enemyBody = energyPills.GetEnemyBody();

        for (int i = 0; i < enemyBody.size(); i++)
            if (c->GetFixtureB() == enemyBody[i]->GetFixtureList())
            {
                enemyBody[i]->DestroyFixture(enemyBody[i]->GetFixtureList());
                enemy.erase(enemy.begin() + i);
                enemyBody.erase(enemyBody.begin() + i);
            }

        energyPills.SetEnemy(enemy);
        energyPills.SetEnemyBody(enemyBody);
    }
    
    list <Object> platform = _platform.GetPlatform();

    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        if (it->move == true) continue;

        if (it->sprite.getGlobalBounds().intersects(playerBall.sprite.getGlobalBounds()))
        {
            b2Vec2 dir = pBodyBall->GetLinearVelocity();

            if (it->sprite.getRotation() == 45 || it->sprite.getRotation() == 225)
            {
                if (dir.x < 0 && dir.y == 0)
                {
                    pBodyBall->SetLinearVelocity(DOWN);
                    directionFlag = true;
                }
                else if (dir.x > 0 && dir.y == 0)
                {
                    pBodyBall->SetLinearVelocity(UP);
                    directionFlag = true;
                }
                else if (dir.y < 0 && dir.x == 0)
                {
                    pBodyBall->SetLinearVelocity(RIGHT);
                    directionFlag = false;
                }
                else if (dir.y > 0 && dir.x == 0)
                {
                    pBodyBall->SetLinearVelocity(LEFT);
                    directionFlag = false;
                }
            }
            else if (it->sprite.getRotation() == 315 || it->sprite.getRotation() == 135)
            {
                if (dir.x < 0 && dir.y == 0)
                {
                    pBodyBall->SetLinearVelocity(UP);
                    directionFlag = true;
                }
                else if (dir.x > 0 && dir.y == 0)
                {
                    pBodyBall->SetLinearVelocity(DOWN);
                    directionFlag = true;
                }
                else if (dir.y < 0 && dir.x == 0)
                {
                    pBodyBall->SetLinearVelocity(LEFT);
                    directionFlag = false;
                }
                else if (dir.y > 0 && dir.x == 0)
                {
                    pBodyBall->SetLinearVelocity(RIGHT);
                    directionFlag = false;
                }
            }
            SetDirection();
        }
    }
}

void Ball::Draw(RenderWindow& window)
{
    window.draw(playerBall.sprite);
}
