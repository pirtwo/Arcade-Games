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

        //======== update =========//

        //======== draw ==========//
        window.clear();

        window.display();
    }

    return EXIT_SUCCESS;
}