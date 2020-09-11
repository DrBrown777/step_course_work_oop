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
	Control();
	void InitBall(b2World& World, const Object& _playerBall, const float& SCALE);
	void InitEnergyPills(b2World& World, const vector <Object>& _enemy, const Vector2i& _tileSize, const float& SCALE);
	Control(const Control& obj) = delete;
	void SetSpeedBall(double _speedMin = 0.45, double _speedMax = 0.5);
	void SetDirectionBall();
	pair <double, double> GetSpeedBall();
	vector <GameObject*> GetEnergyPills();
	void UpdatePositionBall(const float& SCALE);
	void CheckCollisionBall(Batty& platform);
	void DestroyObjects();
	void DrawGameObject(RenderWindow& window);
	~Control();
};