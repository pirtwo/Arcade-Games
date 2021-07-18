#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <memory>
#include <math.h>
#include "Key.h"
#include "Ball.h"
#include "Tile.h"
#include "Entity.h"
#include "Paddle.h"
#include "Particle.h"
#include "Collision.h"
#include "Loader.h"
#include "Atlas.h"

struct Level
{
    std::vector<std::vector<int>> data;
};

void createTiles(
    Level &level, Atlas &atlas, std::vector<std::shared_ptr<Entity>> &world)
{
    float x = 0, y = 0, paddingX = 10, paddingY = 10;

    for (int i = 0; i < (int)level.data.size(); i++)
    {
        for (int j = 0; j < (int)level.data[i].size(); j++)
        {
            if (level.data[i][j] == 0)
                continue;

            std::shared_ptr<Tile> tile;

            switch (level.data[i][j])
            {
            case 2:
                tile = std::make_shared<Tile>(atlas, "tileGreen_01.png", 1);
                break;
            case 3:
                tile = std::make_shared<Tile>(atlas, "tileOrange_01.png", 1);
                break;
            case 4:
                tile = std::make_shared<Tile>(atlas, "tileRed_01.png", 1);
                break;
            default:
                break;
            }

            tile->setScale(0.2, 0.2);
            tile->setPosition(
                x + j * (tile->getBounds().width + paddingX),
                y + i * (tile->getBounds().height + paddingY));
            world.push_back(tile);
        }
    }
}

void addEffect(
    std::vector<std::shared_ptr<ParticleSystem>> &effects,
    const std::vector<std::shared_ptr<sf::Sprite>> &sprites,
    int n,
    float x,
    float y)
{
    auto p = std::make_shared<ParticleSystem>(
        sprites, 0, 360, 2.f, 3.f, 0.05, 0.09, 0, 360, 3, 4, false);
    p->setPosition(x, y);
    p->addFule(n);
    effects.push_back(p);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 700), "Arkanoid");
    window.setFramerateLimit(60);

    // ========= load assets =======//

    auto atlas = std::make_shared<Atlas>();
    if (!atlas->texture.loadFromFile("./assets/atlas.png") ||
        !loadAtlasData("./assets/atlas.dat", atlas->data))
    {
        return EXIT_FAILURE;
    }

    auto level1 = std::make_shared<Level>();
    if (!loadLevelData("./assets/levels/level_01.dat", level1->data))
    {
        return EXIT_FAILURE;
    }

    //======== initialize =========//

    Key arrowL, arrowR;

    sf::Vector2f cp, cn;
    sf::FloatRect bound;

    std::vector<std::shared_ptr<Entity>> world;

    std::vector<std::shared_ptr<ParticleSystem>> effects;
    std::vector<std::shared_ptr<sf::Sprite>> effectSprites;
    effectSprites.push_back(std::make_shared<sf::Sprite>(atlas->texture));
    effectSprites[0]->setTextureRect(atlas->data["tileGreen_02.png"]);

    auto paddle = std::make_shared<Paddle>(*atlas, 5.5);
    paddle->setScale(0.08, 0.08);
    paddle->setPosition(300, 600);
    world.push_back(paddle);

    sf::Vector2f ballPrevPos;
    auto ball = std::make_shared<Ball>(*atlas);
    ball->vel = sf::Vector2f(2.5, -2.5);
    ball->setScale(0.1, 0.1);
    ball->setPosition(window.getSize().x / 2, 400);
    world.push_back(ball);

    auto fieldBounds = sf::RectangleShape(sf::Vector2f(
        window.getSize().x, window.getSize().y));
    fieldBounds.setPosition(0, 0);

    createTiles(*level1, *atlas, world);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::KeyPressed:
                if (e.key.code == sf::Keyboard::Left)
                {
                    arrowL.onPress();
                    break;
                }
                if (e.key.code == sf::Keyboard::Right)
                {
                    arrowR.onPress();
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                if (e.key.code == sf::Keyboard::Left)
                {
                    arrowL.onRelease();
                    break;
                }
                if (e.key.code == sf::Keyboard::Right)
                {
                    arrowR.onRelease();
                    break;
                }
                break;
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        //======== update ========//
        if (arrowL.isDown)
            paddle->moveLeft();
        if (arrowR.isDown)
            paddle->moveRight();
        if (!arrowL.isDown && !arrowR.isDown)
            paddle->stop();

        ballPrevPos = ball->getPosition();

        for (auto &&i : world)
        {
            i->update();
        }

        for (auto i = effects.begin(); i != effects.end(); i++)
        {
            auto e = i->get();
            e->update(1.f);
            if (e->getFule() == 0)
            {
                i = effects.erase(i);
                if (i == effects.end())
                    break;
            }
        }

        // collision detection

        for (auto i = world.begin(); i != world.end(); i++)
        {
            auto e = i->get();

            if (e->name == "ball")
            {
                // check for walls collision
                if (rayVsBox(
                        ballPrevPos,
                        ball->vel,
                        fieldBounds.getPosition(),
                        fieldBounds.getSize(),
                        cp, cn))
                {
                    if (cn.x != 0 && cn.y != 0)
                        ball->vel *= -1.f;
                    else if (cn.x != 0)
                        ball->vel.x *= -1;
                    else if (cn.y != 0)
                        ball->vel.y *= -1;
                    ball->setPosition(cp + cn);
                }
            }

            if (e->name == "paddle")
            {
                // check for ball collision to paddle

                bound = e->getBounds();
                bound.top -= ball->getBounds().height / 2;
                bound.left -= ball->getBounds().width / 2;
                bound.width += ball->getBounds().width;
                bound.height += ball->getBounds().height;

                if (rayVsBox(
                        ballPrevPos,
                        ball->vel,
                        sf::Vector2f(bound.left, bound.top),
                        sf::Vector2f(bound.width, bound.height),
                        cp, cn))
                {
                    if (cn.y == -1 && ballPrevPos.y < paddle->getPosition().y)
                    {
                        if (paddle->vel.x > 0)
                            ball->vel.x = abs(ball->vel.x);
                        else if (paddle->vel.x < 0)
                            ball->vel.x = abs(ball->vel.x) * -1;
                        ball->vel.y *= -1;
                        ball->setPosition(cp + cn);
                    }
                }
            }

            if (e->name == "tile")
            {
                // check for ball collision to tile

                bound = e->getBounds();
                bound.top -= ball->getBounds().height / 2;
                bound.left -= ball->getBounds().width / 2;
                bound.width += ball->getBounds().width;
                bound.height += ball->getBounds().height;

                if (rayVsBox(
                        ballPrevPos,
                        ball->vel,
                        sf::Vector2f(bound.left, bound.top),
                        sf::Vector2f(bound.width, bound.height),
                        cp, cn))
                {
                    if (cn.x != 0 && cn.y != 0)
                        ball->vel *= -1.f;
                    else if (cn.x != 0)
                        ball->vel.x *= -1;
                    else if (cn.y != 0)
                        ball->vel.y *= -1;
                    ball->setPosition(cp + cn);

                    addEffect(effects, effectSprites, 5, cp.x, cp.y);

                    i = world.erase(i);
                    if (i == world.end())
                        break;
                }
            }
        }

        //======== draw ========//
        window.clear();
        for (auto &&i : effects)
        {
            window.draw(*i);
        }
        for (auto &&i : world)
        {
            window.draw(*i);
        }
        window.display();
    }

    return EXIT_SUCCESS;
}