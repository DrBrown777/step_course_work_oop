#include "Level.h"

Object Level::GetObject(string name)
{
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            return objects[i];
    return Object();
}

vector<Object> Level::GetObjects(string name)
{
    vector<Object> vec;
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            vec.push_back(objects[i]);
    
    return vec;  
}

Vector2i Level::GetTileSize()
{
    return Vector2i(tileWidth, tileHeight);
}

void Level::DestroyLevel(b2World& World)
{
    objects.clear();
    layers.clear();
    wall.clear();

    for (int i = 0; i < wallBody.size(); i++)
    {
        wallBody.at(i)->GetFixtureList()->GetBody()->DestroyFixture(wallBody.at(i)->GetFixtureList());
        World.DestroyBody(wallBody.at(i));
    }

    wallBody.clear();
}

void Level::Draw(RenderWindow& window)
{
    for (int layer = 0; layer < layers.size(); layer++)
        for (int tile = 0; tile < layers[layer].tiles.size(); tile++)
            window.draw(layers[layer].tiles[tile]);
}

bool Level::LoadFromFile(string filename, b2World& World, const float& SCALE)
{
    XMLDocument levelFile;

    levelFile.LoadFile(filename.c_str());

    if (levelFile.Error())
    {
        cout << "Loading level \"" << filename << "\" failed." << endl;
        return false;
    }

    XMLElement* maps;
    maps = levelFile.FirstChildElement("map");

    width = atoi(maps->Attribute("width"));
    height = atoi(maps->Attribute("height"));
    tileWidth = atoi(maps->Attribute("tilewidth"));
    tileHeight = atoi(maps->Attribute("tileheight"));

    XMLElement* tilesetElement;
    tilesetElement = maps->FirstChildElement("tileset");

    firstTileID = atoi(tilesetElement->Attribute("firstgid"));

    XMLElement* image;
    image = tilesetElement->FirstChildElement("image");
    string tmp_imagepath = image->Attribute("source");

    string imagepath = "LevelOne/" + tmp_imagepath;

    Image img;

    if (!img.loadFromFile(imagepath))
    {
        cout << "Failed to load tile sheet." << endl;
        return false;
    }

    img.createMaskFromColor(Color(109, 159, 185));

    tilesetImage.loadFromImage(img);

    tilesetImage.setSmooth(false);

    int columns = tilesetImage.getSize().x / tileWidth;
    int rows = tilesetImage.getSize().y / tileHeight;

    vector<Rect<int>> subRects;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            Rect<int> rect;

            rect.top = y * tileHeight;
            rect.height = tileHeight;
            rect.left = x * tileWidth;
            rect.width = tileWidth;

            subRects.push_back(rect);
        }
    }

    XMLElement* layerElement;
    layerElement = maps->FirstChildElement("layer");

    while (layerElement)
    {
        Layer layer;

        if (layerElement->Attribute("opacity") != NULL)
        {
            float opacity = strtod(layerElement->Attribute("opacity"), NULL);
            layer.opacity = 255 * opacity;
        }
        else
        {
            layer.opacity = 255;
        }

        XMLElement* layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data");

        if (layerDataElement == NULL)
        {
            cout << "Bad map. No layer information found." << endl;
        }

        XMLElement* tileElement;
        tileElement = layerDataElement->FirstChildElement("tile");

        if (tileElement == NULL)
        {
            cout << "Bad map. No tile information found." << endl;
            return false;
        }

        int x = 0;
        int y = 0;

        while (tileElement)
        {
            int tileGID = atoi(tileElement->Attribute("gid"));
            int subRectToUse = tileGID - firstTileID;

            if (subRectToUse >= 0)
            {
                Sprite sprite;
                sprite.setTexture(tilesetImage);

                sprite.setTextureRect(subRects[subRectToUse]);
                sprite.setPosition(x * tileWidth, y * tileHeight);
                sprite.setColor(Color(255, 255, 255, layer.opacity));

                layer.tiles.push_back(sprite);
            }

            tileElement = tileElement->NextSiblingElement("tile");

            x++;
            if (x >= width)
            {
                x = 0;
                y++;
                if (y >= height)
                    y = 0;
            }
        }

        layers.push_back(layer);

        layerElement = layerElement->NextSiblingElement("layer");
    }

    XMLElement* objectGroupElement;

    if (maps->FirstChildElement("objectgroup") != NULL)
    {
        objectGroupElement = maps->FirstChildElement("objectgroup");

        while (objectGroupElement)
        {
            XMLElement* objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");

            string objectType;
            if (objectGroupElement->Attribute("name") != NULL)
            {
                objectType = objectGroupElement->Attribute("name");
            }

            while (objectElement)
            {
                string objectName;
                if (objectElement->Attribute("name") != NULL)
                {
                    objectName = objectElement->Attribute("name");
                }

                int x = atoi(objectElement->Attribute("x"));
                int y = atoi(objectElement->Attribute("y"));

                int width, height;

                Sprite sprite;
                sprite.setTexture(tilesetImage);
                sprite.setTextureRect(Rect<int>(0, 0, 0, 0));
                sprite.setPosition(x, y);

                if (objectElement->Attribute("width") != NULL)
                {
                    width = atoi(objectElement->Attribute("width"));
                    height = atoi(objectElement->Attribute("height"));
                    if (objectType == "sphere")
                    {
                        enemyImage.loadFromFile("image/sphere.png");
                        sprite.setTexture(enemyImage);
                        sprite.setTextureRect(IntRect(0, 0, width, height));
                        sprite.setPosition(x, y - height);
                    }
                }
                else
                {
                    width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
                    height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
                    sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
                }

                Object object;
                object.name = objectName;
                object.type = objectType;
                object.sprite = sprite;

                Rect <int> objectRect;

                objectRect.left = x;
                if (objectType == "collision")
                    objectRect.top = y;
                else
                    objectRect.top = y - height;
                objectRect.height = height;
                objectRect.width = width;
                object.rect = objectRect;

                objects.push_back(object);

                objectElement = objectElement->NextSiblingElement("object");
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }
    else
    {
        cout << "No object layers found..." << endl;
    }

    initWall(World, SCALE);

    return true;
}

void Level::initWall(b2World& World, const float& SCALE)
{
    wall = GetObjects("solid");

    for (int i = 0; i < wall.size(); i++)
    {
        b2BodyDef bodyDef;
        b2Body* body;
        b2PolygonShape shape;
        b2FixtureDef fixtureDef;

        bodyDef.type = b2_staticBody;
        bodyDef.position.Set((wall[i].rect.left + tileWidth / 2 * (wall[i].rect.width / tileWidth - 1)) / SCALE,
            (wall[i].rect.top + tileHeight / 2 * (wall[i].rect.height / tileHeight - 1)) / SCALE);
        body = World.CreateBody(&bodyDef);
        shape.SetAsBox(wall[i].rect.width / 2 / SCALE, wall[i].rect.height / 2 / SCALE);
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        body->CreateFixture(&fixtureDef);
        body->SetUserData(&wall[i].name);
        wallBody.push_back(body);
    }
}
