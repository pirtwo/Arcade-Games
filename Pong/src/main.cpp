#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Line.h"
#include "Ball.h"
#include "Paddle.h"

struct Config
{
    int laneThickness = 10;
    float ballSize = 5.f;
    sf::Color ballColor = sf::Color::Yellow;
    float paddleSpeed = 4.5;
    float paddleWid = 10.f;
    float paddleHig = 70.f;
    float paddleMargin = 20.f;
    sf::Color paddleColor = sf::Color::Blue;
} config;

struct Key
{
    bool isDown = false;
} upArrow, downArrow;

void limitPaddleMove(Paddle &paddle, float minY, float maxY);

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 500), "Pong");
    window.setFramerateLimit(60);

    //========== load assets ===========//

    //========== initialize ===========//
    auto ball = std::unique_ptr<Ball>(
        new Ball(config.ballSize, config.ballSize, config.ballColor));

    auto lPaddle = std::unique_ptr<Paddle>(
        new Paddle(config.paddleWid, config.paddleHig, config.paddleColor));
    auto rPaddle = std::unique_ptr<Paddle>(
        new Paddle(config.paddleWid, config.paddleHig, config.paddleColor));
    lPaddle->speed = config.paddleSpeed;
    rPaddle->speed = config.paddleSpeed;
    lPaddle->setPosition(config.paddleMargin, window.getSize().y / 2);
    rPaddle->setPosition(window.getSize().x - config.paddleMargin, window.getSize().y / 2);

    auto toplane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(0, 0),
        sf::Vector2f(window.getSize().x, 0),
        config.laneThickness, sf::Color::Red));
    auto midlane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(window.getSize().x / 2 - config.laneThickness / 2.f, 0),
        sf::Vector2f(window.getSize().x / 2 - config.laneThickness / 2.f, window.getSize().y),
        15, 35.f, config.laneThickness, sf::Color::Red));
    auto botlane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(0, window.getSize().y - config.laneThickness),
        sf::Vector2f(window.getSize().x, window.getSize().y - config.laneThickness),
        config.laneThickness, sf::Color::Red));

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::KeyPressed)
            {
                switch (e.key.code)
                {
                case sf::Keyboard::Up:
                    upArrow.isDown = true;
                    break;
                case sf::Keyboard::Down:
                    downArrow.isDown = true;
                    break;
                default:
                    break;
                }
            }
            else if (e.type == sf::Event::KeyReleased)
            {
                switch (e.key.code)
                {
                case sf::Keyboard::Up:
                    upArrow.isDown = false;
                    break;
                case sf::Keyboard::Down:
                    downArrow.isDown = false;
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

        if (upArrow.isDown)
            lPaddle->moveUp();
        if (downArrow.isDown)
            lPaddle->moveDown();
        if (!upArrow.isDown && !downArrow.isDown)
            lPaddle->stop();

        ball->update();
        lPaddle->update();
        rPaddle->update();

        limitPaddleMove(
            *lPaddle,
            config.laneThickness,
            window.getSize().y - config.paddleHig - config.laneThickness);
        limitPaddleMove(
            *rPaddle,
            config.laneThickness,
            window.getSize().y - config.paddleHig - config.laneThickness);

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