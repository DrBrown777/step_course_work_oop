#pragma once
#include "GameObject.h"
#include "Batty.h"

class Control
{
private:
	GameObject* playerBall;
	vector <GameObject*> energyPills;

	/*var for ball*/
	pair <double, double> ballSpeed;
	bool ballDirFlag;
	b2Vec2 UP, DOWN, LEFT, RIGHT;

public:
	Control(b2World& World, const Object& _playerBall, const vector <Object>& _enemy, const Vector2i& _tileSize, const float& SCALE);
	Control(const Control& obj);
	void SetSpeedBall(double _speedMin = 0.45, double _speedMax = 0.5);
	void SetDirectionBall();
	void UpdatePositionBall(const float& SCALE);
	void CheckCollisionBall(Batty& platform);
	void DrawGameObject(RenderWindow& window);
	~Control();
};