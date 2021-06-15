#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <memory>
#include "Line.h"
#include "Ball.h"
#include "Paddle.h"

struct Config
{
    int laneThickness = 10;
    float ballSize = 17.f;
    float ballMinSpeed = 5.f;
    float ballMaxSpeed = 11.5f;
    sf::Color ballColor = sf::Color::Yellow;
    float paddleSpeed = 8.5;
    float paddleWid = 12.f;
    float paddleHig = 85.f;
    float paddleMargin = 20.f;
    sf::Color paddleColor = sf::Color::Blue;
} config;

struct Key
{
    bool isDown = false;
} wKey, sKey, upKey, downKey;

struct State
{
    int playerOneScore = 0;
    int playerTwoScore = 0;
    bool hasKickoff = false;
    bool ballTouched = false;
} state;

float randFloat(float n, float m);
void limitPaddleMove(Paddle &paddle, float minY, float maxY);
bool checkCollision(sf::FloatRect a, sf::FloatRect b);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 500), "Pong");
    window.setFramerateLimit(60);

    //========== load assets ===========//

    //========== initialize ===========//
    srand(time(0));

    auto ball = std::unique_ptr<Ball>(
        new Ball(config.ballSize, config.ballSize, config.ballColor));
    ball->setPosition(
        window.getSize().x / 2 - config.ballSize / 2,
        window.getSize().y / 2 - config.ballSize / 2);

    auto lPaddle = std::unique_ptr<Paddle>(
        new Paddle(config.paddleWid, config.paddleHig, config.paddleColor));
    auto rPaddle = std::unique_ptr<Paddle>(
        new Paddle(config.paddleWid, config.paddleHig, config.paddleColor));
    lPaddle->speed = config.paddleSpeed;
    rPaddle->speed = config.paddleSpeed;
    lPaddle->setPosition(
        config.paddleMargin,
        window.getSize().y / 2 - config.paddleHig / 2);
    rPaddle->setPosition(
        window.getSize().x - config.paddleMargin,
        window.getSize().y / 2 - config.paddleHig / 2);

    auto toplane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(0, 0),
        sf::Vector2f(window.getSize().x, 0),
        config.laneThickness, sf::Color::Red));
    auto midlane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(window.getSize().x / 2 + config.laneThickness / 2.f, 0),
        sf::Vector2f(window.getSize().x / 2 + config.laneThickness / 2.f, window.getSize().y),
        15, 35.f, config.laneThickness, sf::Color::Red));
    auto botlane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(0, window.getSize().y - config.laneThickness),
        sf::Vector2f(window.getSize().x, window.getSize().y - config.laneThickness),
        config.laneThickness, sf::Color::Red));

    sf::FloatRect fieldBounds[4] = {
        // top lane
        sf::FloatRect(
            0,
            0,
            window.getSize().x,
            config.laneThickness),
        // bot lane
        sf::FloatRect(
            0,
            window.getSize().y - config.laneThickness,
            window.getSize().x,
            config.laneThickness),
        // left lane
        sf::FloatRect(
            -config.laneThickness,
            0,
            config.laneThickness,
            window.getSize().y),
        // right lane
        sf::FloatRect(
            window.getSize().x,
            0,
            config.laneThickness,
            window.getSize().y),
    };

    auto kickoff = [&]()
    {
        state.hasKickoff = true;
        state.ballTouched = false;
        ball->setPosition(
            window.getSize().x / 2 - config.ballSize / 2,
            window.getSize().y / 2 - config.ballSize / 2);

        float r = rand() / (RAND_MAX + 1.f);
        ball->setAngle(r < 0.5 ? randFloat(120, 210) : randFloat(330, 390));
        ball->setSpeed(config.ballMinSpeed);
    };

    kickoff();

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::KeyPressed)
            {
                switch (e.key.code)
                {
                case sf::Keyboard::W:
                    wKey.isDown = true;
                    break;
                case sf::Keyboard::S:
                    sKey.isDown = true;
                    break;
                case sf::Keyboard::Up:
                    upKey.isDown = true;
                    break;
                case sf::Keyboard::Down:
                    downKey.isDown = true;
                    break;
                default:
                    break;
                }
            }
            else if (e.type == sf::Event::KeyReleased)
            {
                switch (e.key.code)
                {
                case sf::Keyboard::W:
                    wKey.isDown = false;
                    break;
                case sf::Keyboard::S:
                    sKey.isDown = false;
                    break;
                case sf::Keyboard::Up:
                    upKey.isDown = false;
                    break;
                case sf::Keyboard::Down:
                    downKey.isDown = false;
                    break;
                default:
                    break;
                }
            }
            else if (e.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        //========== update ===========//
        ball->update();
        lPaddle->update();
        rPaddle->update();

        if (state.hasKickoff && state.ballTouched)
        {
            state.hasKickoff = false;
            ball->setSpeed(config.ballMaxSpeed);
        }

        if (wKey.isDown)
            lPaddle->moveUp();
        if (sKey.isDown)
            lPaddle->moveDown();
        if (!wKey.isDown && !sKey.isDown)
            lPaddle->stop();

        if (upKey.isDown)
            rPaddle->moveUp();
        if (downKey.isDown)
            rPaddle->moveDown();
        if (!upKey.isDown && !downKey.isDown)
            rPaddle->stop();

        limitPaddleMove(
            *lPaddle,
            config.laneThickness,
            window.getSize().y - config.paddleHig - config.laneThickness);
        limitPaddleMove(
            *rPaddle,
            config.laneThickness,
            window.getSize().y - config.paddleHig - config.laneThickness);

        if (checkCollision(lPaddle->getBody(), ball->getBody()))
        {
            state.ballTouched = true;
            if (wKey.isDown)
            {
                ball->setAngle(randFloat(315, 350));
            }
            else if (sKey.isDown)
            {
                ball->setAngle(randFloat(10, 45));
            }
            else
            {
                ball->setAngle(randFloat(350, 370));
            }

            ball->setPosition(
                lPaddle->getPosition().x + config.paddleWid + 5,
                ball->getPosition().y);
        }
        else if (checkCollision(rPaddle->getBody(), ball->getBody()))
        {
            state.ballTouched = true;
            if (upKey.isDown)
            {
                ball->setAngle(randFloat(190, 225));
            }
            else if (downKey.isDown)
            {
                ball->setAngle(randFloat(135, 170));
            }
            else
            {
                ball->setAngle(randFloat(170, 190));
            }

            ball->setPosition(
                rPaddle->getPosition().x - config.ballSize - 5,
                ball->getPosition().y);
        }
        else if (checkCollision(ball->getBody(), fieldBounds[0]))
        {
            ball->bounceY();
            ball->setPosition(ball->getPosition().x, fieldBounds[0].height + 5);
        }
        else if (checkCollision(ball->getBody(), fieldBounds[1]))
        {
            ball->bounceY();
            ball->setPosition(ball->getPosition().x, fieldBounds[1].top - config.ballSize - 5);
        }
        else if (checkCollision(ball->getBody(), fieldBounds[2]))
        {
            kickoff();
        }
        else if (checkCollision(ball->getBody(), fieldBounds[3]))
        {
            kickoff();
        }

        //========== draw   ===========//
        window.clear();
        window.draw(*toplane);
        window.draw(*midlane);
        window.draw(*botlane);
        window.draw(*lPaddle);
        window.draw(*rPaddle);
        window.draw(*ball);
        window.display();
    }

    return EXIT_SUCCESS;
}

void limitPaddleMove(Paddle &paddle, float minY, float maxY)
{
    if (paddle.getPosition().y < minY)
        paddle.setPosition(paddle.getPosition().x, minY);

    if (paddle.getPosition().y > maxY)
        paddle.setPosition(paddle.getPosition().x, maxY);
}

bool checkCollision(sf::FloatRect a, sf::FloatRect b)
{
    bool overlapX = false;
    bool overlapY = false;

    if (a.left < b.left)
        overlapX = b.left - a.left < a.width;
    else
        overlapX = a.left - b.left < b.width;

    if (a.top < b.top)
        overlapY = b.top - a.top < a.height;
    else
        overlapY = a.top - b.top < b.height;

    return overlapX && overlapY;
}

float randFloat(float n, float m)
{
    return n + (m - n) * (rand() / (RAND_MAX + 1.f));
}
