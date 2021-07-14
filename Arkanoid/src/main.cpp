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

/**
 * TODO: create different levels with different tile layout
 * TODO: store level layouts in a file
 */

void createTiles(
    std::vector<std::shared_ptr<Entity>> &world,
    sf::Texture &t,
    float x,
    float y,
    int rows,
    int cols,
    float scaleX,
    float scaleY,
    float paddingX,
    float paddingY)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            auto tile = std::make_shared<Tile>(t, 1);
            tile->setScale(scaleX, scaleY);
            tile->setPosition(
                x + j * (tile->getBounds().width + paddingX),
                y + i * (tile->getBounds().height + paddingY));
            world.push_back(tile);
        }
    }
}

void addEffect(
    std::vector<std::shared_ptr<ParticleSystem>> &effects,
    const std::vector<std::shared_ptr<sf::Texture>> &textures,
    int n,
    float x,
    float y)
{
    auto p = std::make_shared<ParticleSystem>(
        textures, 1, 0, 360, 2.f, 3.f, 0.05, 0.09, 0, 360, 3, 4, false);
    p->setPosition(x, y);
    p->addFule(n);
    effects.push_back(p);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 700), "Arkanoid");
    window.setFramerateLimit(60);

    // ========= load assets =======//

    std::map<std::string, std::shared_ptr<sf::Texture>> textures;
    textures["ball"] =
        std::make_shared<sf::Texture>(sf::Texture());
    textures["paddle"] =
        std::make_shared<sf::Texture>(sf::Texture());
    textures["tileGreen_01"] =
        std::make_shared<sf::Texture>(sf::Texture());
    textures["tileGreen_02"] =
        std::make_shared<sf::Texture>(sf::Texture());
    if (!textures["ball"]->loadFromFile("./assets/ball.png") ||
        !textures["paddle"]->loadFromFile("./assets/paddle.png") ||
        !textures["tileGreen_01"]->loadFromFile("./assets/tileGreen_01.png") ||
        !textures["tileGreen_02"]->loadFromFile("./assets/tileGreen_02.png"))
    {
        return EXIT_FAILURE;
    }

    //======== initialize =========//

    Key arrowL, arrowR;

    sf::Vector2f cp, cn;
    sf::FloatRect bound;

    std::vector<std::shared_ptr<Entity>> world;

    std::vector<std::shared_ptr<ParticleSystem>> effects;
    std::vector<std::shared_ptr<sf::Texture>> effectTextures;
    effectTextures.push_back(textures["tileGreen_02"]);

    auto paddle = std::make_shared<Paddle>(*textures["paddle"], 5.5);
    paddle->setScale(0.08, 0.08);
    paddle->setPosition(300, 600);
    world.push_back(paddle);

    sf::Vector2f ballPrevPos;
    auto ball = std::make_shared<Ball>(*textures["ball"]);
    ball->vel = sf::Vector2f(2.5, -2.5);
    ball->setScale(0.1, 0.1);
    ball->setPosition(window.getSize().x / 2, 400);
    world.push_back(ball);

    auto fieldBounds = sf::RectangleShape(sf::Vector2f(
        window.getSize().x, window.getSize().y));
    fieldBounds.setPosition(0, 0);

    createTiles(
        world,
        *textures["tileGreen_01"],
        50, 50,
        5, 6,
        0.2, 0.2,
        20, 20);

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

        for (auto &&i : effects)
        {
            i->update(1.f);
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
                    if (cn.y == -1)
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

                    addEffect(effects, effectTextures, 5, cp.x, cp.y);

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