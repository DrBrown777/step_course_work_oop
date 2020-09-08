#include "Control.h"

Control::Control(b2World& World, Object _playerBall, vector <Object> _enemy, Vector2i _tileSize, const float& SCALE)
{
	playerBall = new Ball(World, _playerBall, SCALE);
	
	for (int i = 0; i < _enemy.size(); i++)
	{
		energyPills.push_back(new Enemy(World, _enemy[i], _tileSize, SCALE));
	}

	ballSpeed = make_pair(0, 0);

	ballDirFlag = false;

	UP.Set(0.0f, -0.5f); DOWN.Set(0.0f, 0.5f);
	LEFT.Set(-0.5f, 0.0f); RIGHT.Set(0.5f, 0.0f);
}

Control::~Control()
{
	delete playerBall;

	for (auto& obj : energyPills)
	{
		delete obj;
	}
}
