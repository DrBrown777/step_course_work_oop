#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "tinyxml2.h"
#include <iostream>
#include <math.h>

using namespace sf;
using namespace tinyxml2;
using namespace std;

const float SCALE = 30.f; //������� �� ������ � px � box2d
const float DEG = 57.29577f; //������� �� ������ � ������� � box2d

b2Vec2 Gravity(0.0f, 0.0f);
b2World World(Gravity);

struct Object
{
    string name;
    string type;
    Rect<int> rect;
    Sprite sprite;
};

struct Layer
{
    int opacity;
    vector<Sprite> tiles;
};

class Level
{
public:
    bool LoadFromFile(string filename);
    Object GetObject(string name);
    vector<Object> GetObjects(string name);
    void Draw(RenderWindow& window);
    Vector2i GetTileSize();

private:
    int width, height, tileWidth, tileHeight;
    int firstTileID;
    Texture tilesetImage;
    Texture enemyImage;
    vector<Object> objects;
    vector<Layer> layers;
};

Object Level::GetObject(string name)
{
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            return objects[i];
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

void Level::Draw(RenderWindow& window)
{
    for (int layer = 0; layer < layers.size(); layer++)
        for (int tile = 0; tile < layers[layer].tiles.size(); tile++)
            window.draw(layers[layer].tiles[tile]);
}

bool Level::LoadFromFile(string filename)
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

    return true;
}

int main()
{
    double speedMIN = 0, speedMAX = 0;
    bool flag = false;

    Level lvl;

    lvl.LoadFromFile("LevelOne/level1.tmx");

    RenderWindow window;
    window.create(VideoMode(1024, 768), "Whole Ball Game v.1.0");
    //window.setFramerateLimit(60);
    
    /*������� ������ ���*/
    
    Object playerBall = lvl.GetObject("ball");

    b2BodyDef bDef;
    bDef.type = b2_dynamicBody;
    bDef.position.Set(playerBall.rect.left / SCALE, playerBall.rect.top / SCALE);
    b2Body* pBodyBall = World.CreateBody(&bDef);
    b2CircleShape shape;
    shape.m_radius = ((playerBall.rect.height / 2)) / SCALE;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    pBodyBall->CreateFixture(&fixtureDef);
    pBodyBall->SetUserData(&playerBall.name);

    /*������� ������ ���������*/

    Object platform135, platform45;
    vector <Object> platform;

    platform135.name = "platform135";
    platform45.name = "platform45";

    platform.push_back(platform135);
    platform.push_back(platform45);

    Texture img;
    img.loadFromFile("image/platform.png");
    
    for (int i = 0; i < platform.size(); i++)
    {
        platform[i].type = "solid";
        platform[i].rect = Rect<int>(0, 0, 13, 48);
        platform[i].sprite.setTextureRect(platform[i].rect);
        platform[i].sprite.setTexture(img);
        platform[i].sprite.setOrigin(platform[i].rect.width / 2, platform[i].rect.height / 2);
        if (platform[i].name == "platform135")
        {
            platform[i].sprite.setRotation(315);
            platform[i].sprite.setPosition(100, 90);
        }
        else
        {
            platform[i].sprite.setRotation(45);
            platform[i].sprite.setPosition(170, 90);
        }
    }

    /*������� ������ ����*/

    vector <Object> enemy;
    vector <b2Body*> enemyBody;
    Vector2i tileSize = lvl.GetTileSize();

    enemy = (lvl.GetObjects("enemy"));

    for (int i = 0; i < enemy.size(); i++)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set((enemy[i].rect.left +
            tileSize.x / 2 * (enemy[i].rect.width / tileSize.x - 1)) / SCALE,
            (enemy[i].rect.top + tileSize.y / 2 * (enemy[i].rect.height / tileSize.y - 1)) / SCALE);
        b2Body* body = World.CreateBody(&bodyDef);
        b2CircleShape shape;
        shape.m_radius = ((enemy[i].rect.height / 2) - 5) / SCALE;
        body->CreateFixture(&shape, 1.0f);
        body->SetUserData(&enemy[i].name);

        enemyBody.push_back(body);
    }

    /*������� ����� �����*/

    vector<Object> wall = lvl.GetObjects("solid");
    vector <b2Body*> wallBody;

    for (int i = 0; i < wall.size(); i++)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set((wall[i].rect.left + tileSize.x / 2 * (wall[i].rect.width / tileSize.x - 1)) / SCALE,
            (wall[i].rect.top + tileSize.y / 2 * (wall[i].rect.height / tileSize.y - 1)) / SCALE);
        b2Body* body = World.CreateBody(&bodyDef);
        b2PolygonShape shape;
        shape.SetAsBox(wall[i].rect.width / 2 / SCALE, wall[i].rect.height / 2 / SCALE);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        body->CreateFixture(&fixtureDef);
        body->SetUserData(&wall[i].name);
        wallBody.push_back(body);
    }

    while (window.isOpen())
    {
        Event event;

        Vector2i mousePos = Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyPressed:
                if (event.key.code == Keyboard::Up)
                {
                    pBodyBall->SetLinearVelocity(b2Vec2(0.0f, -0.5f));
                    flag = true;
                } 
                if (event.key.code == Keyboard::Right)
                {
                    pBodyBall->SetLinearVelocity(b2Vec2(0.5f, 0.0f));
                    speedMIN = 0.45, speedMAX = 0.5;
                    flag = false;
                } 
                if (event.key.code == Keyboard::Left)
                {
                    pBodyBall->SetLinearVelocity(b2Vec2(-0.5f, 0.0f));
                    flag = false;
                }
                if (event.key.code == Keyboard::Down)
                {
                    pBodyBall->SetLinearVelocity(b2Vec2(0.0f, 0.5f));
                    flag = true;
                }
                if (event.key.code == Keyboard::Space)
                {
                    speedMIN *= 2;
                    speedMAX *= 2;
                }
                if (event.key.code == Keyboard::Enter)
                {
                    speedMIN /= 2;
                    speedMAX /= 2;
                }           
                break;
            }
        }

        b2Vec2 vel = pBodyBall->GetLinearVelocity();

        if (flag == false)
        {
            if (vel.x < 0)
            {
                if (vel.x >= -speedMIN) { vel.x = -speedMIN; }
                if (vel.x <= -speedMAX) { vel.x = -speedMAX;  }
                vel.y = 0;
            }
            else
            {
                if (vel.x <= speedMIN) { vel.x = speedMIN; }
                if (vel.x >= speedMAX) { vel.x = speedMAX;  }
                vel.y = 0;
            }
        }
        if (flag == true)
        {
            if (vel.y < 0)
            {
                if (vel.y >= -speedMIN) { vel.y = -speedMIN; }
                if (vel.y <= -speedMAX) { vel.y = -speedMAX;  }
                vel.x = 0;
            }
            else
            {
                if (vel.y <= speedMIN) { vel.y = speedMIN; }
                if (vel.y >= speedMAX) { vel.y = speedMAX;  }
                vel.x = 0;
            }
        }

        pBodyBall->SetLinearVelocity(vel);

        //vel.Normalize();

        for (b2ContactEdge* ce = pBodyBall->GetContactList(); ce; ce = NULL)
        {
            b2Contact* c = ce->contact;

            for (int i = 0; i < enemyBody.size(); i++)
                if (c->GetFixtureB() == enemyBody[i]->GetFixtureList())
                {
                    enemyBody[i]->DestroyFixture(enemyBody[i]->GetFixtureList());
                    enemy.erase(enemy.begin() + i);
                    enemyBody.erase(enemyBody.begin() + i);
                }
            /*
            //�������� ������������ � ������, �������� �� �������� �� ���������.
            for (int i = 0; i < wallBody.size(); i++)
                if (c->GetFixtureA() == wallBody[i]->GetFixtureList())
                {
                    //float angle = 180 / DEG; //���� ������
                    //float angle = 270 / DEG; //���� ����
                    //float angle = 90 / DEG; //���� �����
                    //float angle = 0 / DEG; //���� �����
                    //float speed = sqrt(velosity.x * velosity.x + velosity.y * velosity.y);
                    //b2Vec2 vector = speed * b2Vec2(cos(angle), sin(angle));
                }
            */
        }

        World.Step(1 / 60.f, 8, 3);
        
        b2Vec2 pos = pBodyBall->GetPosition();

        //float angle = pBodyBall->GetAngle();

        playerBall.sprite.setPosition(pos.x * SCALE, pos.y * SCALE);
        //playerBall.sprite.setRotation(angle * DEG);

        window.clear();

        lvl.Draw(window);

        window.draw(playerBall.sprite);

        for (const auto &obj: enemy)
        {
            window.draw(obj.sprite);
        }

        for (int i = 0; i < platform.size(); i++)
        {
            if (platform[i].sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mouse::isButtonPressed(Mouse::Left))
            {
                platform[i].sprite.setPosition(mousePos.x, mousePos.y);
            }
        }

        for (const auto& obj : platform)
        {
            window.draw(obj.sprite);
        }

        window.display();
    }

    /*
    RenderWindow window(VideoMode(1024, 768), "Whole Ball");
    window.setFramerateLimit(60);

    RectangleShape rectangle(Vector2f(640.f, 40.f));
    rectangle.setFillColor(Color(250, 150, 100));
    rectangle.setPosition(Vector2f(192.f, 600.f));

    Texture tx;
    tx.create(20, 20);
    tx.loadFromFile("sphere-16.png");

    CircleShape circle(20.f);
    //circle.setFillColor(Color(100, 250, 50));
    circle.setTexture(&tx, 0);
    circle.setPosition(Vector2f(300.f, 200.f));

    b2PolygonShape gr;
    gr.SetAsBox(320 / SCALE, 40 / SCALE);

    b2BodyDef bdef;
    bdef.position.Set(490 / SCALE, 620 / SCALE);

    b2Body* b_ground = World.CreateBody(&bdef);
    b_ground->CreateFixture(&gr, 1);

    b2CircleShape ball;
    ball.m_radius = 20 / SCALE;

    bdef.type = b2_dynamicBody;
    //bdef.type = b2_kinematicBody;;

    b2FixtureDef fdef;
    fdef.density = 2;
    fdef.shape = &ball;

    char ball_str[] = "ball";
    int posX = 300;
    int posY = 200;
    float res = 1;

    for (size_t i = 0; i < 5; i++)
    {
        bdef.position.Set(posX / SCALE, posY / SCALE);
        b2Body* pBody = World.CreateBody(&bdef);
        fdef.restitution = res;
        pBody->CreateFixture(&fdef);
        pBody->SetUserData(ball_str);
        //pBody->SetLinearVelocity(b2Vec2(10, 0));
        //pBody->SetLinearDamping(1);
        posX += 100; posY -= 10;
        res -= 0.01;
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }
        World.Step(1 / 60.f, 8, 3);
        window.clear();
        window.draw(rectangle);
        b2Vec2 pos;
        Vector2i mousePos = Mouse::getPosition(window);

        for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
        {
            pos = it->GetPosition();
            float angle = it->GetAngle();

            if (it->GetUserData() == ball_str)
            {
                circle.setPosition(pos.x * SCALE, pos.y * SCALE);
                //circle.setRotation(angle * DEG);
            }
            window.draw(circle);
        }

        if (circle.getGlobalBounds().contains(mousePos.x, mousePos.y) && Mouse::isButtonPressed(Mouse::Left))
        {
            circle.setPosition(mousePos.x - 20, mousePos.y - 20);
            pos.x = mousePos.x - 20; pos.y = mousePos.y - 20;
            //circle.setRotation(angle * DEG);

        }
        window.display();
    }
    */
    return 0;
}