#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/box2d.h>
#include "Level.h"
#include "Batty.h"
#include "GameObject.h"
#include "Control.h"
#include "GameStatistics.h"
#include <list>
#include <queue>

using namespace sf;
using namespace std;

const float SCALE = 30.f; // conversion from meters to px in box2d
const float DEG = 57.29577f; // convert from radians to degrees in box2d

b2Vec2 Gravity(0.0f, 0.0f);
b2World World(Gravity);

void AddRounds(queue <string>& Rounds);

int main()
{
    bool gameOver = false;
    bool failRound = false;

    queue <string> Rounds;
    AddRounds(Rounds);

    RenderWindow window;
    window.create(VideoMode(1024, 768), "Whole Ball Game v.1.0");

    /* Create a Map object */
    Level lvl;
    lvl.LoadFromFile(Rounds.front(), World, SCALE);

    /*Create an object of manager*/
    Control manager;

    /*Create an object of Ball*/
    manager.InitBall(World, lvl.GetObject("ball"), SCALE);

    /*Create an object of EnergyPills*/
    manager.InitEnergyPills(World, lvl.GetObjects("enemy"), lvl.GetTileSize(), SCALE);

    /*Create an object of SpeedUp*/
    manager.InitSpeedUp(World, lvl.GetObject("speed"), lvl.GetTileSize(), SCALE);

    /*Create an object of Teleport*/
    manager.InitTeleport(World, lvl.GetObjects("teleport"), lvl.GetTileSize(), SCALE);

    /*Create a platform object*/
    Batty platform;

    /*Create object statisticks*/
    GameStatistics stata;

    Clock clock;
    Time time;

    SoundBuffer buffer;
    buffer.loadFromFile("sound/collect3.wav");
    Sound soundCollect;
    soundCollect.setBuffer(buffer);
    Sound win;
    SoundBuffer buffer_2;
    buffer_2.loadFromFile("sound/win.wav");
    win.setBuffer(buffer_2);
    Sound music;
    SoundBuffer buffer_3;
    buffer_3.loadFromFile("sound/happy.ogg");
    music.setBuffer(buffer_3);

    while (window.isOpen())
    {
        Event event;

        Vector2i mousePos = Mouse::getPosition(window);

        /*Update Statistic*/
        stata.Update(time.asSeconds(), manager.GetEnergyPills().size());

        if (manager.GetSpeedBall().second != 0)
        {
            time = clock.getElapsedTime();
            /*Moving platform*/
            platform.UpdatePosition(mousePos);
        }

        if (stata.GetTimeRound() <= 0)
        {
            failRound = true;
            stata.SetLiveDown();
        }
        
        if (failRound || manager.GetEnergyPills().empty())
        {
            music.stop();
            Rounds.pop();
            win.play();
            clock.restart();
            time = clock.getElapsedTime();
            stata.SetTimeRonud();

            lvl.DestroyLevel(World);
            manager.DestroyObjects(World);
            platform.DestroyPlatform();

            buffer_3.loadFromFile("sound/fly.ogg");
            music.setBuffer(buffer_3);
            lvl.LoadFromFile(Rounds.front(), World, SCALE);
            manager.InitBall(World, lvl.GetObject("ball"), SCALE);
            manager.InitEnergyPills(World, lvl.GetObjects("enemy"), lvl.GetTileSize(), SCALE);
            manager.InitSpeedUp(World, lvl.GetObject("speed"), lvl.GetTileSize(), SCALE);
            manager.InitTeleport(World, lvl.GetObjects("teleport"), lvl.GetTileSize(), SCALE);
            platform.AddPlatform();
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
                    music.play();
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

        /*Collision check*/
        manager.CheckCollisionBall(World, platform.GetPlatform(), SCALE, stata, soundCollect);

        /*Drawing Map*/
        lvl.Draw(window);

        /*Draving Objects*/
        manager.DrawGameObject(window);

        /*Draving Platform*/
        platform.Draw(window);

        /*Draving Statistics*/
        stata.DrawStatistics(window);

        window.display();
    }

    return 0;
}

void AddRounds(queue<string>& Rounds)
{
    Rounds.push("Levels/level1.tmx");
    Rounds.push("Levels/level2.tmx");
    /* Rounds.push("LevelOne/level..n..");*/
}
