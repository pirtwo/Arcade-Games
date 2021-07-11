#include <SFML/Graphics.hpp>
#include "Particle.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 500), "Arkanoid");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.display();
    }
}