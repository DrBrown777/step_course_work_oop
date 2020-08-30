#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Level.h"
#include <list>

using namespace sf;
using namespace std;

const float SCALE = 30.f; //перевод из метров в px в box2d
const float DEG = 57.29577f; //перевод из радиан в градусы в box2d

b2Vec2 Gravity(0.0f, 0.0f);
b2World World(Gravity);

Object getPlatform(Texture& img)
{
    Object obj;

    obj.name = "platform";
    obj.type = "solid";
    obj.rect = Rect<int>(0, 0, 13, 48);
    obj.sprite.setTexture(img);
    obj.sprite.setTextureRect(obj.rect);
    obj.sprite.setOrigin(obj.rect.width / 2, obj.rect.height / 2);
    obj.sprite.setPosition(100, 85);
    obj.sprite.rotate(45);

    return obj;
}

int main()
{
    double speedMIN = 0, speedMAX = 0;
    bool directionFlag = false;

    Level lvl;

    lvl.LoadFromFile("LevelOne/level1.tmx");

    RenderWindow window;
    window.create(VideoMode(1024, 768), "Whole Ball Game v.1.0");
    //window.setFramerateLimit(60);
    
    /*Создаем обьект шар*/
    Object playerBall = lvl.GetObject("ball");

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(playerBall.rect.left / SCALE, playerBall.rect.top / SCALE);
    b2Body* pBodyBall = World.CreateBody(&bodyDef);
    b2CircleShape shape;
    shape.m_radius = ((playerBall.rect.height / 2)) / SCALE;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    pBodyBall->CreateFixture(&fixtureDef);
    pBodyBall->SetUserData(&playerBall.name);

    /*Создаем обьект платформа*/
    Texture platformImg; 
    platformImg.loadFromFile("image/platform.png");
    list <Object> platform;
    platform.push_back(getPlatform(platformImg));
  
    /*Создаем обьект враг*/
    vector <Object> enemy;
    vector <b2Body*> enemyBody;
    Vector2i tileSize = lvl.GetTileSize();

    enemy = lvl.GetObjects("enemy");

    for (int i = 0; i < enemy.size(); i++)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set((enemy[i].rect.left +
            tileSize.x / 2 * (enemy[i].rect.width / tileSize.x - 1)) / SCALE,
            (enemy[i].rect.top + tileSize.y / 2 * (enemy[i].rect.height / tileSize.y - 1)) / SCALE);
        b2Body* body = World.CreateBody(&bodyDef);
        b2CircleShape shape;
        shape.m_radius = ((enemy[i].rect.height / 2) - 5) / SCALE;
        body->CreateFixture(&shape, 1.0f);
        body->SetUserData(&enemy[i].name);

        enemyBody.push_back(body);
    }

    /*Создаем обьект стена*/
    vector<Object> wall = lvl.GetObjects("solid");
    vector <b2Body*> wallBody;

    for (int i = 0; i < wall.size(); i++)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set((wall[i].rect.left + tileSize.x / 2 * (wall[i].rect.width / tileSize.x - 1)) / SCALE,
            (wall[i].rect.top + tileSize.y / 2 * (wall[i].rect.height / tileSize.y - 1)) / SCALE);
        b2Body* body = World.CreateBody(&bodyDef);
        b2PolygonShape shape;
        shape.SetAsBox(wall[i].rect.width / 2 / SCALE, wall[i].rect.height / 2 / SCALE);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        body->CreateFixture(&fixtureDef);
        body->SetUserData(&wall[i].name);
        wallBody.push_back(body);
    }

    while (window.isOpen())
    {
        Event event;

        Vector2i mousePos = Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyPressed:
                if (event.key.code == Keyboard::Space)
                {
                    pBodyBall->SetLinearVelocity(b2Vec2(0.5f, 0.0f));
                    speedMIN = 0.45, speedMAX = 0.5;
                }
                break;
            case Event::MouseButtonReleased:
                for (auto it = platform.begin(); it != platform.end(); it++)
                {
                    if (it->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && event.key.code == Mouse::Right)
                    {
                        it->sprite.rotate(45);
                        break;
                    }
                    if (it->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && event.key.code == Mouse::Middle)
                    {
                        platform.erase(it);
                        break;
                    }
                }
                if (platform.empty())
                    platform.push_back(getPlatform(platformImg));
                else if (platform.back().sprite.getPosition().y > 100)
                    platform.push_back(getPlatform(platformImg));
                break;
            }
        }

        /*Смена направления в зависимости от скорости*/
        b2Vec2 vel = pBodyBall->GetLinearVelocity();
        if (directionFlag == false)
        {
            if (vel.x < 0)
            {
                if (vel.x >= -speedMIN) { vel.x = -speedMIN; }
                if (vel.x <= -speedMAX) { vel.x = -speedMAX;  }
                vel.y = 0;
            }
            else
            {
                if (vel.x <= speedMIN) { vel.x = speedMIN; }
                if (vel.x >= speedMAX) { vel.x = speedMAX;  }
                vel.y = 0;
            }
        }
        if (directionFlag == true)
        {
            if (vel.y < 0)
            {
                if (vel.y >= -speedMIN) { vel.y = -speedMIN; }
                if (vel.y <= -speedMAX) { vel.y = -speedMAX;  }
                vel.x = 0;
            }
            else
            {
                if (vel.y <= speedMIN) { vel.y = speedMIN; }
                if (vel.y >= speedMAX) { vel.y = speedMAX;  }
                vel.x = 0;
            }
        }
        pBodyBall->SetLinearVelocity(vel);
        vel.Normalize();

        /*Проверка на столкновения с шарами*/
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

        World.Step(1 / 60.f, 8, 3);
        
        window.clear();

        b2Vec2 pos = pBodyBall->GetPosition();
        playerBall.sprite.setPosition(pos.x* SCALE, pos.y* SCALE);
        
        /*Перемещение платформы*/
        for (auto it = platform.begin(); it != platform.end(); it++)
        {
            if (it->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mouse::isButtonPressed(Mouse::Left))
            {
                it->sprite.setPosition(mousePos.x, mousePos.y);
                break;
            }
        }

        /*Проверка на столкновение с платформами*/
        for (auto it = platform.begin(); it != platform.end(); it++)
        {
            if (it->sprite.getGlobalBounds().intersects(playerBall.sprite.getGlobalBounds()))
            {
                b2Vec2 dir = pBodyBall->GetLinearVelocity();

                if (it->sprite.getRotation() == 45 || it->sprite.getRotation() == 225)
                {
                    if (dir.x < 0 && dir.y == 0)
                    {
                        pBodyBall->SetLinearVelocity(b2Vec2(0.0f, 0.5f));
                        directionFlag = true;
                    }
                    else if (dir.x > 0 && dir.y == 0)
                    {
                        pBodyBall->SetLinearVelocity(b2Vec2(0.0f, -0.5f));
                        directionFlag = true;
                    }
                    else if (dir.y < 0 && dir.x == 0)
                    {
                        pBodyBall->SetLinearVelocity(b2Vec2(0.5f, 0.0f));
                        directionFlag = false;
                    }
                    else if (dir.y > 0 && dir.x == 0)
                    {
                        pBodyBall->SetLinearVelocity(b2Vec2(-0.5f, 0.0f));
                        directionFlag = false;
                    }
                }
                else if (it->sprite.getRotation() == 315 || it->sprite.getRotation() == 135)
                {
                    if (dir.x < 0 && dir.y == 0)
                    {
                        pBodyBall->SetLinearVelocity(b2Vec2(0.0f, -0.5f));
                        directionFlag = true;
                    }
                    else if (dir.x > 0 && dir.y == 0)
                    {
                        pBodyBall->SetLinearVelocity(b2Vec2(0.0f, 0.5f));
                        directionFlag = true;
                    }
                    else if (dir.y < 0 && dir.x == 0)
                    {
                        pBodyBall->SetLinearVelocity(b2Vec2(-0.5f, 0.0f));
                        directionFlag = false;
                    }
                    else if (dir.y > 0 && dir.x == 0)
                    {
                        pBodyBall->SetLinearVelocity(b2Vec2(0.5f, 0.0f));
                        directionFlag = false;
                    }
                }
            }
        }

        lvl.Draw(window);

        window.draw(playerBall.sprite);

        for (const auto& obj : enemy)
        {
            window.draw(obj.sprite);
        }

        for (const auto& obj : platform)
        {
            window.draw(obj.sprite);
        }

        window.display();
    }

    return 0;
}