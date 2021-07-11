#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <memory>
#include "Key.h"
#include "Paddle.h"
#include "Particle.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 500), "Arkanoid");
    window.setFramerateLimit(60);

    Key arrowL, arrowR;

    std::map<std::string, std::shared_ptr<sf::Texture>> textures;
    textures["paddle"] = std::make_shared<sf::Texture>();

    if (!textures["paddle"]->loadFromFile("./assets/paddle.png"))
    {
        return EXIT_FAILURE;
    }

    auto paddle = std::make_shared<Paddle>(*textures["paddle"]);
    paddle->setSpeed(1.2);
    paddle->setScale(0.2, 0.2);
    paddle->setPosition(50, 450);

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

        //======= update =========//
        if (arrowL.isDown)
            paddle->slideLeft();
        if (arrowR.isDown)
            paddle->slideRight();
        if (!arrowL.isDown && !arrowR.isDown)
            paddle->stop();

        paddle->update(1.0);

        //======== draw ==========//
        window.clear();

        window.draw(*paddle);

        window.display();
    }

    return EXIT_SUCCESS;
}