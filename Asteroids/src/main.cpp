#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Spacecraft.h"
#include "Emitter.h"

using namespace std;

struct Key
{
    bool isDown = false;
    bool isUp = true;
    void press()
    {
        isDown = true;
        isUp = false;
    }
    void release()
    {
        isDown = false;
        isUp = true;
    }
} upArrow, leftArrow, rightArrow;

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

    Spacecraft *ship = new Spacecraft(*textures[0], 5.0, 4.5, 0.99, 0.1);
    ship->setPosition(sf::Vector2f(200, 200));
    ship->setScale(sf::Vector2f(0.5, 0.5));

    Emitter *emitter = new Emitter(100.f, 100.f, 20, 60.f, 120.f, 2.f, 4.f, 1.f, 2.f, 100.f);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (e.type == sf::Event::KeyPressed)
            {
                switch (e.key.code)
                {
                case sf::Keyboard::Key::Up:
                    upArrow.press();
                    break;
                case sf::Keyboard::Key::Left:
                    leftArrow.press();
                    break;
                case sf::Keyboard::Key::Right:
                    rightArrow.press();
                    break;
                default:
                    break;
                }
            }
            else if (e.type == sf::Event::KeyReleased)
            {
                switch (e.key.code)
                {
                case sf::Keyboard::Key::Up:
                    upArrow.release();
                    break;
                case sf::Keyboard::Key::Left:
                    leftArrow.release();
                    break;
                case sf::Keyboard::Key::Right:
                    rightArrow.release();
                    break;
                }
            }
        }

        //====== update =======
        if (leftArrow.isDown)
            ship->turnLeft();
        if (rightArrow.isDown)
            ship->turnRight();
        if (upArrow.isDown)
            ship->thrust();
        else
            ship->reverseThrust();

        // keep spaceship inside the window
        if (ship->getPosition().x < 0)
            ship->setPosition(sf::Vector2f(window.getSize().x, ship->getPosition().y));
        if (ship->getPosition().x > window.getSize().x)
            ship->setPosition(sf::Vector2f(0, ship->getPosition().y));
        if (ship->getPosition().y < 0)
            ship->setPosition(sf::Vector2f(ship->getPosition().x, window.getSize().y));
        if (ship->getPosition().y > window.getSize().y)
            ship->setPosition(sf::Vector2f(ship->getPosition().x, 0));

        ship->update();
        emitter->update();

        //====== draw =========
        window.clear();

        window.draw(*ship);
        window.draw(*emitter);

        window.display();
    }

    delete emitter;

    return EXIT_SUCCESS;
}