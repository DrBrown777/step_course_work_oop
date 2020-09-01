#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Level.h"
#include "Ball.h"
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
    RenderWindow window;
    window.create(VideoMode(1024, 768), "Whole Ball Game v.1.0");

    /*Создаем обьект Уровень*/
    Level lvl;

    lvl.LoadFromFile("LevelOne/level1.tmx");

    /*Создаем обьект шар*/
    Ball playerBall(World, lvl.GetObject("ball"), SCALE);
    
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
                    playerBall.SetLinVel(b2Vec2(0.5f, 0.0f));
                    playerBall.SetSpeed();
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
                for (auto it = platform.begin(); it != platform.end(); it++)
                {
                    if (it->sprite.getPosition().y > 100)
                        it->direct = false;
                }
                break;
            }
        }

        /*Смена направления в зависимости от скорости*/
        playerBall.SetDirection();

        /*Проверка на столкновения с шарами*/
        playerBall.CheckCollisionEnemy(enemy, enemyBody);

        World.Step(1 / 60.f, 8, 3);
        
        window.clear();

        playerBall.UpdatePosition(SCALE);

        /*Перемещение платформы*/
        for (auto it = platform.begin(); it != platform.end(); it++)
        {   
            if (it->direct == false) continue;
            if (it->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mouse::isButtonPressed(Mouse::Left))
            {
                it->sprite.setPosition(mousePos.x, mousePos.y);
                break;
            }
        }

        /*Проверка на столкновение с платформами*/
        playerBall.CheckCollisionPlatform(platform);

        lvl.Draw(window);

        playerBall.Draw(window);

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