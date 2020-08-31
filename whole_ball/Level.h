#pragma once

#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include <iostream>
#include <string>
#include <vector>

using namespace sf;
using namespace tinyxml2;
using namespace std;

struct Object
{
    string name;
    string type;
    Rect<int> rect;
    Sprite sprite;
    bool direct = true;
};

struct Layer
{
    int opacity;
    vector<Sprite> tiles;
};

class Level
{
private:
    int width, height, tileWidth, tileHeight;
    int firstTileID;
    Texture tilesetImage;
    Texture enemyImage;
    vector<Object> objects;
    vector<Layer> layers;
public:
    Object GetObject(string name);
    vector<Object> GetObjects(string name);
    Vector2i GetTileSize();
    void Draw(RenderWindow& window);
    bool LoadFromFile(string filename);
};
