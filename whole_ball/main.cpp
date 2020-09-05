#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Level.h"
#include "Ball.h"
#include "Enemy.h"
#include "Batty.h"
#include <list>

using namespace sf;
using namespace std;

const float SCALE = 30.f; // conversion from meters to px in box2d
const float DEG = 57.29577f; // convert from radians to degrees in box2d

b2Vec2 Gravity(0.0f, 0.0f);
b2World World(Gravity);

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
    Batty platform;

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
                platform.InitPlatform(mousePos, event);
                break;
            }
        }

        World.Step(1 / 60.f, 8, 3);
        
        window.clear();

        /*Change of direction depending on speed*/
        playerBall.SetDirection();

        /*Moving ball*/
        playerBall.UpdatePosition(SCALE);

        /*Moving platform*/
        platform.UpdatePosition(mousePos);

        /*Collision check*/
        playerBall.CheckCollision(energyPills, platform);

        /*Drawing Map*/
        lvl.Draw(window);

        /*Draving Objects*/
        playerBall.Draw(window);
        energyPills.Draw(window);
        platform.Draw(window);

        window.display();
    }

    return 0;
}