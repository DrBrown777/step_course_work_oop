#include "Control.h"

Control::Control()
{
    playerBall = nullptr;
    speedUp = nullptr;
    teleport = make_pair(nullptr, nullptr);
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

void Control::InitSpeedUp(b2World& World, const Object& _speed, const Vector2i& _tileSize, const float& SCALE)
{
    if (_speed.name == "")
        return;
    speedUp = new SpeedUp(World, _speed, _tileSize, SCALE);
}

void Control::InitTeleport(b2World& World, const vector<Object>& _teleport, const Vector2i& _tileSize, const float& SCALE)
{
    if (_teleport.empty()) 
        return;
    teleport.first = new Teleport(World, _teleport.at(0), _tileSize, SCALE);
    teleport.second = new Teleport(World, _teleport.at(1), _tileSize, SCALE);
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

    if (teleport.first != nullptr && port == false)
    {
        bool tmp1 = playerBall->GetObj().sprite.getGlobalBounds().intersects(teleport.first->GetObj().sprite.getGlobalBounds());
        bool tmp2 = playerBall->GetObj().sprite.getGlobalBounds().intersects(teleport.second->GetObj().sprite.getGlobalBounds());
        
        if (tmp1 == false && tmp2 == false)
        {
            port = true;
        }
    }
}

void Control::CheckCollisionBall(b2World& World, const list <Object>& platform, const float& SCALE, GameStatistics& stata, Sound& sound)
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
                stata.SetScoreUp();
                sound.play();
            }
        }
    }

    if (speedUp != nullptr)
    {
        if (playerBall->GetObj().sprite.getGlobalBounds().intersects(speedUp->GetObj().sprite.getGlobalBounds()))
        {
            SetSpeedBall(1.8f, 2.0f);
        }
    }

    if (teleport.first != nullptr && port == true)
    {
        Vector2f posFirst = teleport.first->GetObj().sprite.getPosition();
        Vector2f posSecond = teleport.second->GetObj().sprite.getPosition();

        if (playerBall->GetObj().sprite.getGlobalBounds().intersects(teleport.first->GetObj().sprite.getGlobalBounds()))
        {
            playerBall->GetBody()->SetTransform(b2Vec2(posSecond.x / SCALE, posSecond.y / SCALE), 0.f);
            port = false;
        }
        else if (playerBall->GetObj().sprite.getGlobalBounds().intersects(teleport.second->GetObj().sprite.getGlobalBounds()))
        {
            playerBall->GetBody()->SetTransform(b2Vec2(posFirst.x / SCALE, posFirst.y / SCALE), 0.f);
            port = false;
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

    if (speedUp != nullptr)
    {
        World.DestroyBody(speedUp->GetBody());
        delete speedUp;
    }

    if (teleport.first != nullptr)
    {
        World.DestroyBody(teleport.first->GetBody());
        World.DestroyBody(teleport.second->GetBody());
        delete teleport.first;
        delete teleport.second;
    }
}

void Control::DrawGameObject(RenderWindow& window)
{
    playerBall->Draw(window);

    for (const auto& obj : energyPills)
    {
        obj->Draw(window);
    }
    
    if (speedUp != nullptr) 
        speedUp->Draw(window);

    if (teleport.first != nullptr)
    {
        teleport.first->Draw(window);
        teleport.second->Draw(window);
    }
}

Control::~Control()
{
	delete playerBall;
   
	for (auto obj : energyPills)
	{
        delete obj;
	}

    if (speedUp != nullptr) 
        delete speedUp;

    if (teleport.first != nullptr)
    {
        delete teleport.first;
        delete teleport.second;
    }
}
