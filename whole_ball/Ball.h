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
	void SetLinVel(b2Vec2 _linearVelocity);
	void SetSpeed(double _speedMin = 0.45, double _speedMax = 0.5);
	void SetDirection();
	void UpdatePosition(const float& SCALE);
	void SetDirFlag(bool _flag);
	b2Vec2 GetLinVel();
	pair <double, double> GetSpeed();
	bool GetDirFlag();
	void CheckCollision(vector <Object>& enemy, vector <b2Body*>& enemyBody, const list <Object>& platform);
	void Draw(RenderWindow& window);
};

