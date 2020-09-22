#pragma once
#include "GameObject.h"
#include "Batty.h"
#include "GameStatistics.h"
#include <SFML/Audio.hpp>

class Control
{
private:
	GameObject* playerBall;
	vector <GameObject*> energyPills;
	GameObject* speedUp;
	pair <GameObject*, GameObject*> teleport;
	bool port = true;
	pair <double, double> ballSpeed;
	bool ballDirFlag;
	b2Vec2 UP, DOWN, LEFT, RIGHT;
public:
	Control();

	void InitGameObjects(b2World& World, Level& lvl, const Vector2i& _tileSize, const float& SCALE);
	void SetSpeedBall(double _speedMin = 0.45, double _speedMax = 0.5);
	void SetDirectionBall();
	pair <double, double> GetSpeedBall();
	vector <GameObject*> GetEnergyPills();
	void UpdatePositionBall(const float& SCALE);
	void CheckCollisionBall(b2World& World, const list <Object>& platform, const float& SCALE, GameStatistics& stata, Sound& sound);
	void DestroyObjects(b2World& World);
	void DrawGameObject(RenderWindow& window);
	Control(const Control& other) = delete;
	Control& operator=(const Control& other) = delete;
	~Control();
};