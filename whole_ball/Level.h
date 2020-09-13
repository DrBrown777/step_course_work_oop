#pragma once

#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include <box2d/box2d.h>
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
    bool move = true;
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
    vector<Object> wall;
    vector <b2Body*> wallBody;
    void initWall(b2World& World, const float& SCALE);
public:
    Object GetObject(string name);
    vector<Object> GetObjects(string name);
    Vector2i GetTileSize();
    void DestroyLevel(b2World& World);
    void Draw(RenderWindow& window);
    bool LoadFromFile(string filename, b2World& World, const float& SCALE);
};
