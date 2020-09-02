#pragma once
#include "Level.h"
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
public:
	Ball(b2World& World, Object _playerBall, const float& SCALE);
	void SetDirection();
	void UpdatePosition(const float& SCALE);
	void CheckCollision(vector <Object>& enemy, vector <b2Body*>& enemyBody, const list <Object>& platform);
	void Draw(RenderWindow& window);
	friend void SetSpeed(Ball& playerBall, double _speedMin, double _speedMax);
};