#include "Enemy.h"

Enemy::Enemy(b2World& World, vector<Object> _enemy, Vector2i _tileSize, const float& SCALE)
{
    enemy = _enemy;

    for (int i = 0; i < enemy.size(); i++)
    {
        b2BodyDef bodyDef;
        b2Body* body;
        b2CircleShape shape;

        bodyDef.type = b2_staticBody;
        bodyDef.position.Set((enemy[i].rect.left +
            _tileSize.x / 2 * (enemy[i].rect.width / _tileSize.x - 1)) / SCALE,
            (enemy[i].rect.top + _tileSize.y / 2 * (enemy[i].rect.height / _tileSize.y - 1)) / SCALE);
        body = World.CreateBody(&bodyDef);
        shape.m_radius = ((enemy[i].rect.height / 2) - 5) / SCALE;
        body->CreateFixture(&shape, 1.0f);
        body->SetUserData(&enemy[i].name);

        enemyBody.push_back(body);
    }

}

vector<Object>* Enemy::GetEnemy()
{
    return &enemy;
}

vector<b2Body*>* Enemy::GetEnemyBody()
{
    return &enemyBody;
}

void Enemy::Draw(RenderWindow& window)
{
    for (const auto& obj : enemy)
    {
        window.draw(obj.sprite);
    }
}
