#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include "Asteroid.h"
#include "Spacecraft.h"
#include "Particle.h"
#include "Projectile.h"
#include "Keyboard.h"
#include "Helper.h"

using namespace std;

enum Actor
{
    PLAYER,
    CPU
};

struct Settings
{
    const int MaxAstroidNum = 5;
} settings;

struct Player
{
    int hp = 3;
    int score = 0;
} player;

void keepInWindow(sf::Transformable &item, sf::RenderWindow &window);
bool checkCollision(sf::Sprite &a, sf::Sprite &b);

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
    textures["asteroid_01"] =
        shared_ptr<sf::Texture>(new sf::Texture);
    textures["asteroid_02"] =
        shared_ptr<sf::Texture>(new sf::Texture);
    textures["asteroid_03"] =
        shared_ptr<sf::Texture>(new sf::Texture);
    textures["asteroid_04"] =
        shared_ptr<sf::Texture>(new sf::Texture);

    if (!textures["ship"]->loadFromFile("./assets/ship_01.png") ||
        !textures["greenLaser"]->loadFromFile("./assets/laserGreen.png") ||
        !textures["asteroid_01"]->loadFromFile("./assets/meteor_01.png") ||
        !textures["asteroid_02"]->loadFromFile("./assets/meteor_02.png") ||
        !textures["asteroid_03"]->loadFromFile("./assets/meteor_03.png") ||
        !textures["asteroid_04"]->loadFromFile("./assets/meteor_04.png"))
    {
        return EXIT_FAILURE;
    }

    //========= initialize ==========

    srand(time(NULL));

    Keyboard upArrow, leftArrow, rightArrow, spacebar;

    list<unique_ptr<Spacecraft>> spacecrafts;
    list<unique_ptr<Asteroid>> asteroids;
    list<unique_ptr<Emitter>> effects;
    list<unique_ptr<Projectile>> projectiles;

    // player spacecraft
    auto ship = unique_ptr<Spacecraft>(
        new Spacecraft(*textures["ship"], 5.0, 4.5, 0.99, 0.1));
    ship->setPosition(sf::Vector2f(200, 200));
    ship->setScale(sf::Vector2f(0.5, 0.5));

    auto shipDust = unique_ptr<Emitter>(new Emitter(
        ship->getPosition().x,
        ship->getPosition().y,
        60.f, 120.f,
        2.f, 4.f,
        4, 7));

    auto spacecraftFire = [&](int actor)
    {
        auto p = unique_ptr<Projectile>(new Projectile(
            *textures["greenLaser"], 7.5, ship->getRotation(), 500));
        p->setActor(actor);
        p->setPosition(ship->getPosition());
        p->setRotation(ship->getRotation());
        p->setScale(0.5, 0.5);
        projectiles.push_back(std::move(p));
    };

    auto spawnAsteroid = [&](float x, float y, float minAngle, float maxAngle, int hp)
    {
        auto elm = unique_ptr<Asteroid>(new Asteroid(
            *textures["asteroid_01"], hp,
            randFloat(0.5, 1.5), randFloat(minAngle, maxAngle)));

        if (hp == 1)
        {
            elm->setScale(sf::Vector2f(0.4, 0.4));
        }
        else if (hp == 2)
        {
            elm->setScale(sf::Vector2f(0.7, 0.7));
        }
        else if (hp == 3)
        {
            elm->setScale(sf::Vector2f(0.9, 0.9));
        }

        elm->setPosition(x, y);
        elm->setRotation(randFloat(0.f, 360.f));
        asteroids.push_back(std::move(elm));
    };

    auto spawnDust = [&](float x, float y, float minAngle, float maxAngle, int num)
    {
        auto effect = unique_ptr<Emitter>(new Emitter(
            x, y, minAngle, maxAngle, 1.f, 2.f, 4, 7));
        effect->addFuel(num);
        effects.push_back(std::move(effect));
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
                    spacecraftFire(Actor::PLAYER);
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
        if (upArrow.isUp)
            ship->reverseThrust();

        shipDust->update();
        shipDust->setMinAngle(ship->getRotation() + 180 - 10.f);
        shipDust->setMaxAngle(ship->getRotation() + 180 + 10.f);
        shipDust->setPosition(ship->getPosition());

        for (auto i = spacecrafts.begin(); i != spacecrafts.end(); i++)
        {
            auto elm = i->get();
            elm->update();
            keepInWindow(*elm, window);
            if (elm->hasThrust())
                spawnDust(
                    elm->getPosition().x,
                    elm->getPosition().y,
                    elm->getRotation() + 180 - 10.f,
                    elm->getRotation() + 180 + 10.f, 5);
        }

        //update asteroids
        for (auto i = asteroids.begin(); i != asteroids.end(); i++)
        {
            auto elm = i->get();
            elm->update();
            keepInWindow(*elm, window);
        }
        // spawn asteroids
        if (asteroids.size() < settings.MaxAstroidNum)
        {
            int n = settings.MaxAstroidNum - asteroids.size();
            for (int i = 0; i < n; i++)
                spawnAsteroid(0.f, randFloat(0, window.getSize().x), 0.f, 360.f, randInt(1, 3));
        }

        // update projectiles
        for (auto i = projectiles.begin(); i != projectiles.end(); i++)
        {
            auto elm = i->get();
            elm->update();
            keepInWindow(*elm, window);

            if (elm->isBeyondRange())
            {
                i = projectiles.erase(i);
                if (i == projectiles.end())
                    break;
            }
        }

        // update effects
        for (auto i = effects.begin(); i != effects.end(); i++)
        {
            auto elm = i->get();
            elm->update();
            if (elm->getFule() <= 0)
            {
                i = effects.erase(i);
                if (i == effects.end())
                    break;
            }
        }

        // check collisions
        for (auto i = projectiles.begin(); i != projectiles.end(); i++)
        {
            auto p = i->get();
            bool hasCollision = false;

            for (auto j = asteroids.begin(); j != asteroids.end(); j++)
            {
                auto a = j->get();
                if (checkCollision(*a, *p))
                {
                    a->takeHit();
                    spawnDust(
                        p->getPosition().x,
                        p->getPosition().y,
                        0.f, 360.f, 10);

                    if (a->getHP() <= 0 && a->getMaxHP() > 1)
                    {
                        spawnAsteroid(
                            a->getPosition().x,
                            a->getPosition().y,
                            a->getAngle() - 45,
                            a->getAngle() + 45,
                            a->getMaxHP() - 1);
                        spawnAsteroid(
                            a->getPosition().x,
                            a->getPosition().y,
                            a->getAngle() - 45,
                            a->getAngle() + 45,
                            a->getMaxHP() - 1);
                        spawnAsteroid(
                            a->getPosition().x,
                            a->getPosition().y,
                            a->getAngle() - 45,
                            a->getAngle() + 45,
                            a->getMaxHP() - 1);
                    }

                    if (a->getHP() <= 0)
                        asteroids.erase(j);

                    hasCollision = true;
                    break;
                }
            }

            if (hasCollision)
            {
                i = projectiles.erase(i);
                if (i == projectiles.end())
                    break;
            }
        }

        for (auto i = asteroids.begin(); i != asteroids.end(); i++)
        {
            auto elm = i->get();
            if (checkCollision(*ship, *elm))
            {
                player.hp--;
                spawnDust(
                    ship->getPosition().x,
                    ship->getPosition().y,
                    0.f, 360.f, 10);
            }
        }

        //====== draw =========
        window.clear();

        for (auto i = spacecrafts.begin(); i != spacecrafts.end(); i++)
            window.draw(*i->get());

        for (auto i = asteroids.begin(); i != asteroids.end(); i++)
            window.draw(*i->get());

        for (auto i = projectiles.begin(); i != projectiles.end(); i++)
            window.draw(*i->get());

        for (auto i = effects.begin(); i != effects.end(); i++)
            window.draw(*i->get());

        window.display();
    }

    return EXIT_SUCCESS;
}

void keepInWindow(sf::Transformable &item, sf::RenderWindow &window)
{
    if (item.getPosition().x < 0)
        item.setPosition(sf::Vector2f(window.getSize().x, item.getPosition().y));
    if (item.getPosition().x > window.getSize().x)
        item.setPosition(sf::Vector2f(0, item.getPosition().y));
    if (item.getPosition().y < 0)
        item.setPosition(sf::Vector2f(item.getPosition().x, window.getSize().y));
    if (item.getPosition().y > window.getSize().y)
        item.setPosition(sf::Vector2f(item.getPosition().x, 0));
}

bool checkCollision(sf::Sprite &a, sf::Sprite &b)
{
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}