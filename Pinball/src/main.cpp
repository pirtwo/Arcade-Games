#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include "Ball.h"
#include "Wall.h"
#include "Obstacle.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 500), "Pinball");

    //===== load assets ===//

    //===== initialize ====//
    sf::Clock clock;
    float timestepMS = 1000 / 60.f;
    float deltaMS = 0;
    float timestampMS = 0;
    float lastframeMS = 0;
    int updateCounter = 0;

    sf::FloatRect bounds(0, 0, window.getSize().x, window.getSize().y);

    auto ball = std::make_unique<Ball>(7.f);
    ball->setPosition(350, 250);

    std::vector<std::unique_ptr<Obstacle>> obstcales;
    obstcales.push_back(std::make_unique<Wall>(200, 10, sf::Color::Yellow));
    obstcales[0]->setPosition(300, 400);
    obstcales[0]->rotate(0);

    auto tick = [&](float delta)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            ball->applyForce(
                10.f, atan2(pos.y - ball->getPosition().y, pos.x - ball->getPosition().x));
        }

        ball->update(delta);
        for (auto &&i : obstcales)
            i->update();

        for (auto &&i : obstcales)
        {
            if (i->bounds().intersects(ball->bounds()))
            {
                ball->setPosition(ball->getPosition().x, i->getPosition().y - ball->getRadius() * 2);
                ball->bounceY();
            }
        }

        if (ball->getPosition().x < bounds.left)
        {
            ball->setPosition(ball->getRadius() + 1, ball->getPosition().y);
            ball->bounceX();
        }
        if (ball->getPosition().x > bounds.width)
        {
            ball->setPosition(bounds.width - ball->getRadius() - 1, ball->getPosition().y);
            ball->bounceX();
        }
        if (ball->getPosition().y - ball->getRadius() < bounds.top)
        {
            ball->setPosition(ball->getPosition().x, ball->getRadius() + 1);
            ball->bounceY();
        }
        if (ball->getPosition().y + ball->getRadius() > bounds.height)
        {
            ball->setPosition(ball->getPosition().x, bounds.height - ball->getRadius());
            ball->bounceY();
        }
    };

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        //====== update ====//
        timestampMS = clock.getElapsedTime().asMilliseconds();
        deltaMS += timestampMS - lastframeMS;
        lastframeMS = timestampMS;

        updateCounter = 0;
        while (deltaMS > timestepMS)
        {
            tick(timestepMS);
            deltaMS -= timestepMS;
            if (updateCounter++ > 20)
                break;
        }

        //====== draw ======//
        window.clear();
        window.draw(*ball);
        for (auto &&i : obstcales)
            window.draw(*i);
        window.display();
    }

    return EXIT_SUCCESS;
}