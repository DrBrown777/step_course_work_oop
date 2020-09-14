﻿#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Level.h"
#include "Batty.h"
#include "GameObject.h"
#include "Control.h"
#include "GameStatistics.h"
#include <list>
#include <iomanip>
#include <sstream>

using namespace sf;
using namespace std;

const float SCALE = 30.f; // conversion from meters to px in box2d
const float DEG = 57.29577f; // convert from radians to degrees in box2d

b2Vec2 Gravity(0.0f, 0.0f);
b2World World(Gravity);

int main()
{
    bool gameOver = false;
    bool failRound = false;

    RenderWindow window;
    window.create(VideoMode(1024, 768), "Whole Ball Game v.1.0");

    /* Create a Map object */
    Level lvl;
    lvl.LoadFromFile("LevelOne/level1.tmx", World, SCALE);

    /*Create an object of manager*/
    Control manager;

    /*Create an object of Ball*/
    manager.InitBall(World, lvl.GetObject("ball"), SCALE);

    /*Create an object of EnergyPills*/
    manager.InitEnergyPills(World, lvl.GetObjects("enemy"), lvl.GetTileSize(), SCALE);

    /*Create a platform object*/
    Batty platform;

    /*Create object statisticks*/
    GameStatistics stata;

    Clock clock;
    Time time;

    while (window.isOpen())
    {
        Event event;

        Vector2i mousePos = Mouse::getPosition(window);

        if (manager.GetSpeedBall().second != 0)
        {
            time = clock.getElapsedTime();
        }

        /*Update Statistic*/
        stata.Update(time.asSeconds(), manager.GetEnergyPills().size());

        if (manager.GetEnergyPills().empty() || stata.GetTimeRound() <= 0) failRound = true;
        
        if (failRound)
        {
            stata.SetLive();

            lvl.DestroyLevel(World);
            manager.DestroyObjects(World);
            platform.DestroyPlatform();

            window.clear();

            lvl.LoadFromFile("LevelOne/level1.tmx", World, SCALE);
            manager.InitBall(World, lvl.GetObject("ball"), SCALE);
            manager.InitEnergyPills(World, lvl.GetObjects("enemy"), lvl.GetTileSize(), SCALE);
            platform.AddPlatform();
            clock.restart();
            failRound = false;
        }

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
                    clock.restart();
                    manager.SetSpeedBall();
                }
                if (event.key.code == Keyboard::Enter)
                {
                    manager.SetSpeedBall(manager.GetSpeedBall().first * 2, manager.GetSpeedBall().second * 2);
                }
                break;
            case Event::MouseButtonReleased:
                platform.StatePlatform(mousePos, event);
                break;
            }
        }

        World.Step(1 / 60.f, 8, 3);
        
        window.clear();

        /*Change of direction depending on speed*/
        manager.SetDirectionBall();

        /*Moving ball*/
        manager.UpdatePositionBall(SCALE);

        /*Moving platform*/
        platform.UpdatePosition(mousePos);

        /*Collision check*/
        manager.CheckCollisionBall(World, platform.GetPlatform());

        /*Drawing Map*/
        lvl.Draw(window);

        /*Draving Objects*/
        manager.DrawGameObject(window);
        platform.Draw(window);

        stata.DrawStatistics(window);

        window.display();
    }

    return 0;
}