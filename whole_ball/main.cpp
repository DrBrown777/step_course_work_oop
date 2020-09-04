#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Level.h"
#include "Ball.h"
#include "Enemy.h"
#include <list>

using namespace sf;
using namespace std;

const float SCALE = 30.f; // conversion from meters to px in box2d
const float DEG = 57.29577f; // convert from radians to degrees in box2d

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

    /* Create a Map object */
    Level lvl;
    lvl.LoadFromFile("LevelOne/level1.tmx", World, SCALE);

    /*Create a ball object*/
    Ball playerBall(World, lvl.GetObject("ball"), SCALE);

    /*Create an object of energy pills*/
    Enemy energyPills(World, lvl.GetObjects("enemy"), lvl.GetTileSize(), SCALE);

    /*Create a platform object*/
    Texture platformImg; 
    platformImg.loadFromFile("image/platform.png");
    list <Object> platform;
    platform.push_back(getPlatform(platformImg));

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

        World.Step(1 / 60.f, 8, 3);
        
        window.clear();

        /*Change of direction depending on speed*/
        playerBall.SetDirection();

        playerBall.UpdatePosition(SCALE);

        /*Moving platform*/
        for (auto it = platform.begin(); it != platform.end(); it++)
        {   
            if (it->direct == false) continue;
            if (it->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mouse::isButtonPressed(Mouse::Left))
            {
                it->sprite.setPosition(mousePos.x, mousePos.y);
                break;
            }
        }

        /*Collision check*/
        playerBall.CheckCollision(energyPills, platform);

        lvl.Draw(window);

        playerBall.Draw(window);

        energyPills.Draw(window);

        for (const auto& obj : platform)
        {
            window.draw(obj.sprite);
        }

        window.display();
    }

    return 0;
}