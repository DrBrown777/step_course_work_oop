#include "Control.h"

Control::Control()
{
    playerBall = nullptr;
	ballSpeed = make_pair(0, 0);

	ballDirFlag = false;

	UP.Set(0.0f, -0.5f); DOWN.Set(0.0f, 0.5f);
	LEFT.Set(-0.5f, 0.0f); RIGHT.Set(0.5f, 0.0f);
}

void Control::InitBall(b2World& World, const Object& _playerBall, const float& SCALE)
{
    playerBall = new Ball(World, _playerBall, SCALE);
    SetSpeedBall(0, 0);
}

void Control::InitEnergyPills(b2World& World, const vector<Object>& _enemy, const Vector2i& _tileSize, const float& SCALE)
{
    for (int i = 0; i < _enemy.size(); i++)
    {
        energyPills.push_back(new Enemy(World, _enemy[i], _tileSize, SCALE));
    }
}

void Control::SetSpeedBall(double _speedMin, double _speedMax)
{
	ballSpeed.first = _speedMin;
	ballSpeed.second = _speedMax;
}

void Control::SetDirectionBall()
{
    b2Vec2 vel = playerBall->GetBody()->GetLinearVelocity();

    if (ballDirFlag == false)
    {
        if (vel.x < 0)
        {
            if (vel.x >= -ballSpeed.first) { vel.x = -ballSpeed.first; }
            if (vel.x <= -ballSpeed.second) { vel.x = -ballSpeed.second; }
            vel.y = 0;
        }
        else
        {
            if (vel.x <= ballSpeed.first) { vel.x = ballSpeed.first; }
            if (vel.x >= ballSpeed.second) { vel.x = ballSpeed.second; }
            vel.y = 0;
        }
    }
    if (ballDirFlag == true)
    {
        if (vel.y < 0)
        {
            if (vel.y >= -ballSpeed.first) { vel.y = -ballSpeed.first; }
            if (vel.y <= -ballSpeed.second) { vel.y = -ballSpeed.second; }
            vel.x = 0;
        }
        else
        {
            if (vel.y <= ballSpeed.first) { vel.y = ballSpeed.first; }
            if (vel.y >= ballSpeed.second) { vel.y = ballSpeed.second; }
            vel.x = 0;
        }
    }

    playerBall->GetBody()->SetLinearVelocity(vel);
    
    vel.Normalize();
}

pair<double, double> Control::GetSpeedBall()
{
    return ballSpeed;
}

vector<GameObject*> Control::GetEnergyPills()
{
    return energyPills;
}

void Control::UpdatePositionBall(const float& SCALE)
{
    b2Vec2 pos = playerBall->GetBody()->GetPosition();

    playerBall->SetPosObj(pos.x * SCALE, pos.y * SCALE);
}

void Control::CheckCollisionBall(b2World& World, list <Object> platform)
{
    for (b2ContactEdge* ce = playerBall->GetBody()->GetContactList(); ce; ce = NULL)
    {
        b2Contact* c = ce->contact;

        for (int i = 0; i < energyPills.size(); i++)
        {
            if ( (c->GetFixtureA() == energyPills[i]->GetBody()->GetFixtureList()) || (c->GetFixtureB() == energyPills[i]->GetBody()->GetFixtureList()) )
            {
                World.DestroyBody(energyPills.at(i)->GetBody());
                delete energyPills.at(i);
                energyPills.erase(energyPills.begin() + i);
            }
        }
    }

    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        if (it->move == true) continue;

        if (it->sprite.getGlobalBounds().intersects(playerBall->GetObj().sprite.getGlobalBounds()))
        {
            b2Vec2 dir = playerBall->GetBody()->GetLinearVelocity();

            if (it->sprite.getRotation() == 45 || it->sprite.getRotation() == 225)
            {
                if (dir.x < 0 && dir.y == 0)
                {
                    playerBall->GetBody()->SetLinearVelocity(DOWN);
                    ballDirFlag = true;
                }
                else if (dir.x > 0 && dir.y == 0)
                {
                    playerBall->GetBody()->SetLinearVelocity(UP);
                    ballDirFlag = true;
                }
                else if (dir.y < 0 && dir.x == 0)
                {
                    playerBall->GetBody()->SetLinearVelocity(RIGHT);
                    ballDirFlag = false;
                }
                else if (dir.y > 0 && dir.x == 0)
                {
                    playerBall->GetBody()->SetLinearVelocity(LEFT);
                    ballDirFlag = false;
                }
            }
            else if (it->sprite.getRotation() == 315 || it->sprite.getRotation() == 135)
            {
                if (dir.x < 0 && dir.y == 0)
                {
                    playerBall->GetBody()->SetLinearVelocity(UP);
                    ballDirFlag = true;
                }
                else if (dir.x > 0 && dir.y == 0)
                {
                    playerBall->GetBody()->SetLinearVelocity(DOWN);
                    ballDirFlag = true;
                }
                else if (dir.y < 0 && dir.x == 0)
                {
                    playerBall->GetBody()->SetLinearVelocity(LEFT);
                    ballDirFlag = false;
                }
                else if (dir.y > 0 && dir.x == 0)
                {
                    playerBall->GetBody()->SetLinearVelocity(RIGHT);
                    ballDirFlag = false;
                }
            }
            SetDirectionBall();
        }
    }
}

void Control::DestroyObjects(b2World& World)
{
    World.DestroyBody(playerBall->GetBody());
    delete playerBall;

    ballDirFlag = false;
    ballSpeed.first, ballSpeed.second = 0;

    for (int i = 0; i < energyPills.size(); i++)
    {
        World.DestroyBody(energyPills.at(i)->GetBody());
        delete energyPills.at(i);
    }

    energyPills.clear();
}

void Control::DrawGameObject(RenderWindow& window)
{
    window.draw(playerBall->GetObj().sprite);

    for (const auto& obj : energyPills)
    {
        window.draw(obj->GetObj().sprite);
    }
}

Control::~Control()
{
	delete playerBall;
   
	for (auto obj : energyPills)
	{
        delete obj;
	}
}
