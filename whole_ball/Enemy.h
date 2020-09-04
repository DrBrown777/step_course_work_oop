#pragma once
#include "Level.h"
#include <box2d/box2d.h>

class Enemy
{
private:
	vector <Object> enemy;
	vector <b2Body*> enemyBody;
public:
	Enemy(b2World& World, vector <Object> _enemy, Vector2i _tileSize, const float& SCALE);
	vector <Object>* GetEnemy();
	vector <b2Body*>* GetEnemyBody();
	void Draw(RenderWindow& window);
};