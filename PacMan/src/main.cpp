#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 700), "PacMan");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        //====== update ======//

        //====== draw   ======//
        window.clear();
        window.display();
    }

    return EXIT_SUCCESS;
}