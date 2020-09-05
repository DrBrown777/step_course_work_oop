#pragma once
#include "Level.h"
#include "Enemy.h"
#include "Batty.h"
#include <box2d/box2d.h>
#include <list>

using namespace std;
using namespace sf;

class Ball
{
private:
	Object playerBall;
	b2Body* pBodyBall;
	pair <double, double> speed;
	bool directionFlag;
	b2Vec2 UP, DOWN, LEFT, RIGHT;
public:
	Ball(b2World& World, Object _playerBall, const float& SCALE);
	void SetDirection();
	void SetSpeed(double _speedMin = 0.45, double _speedMax = 0.5);
	void UpdatePosition(const float& SCALE);
	void CheckCollision(Enemy& energyPills, Batty& _platform);
	void Draw(RenderWindow& window);
};