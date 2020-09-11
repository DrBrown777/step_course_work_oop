#include "Batty.h"

Batty::Batty(b2World& World, const float& SCALE)
{
    platformImg.loadFromFile("image/platform.png");

    bat.name = "platform";
    bat.type = "solid";
    bat.rect = Rect<int>(0, 0, 13, 48);
    bat.sprite.setTexture(platformImg);
    bat.sprite.setTextureRect(bat.rect);
    bat.sprite.setOrigin(bat.rect.width / 2, bat.rect.height / 2);
    bat.sprite.setPosition(100, 150);
    //bat.sprite.setRotation(45);

    b2BodyDef bodyDef;
    b2PolygonShape shape;
    b2FixtureDef fixtureDef;

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(bat.sprite.getPosition().x / SCALE, bat.sprite.getPosition().y / SCALE);
    batBody = World.CreateBody(&bodyDef);
    shape.SetAsBox(bat.sprite.getOrigin().x / SCALE, bat.sprite.getOrigin().y / SCALE);
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    batBody->CreateFixture(&fixtureDef);
    batBody->SetUserData(&bat.name);

    platform.push_back(make_pair(bat, batBody));
}

list <pair <Object, b2Body*>> Batty::GetPlatform()
{
    return platform;
}

void Batty::AddPlatform()
{
    platform.push_back(make_pair(bat, batBody));
}

void Batty::StatePlatform(const Vector2i& mousePos, const Event& event)
{
    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        if (it->first.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && event.key.code == Mouse::Right)
        {
            it->first.sprite.rotate(45);
            break;
        }
        if (it->first.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && event.key.code == Mouse::Middle)
        {
            platform.erase(it);
            break;
        }
    }
    if (platform.empty())
        AddPlatform();
    else if (platform.back().first.sprite.getPosition().y > 100)
        AddPlatform();
    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        /*if (it->first.sprite.getPosition().y > 100)
            it->first.move = false;*/
    }
}

void Batty::DestroyPlatform()
{
    platform.clear();
}

void Batty::UpdatePosition(const Vector2i& mousePos, const float& SCALE)
{
    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        if (it->first.move == false) continue;
        if (it->first.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mouse::isButtonPressed(Mouse::Left))
        {
            it->first.sprite.setPosition(mousePos.x, mousePos.y);
            it->second->SetTransform(b2Vec2(mousePos.x, mousePos.y), 0);
            break;
        }
    }
}

void Batty::Draw(RenderWindow& window)
{
    for (const auto& obj : platform)
    {
        window.draw(obj.first.sprite);
    }
}
