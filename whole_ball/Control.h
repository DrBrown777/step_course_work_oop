#pragma once
#include "GameObject.h"

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
	Control(b2World& World, Object _playerBall, vector <Object> _enemy, Vector2i _tileSize, const float& SCALE);
	~Control();
};

