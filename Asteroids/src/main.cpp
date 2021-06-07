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
    const int RateOfFire = 400; // bullet per ms
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
    sf::Font font;

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

    if (!font.loadFromFile("./assets/kenvector_future.ttf") ||
        !textures["ship"]->loadFromFile("./assets/ship_01.png") ||
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

    sf::Clock clock;

    Keyboard upArrow, leftArrow, rightArrow, spacebar;

    list<unique_ptr<Spacecraft>> spacecrafts;
    list<unique_ptr<Asteroid>> asteroids;
    list<unique_ptr<Emitter>> effects;
    list<unique_ptr<Projectile>> projectiles;

    char playerScore[20];
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(window.getSize().x - 200, 50);

    char playerHP[20];
    sf::Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(50, 50);

    auto spawnPlayer =
        [&](float x, float y)
    {
        auto elm = unique_ptr<Spacecraft>(
            new Spacecraft(*textures["ship"], 5.0, 4.5, 0.99, 0.1));
        elm->setActor(Actor::PLAYER);
        elm->setScale(sf::Vector2f(0.5, 0.5));
        elm->setPosition(sf::Vector2f(x, y));
        spacecrafts.push_back(std::move(elm));
    };

    auto spacecraftFire =
        [&](Spacecraft &s)
    {
        auto p = unique_ptr<Projectile>(new Projectile(
            *textures["greenLaser"], 7.5, s.getRotation(), 500));
        p->setActor(s.getActor());
        p->setPosition(s.getPosition());
        p->setRotation(s.getRotation());
        p->setScale(0.5, 0.5);
        projectiles.push_back(std::move(p));
    };

    auto findEmptyArea =
        [&](int w, int h) -> sf::Vector2i
    {
        sf::FloatRect mask(0, 0, w, h);
        for (int y = 0; y < window.getSize().y - mask.height; y++)
        {
            for (int x = 0; x < window.getSize().x - mask.width; x++)
            {
                mask.top = y;
                mask.left = x;
                bool collision = false;
                for (auto i = asteroids.begin(); i != asteroids.end(); i++)
                {
                    if (mask.intersects(i->get()->getGlobalBounds()))
                    {
                        collision = true;
                        break;
                    }
                }

                if (!collision)
                    return sf::Vector2i(x, y);
            }
        }

        return sf::Vector2i(-1, -1);
    };

    auto spawnAsteroid =
        [&](
            float x,
            float y,
            float minAngle,
            float maxAngle,
            int hp)
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

    auto spawnDust =
        [&](
            int num,
            float x,
            float y,
            float minAngle,
            float maxAngle,
            float minSpeed,
            float maxSpeed,
            int minFade,
            int maxFade)
    {
        auto effect = unique_ptr<Emitter>(new Emitter(
            x, y, minAngle, maxAngle, minSpeed, maxSpeed, minFade, maxFade));
        effect->addFuel(num);
        effects.push_back(std::move(effect));
    };

    spawnPlayer(window.getSize().x / 2, window.getSize().y / 2);

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
                    spacebar.release();
                    break;
                default:
                    break;
                }
            }
        }

        //====== update =======//

        snprintf(playerScore, 20, "score: %d", player.score);
        scoreText.setString(playerScore);

        snprintf(
            playerHP, 20,
            "hp: %s%s%s",
            player.hp == 1 ? "|" : "",
            player.hp == 2 ? "||" : "",
            player.hp == 3 ? "|||" : "");
        hpText.setString(playerHP);

        // update spacecrafts
        for (auto i = spacecrafts.begin(); i != spacecrafts.end(); i++)
        {
            auto elm = i->get();
            elm->update();
            keepInWindow(*elm, window);

            // spawn thrust dust
            if (elm->hasThrust())
                spawnDust(
                    2,
                    elm->getPosition().x,
                    elm->getPosition().y,
                    elm->getRotation() + 180 - 10.f,
                    elm->getRotation() + 180 + 10.f,
                    1.f, 4.f,
                    5, 7);

            if (elm->getActor() == Actor::PLAYER)
            {
                if (upArrow.isDown)
                    elm->thrust();
                if (upArrow.isUp)
                    elm->reverseThrust();
                if (leftArrow.isDown)
                    elm->turnLeft();
                if (rightArrow.isDown)
                    elm->turnRight();
                if (spacebar.isDown &&
                    clock.getElapsedTime().asMilliseconds() > settings.RateOfFire)
                {
                    clock.restart();
                    spacecraftFire(*elm);
                }
            }
            else
            {
                // update cpu spacecrafts
            }
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
                spawnAsteroid(
                    randFloat(0, window.getSize().x),
                    0.f, 0.f, 360.f, randInt(1, 3));
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

        // check projectile collision
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
                        10,
                        p->getPosition().x,
                        p->getPosition().y,
                        0.f, 360.f,
                        1.f, 2.f,
                        5, 7);

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

                    if (a->getHP() <= 0 && a->getMaxHP() == 1)
                    {
                        player.score += 10;
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

        // check asteroids collision
        for (auto i = asteroids.begin(); i != asteroids.end(); i++)
        {
            auto elm = i->get();

            for (auto j = spacecrafts.begin(); j != spacecrafts.end(); j++)
            {
                auto sc = j->get();
                if (checkCollision(*sc, *elm))
                {
                    // TODO: play crash sound

                    spawnDust(
                        20,
                        sc->getPosition().x,
                        sc->getPosition().y,
                        0.f, 360.f,
                        1.f, 2.f,
                        3, 5);

                    if (sc->getActor() == Actor::PLAYER)
                    {
                        player.hp--;
                        // TODO: check for game over
                        auto spawnArea = findEmptyArea(300, 300);
                        spawnPlayer(spawnArea.x + 150, spawnArea.y + 150);
                    }

                    /* 
                        remove spacecraft from scene
                    */
                    j = spacecrafts.erase(j);
                    if (j == spacecrafts.end())
                        break;
                }
            }
        }

        //====== draw =========
        window.clear();

        window.draw(hpText);
        window.draw(scoreText);

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