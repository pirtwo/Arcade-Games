#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <memory>
#include "Spacecraft.h"
#include "Particle.h"
#include "Projectile.h"

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
} upArrow, leftArrow, rightArrow, spacebar;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 700), "Asteroids");
    window.setFramerateLimit(60);

    //======== load assets =========
    map<string, shared_ptr<sf::Texture>> textures;
    textures["ship"] =
        shared_ptr<sf::Texture>(new sf::Texture);
    textures["greenLaser"] =
        shared_ptr<sf::Texture>(new sf::Texture);

    if (!textures["ship"]->loadFromFile("./assets/ship_01.png") ||
        !textures["greenLaser"]->loadFromFile("./assets/laserGreen.png"))
    {
        return EXIT_FAILURE;
    }

    //========= initialize ==========

    // player spacecraft
    auto ship = unique_ptr<Spacecraft>(
        new Spacecraft(*textures["ship"], 5.0, 4.5, 0.99, 0.1));
    ship->setPosition(sf::Vector2f(200, 200));
    ship->setScale(sf::Vector2f(0.5, 0.5));

    auto shipDust = unique_ptr<Emitter>(new Emitter(
        ship->getPosition().x,
        ship->getPosition().y,
        60.f, 120.f, 2.f, 4.f,
        4, 7, 100.f));

    // projectiles list
    list<unique_ptr<Projectile>> projs;

    auto spacecraftFire = [&]()
    {
        auto p = unique_ptr<Projectile>(new Projectile(
            *textures["greenLaser"], 7.5, ship->getRotation(), 500));
        p->setPosition(ship->getPosition());
        p->setRotation(ship->getRotation());
        p->setScale(0.5, 0.5);
        projs.push_back(std::move(p));
    };

    //=========== game loop ============
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
                case sf::Keyboard::Key::Space:
                    spacebar.press();
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
                case sf::Keyboard::Key::Space:
                    spacecraftFire();
                    spacebar.release();
                    break;
                default:
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
        if (ship->hasThrust())
            shipDust->addFuel(1);

        shipDust->update();
        shipDust->setMinAngle(ship->getRotation() + 180 - 10.f);
        shipDust->setMaxAngle(ship->getRotation() + 180 + 10.f);
        shipDust->setPosition(ship->getPosition());

        // update projectiles
        for (auto i = projs.begin(); i != projs.end(); i++)
        {
            auto proj = i->get();
            proj->update();
            if (proj->isBeyondRange())
            {
                i = projs.erase(i);
                if (i == projs.end())
                    break;
            }
        }

        //====== draw =========
        window.clear();

        window.draw(*shipDust);
        window.draw(*ship);

        // draw projectiles
        for (auto i = projs.begin(); i != projs.end(); i++)
            window.draw(*i->get());

        window.display();
    }

    return EXIT_SUCCESS;
}