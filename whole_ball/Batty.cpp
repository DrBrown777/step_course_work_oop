#include "Batty.h"

Batty::Batty()
{
    platformImg.loadFromFile("image/platform.png");

    bat.name = "platform";
    bat.type = "solid";
    bat.rect = Rect<int>(0, 0, 13, 48);
    bat.sprite.setTexture(platformImg);
    bat.sprite.setTextureRect(bat.rect);
    bat.sprite.setOrigin(bat.rect.width / 2, bat.rect.height / 2);
    bat.sprite.setPosition(100, 85);
    bat.sprite.setRotation(45);

    platform.push_back(bat);
}

list<Object>& Batty::GetPlatform()
{
    return platform;
}

void Batty::AddPlatform()
{
    platform.push_back(bat);
}

void Batty::StatePlatform(const Vector2i& mousePos, const Event& event)
{
    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        if (it->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && event.key.code == Mouse::Right)
        {
            it->sprite.rotate(45);
            break;
        }
        if (it->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && event.key.code == Mouse::Middle)
        {
            platform.erase(it);
            break;
        }
    }
    if (platform.empty())
        AddPlatform();
    else if (platform.back().sprite.getPosition().y > 100)
        AddPlatform();
    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        if (it->sprite.getPosition().y > 100)
            it->move = false;
    }
}

void Batty::DestroyPlatform()
{
    platform.clear();
}

void Batty::UpdatePosition(const Vector2i& mousePos)
{
    for (auto it = platform.begin(); it != platform.end(); it++)
    {
        if (it->move == false) continue;
        if (it->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mouse::isButtonPressed(Mouse::Left))
        {
            it->sprite.setPosition(mousePos.x, mousePos.y);
            break;
        }
    }
}

void Batty::Draw(RenderWindow& window)
{
    for (const auto& obj : platform)
    {
        window.draw(obj.sprite);
    }
}