#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Spacecraft.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 700), "Asteroids");
    window.setFramerateLimit(60);

    vector<shared_ptr<sf::Texture>> textures;
    textures.push_back(shared_ptr<sf::Texture>(new sf::Texture));

    if (!textures[0]->loadFromFile("./assets/ship_01.png"))
    {
        return EXIT_FAILURE;
    }

    Spacecraft *ship = new Spacecraft(*textures[0]);
    ship->setPosition(sf::Vector2f(200, 200));
    ship->setScale(sf::Vector2f(0.5, 0.5));

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            if (e.type == sf::Event::KeyPressed)
            {
                switch (e.key.code)
                {
                case sf::Keyboard::Key::Up:
                    ship->forward();
                    break;
                case sf::Keyboard::Key::Left:
                    ship->turnLeft();
                    break;
                case sf::Keyboard::Key::Right:
                    ship->turnRight();
                    break;
                default:
                    break;
                }
            }
            if (e.type == sf::Event::KeyReleased)
            {
            }
        }

        //====== update =======
        ship->update();

        //====== draw =========
        window.clear();

        window.draw(*ship);

        window.display();
    }

    return EXIT_SUCCESS;
}