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
    const int PlayerRateOfFire = 400;
    const int PlayerSpawnDelayMS = 1000;
    const int EnemyRateOfFire = 400;
    const int EnemySpawnRateMS = 10000;
    const float EnemySpeedMin = 1.f;
    const float EnemySpeedMax = 2.f;
} settings;

struct State
{
    bool pause = false;
    bool gameover = true;
    bool showHelp = false;
    bool hasEnemySpawn = false;
    bool hasPlayerSpawn = false;
    int hp = 3;
    int score = 0;
    int lastPlayerFireMS = 0;
    int lastPlayerSpawnMS = 0;
    int lastEnemyFireMS = 0;
    int lastEnemySpawnMS = 0;
} state;

void keepInWindow(sf::Transformable &item, sf::RenderWindow &window);
bool checkCollision(sf::Sprite &a, sf::Sprite &b);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 700), "Asteroids");
    window.setFramerateLimit(60);

    //======== load assets =========
    sf::Font font;

    map<string, shared_ptr<sf::Texture>> textures;
    textures["playerShip"] =
        shared_ptr<sf::Texture>(new sf::Texture);
    textures["enemyShip"] =
        shared_ptr<sf::Texture>(new sf::Texture);
    textures["redLaser"] =
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
        !textures["playerShip"]->loadFromFile("./assets/ship_01.png") ||
        !textures["enemyShip"]->loadFromFile("./assets/enemy_01.png") ||
        !textures["redLaser"]->loadFromFile("./assets/laserRed.png") ||
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
    float elapsed = 0;

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

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::Red);
    titleText.setString("asteroids");
    titleText.setPosition(
        window.getSize().x / 2 - titleText.getGlobalBounds().width / 2,
        window.getSize().y / 2 - titleText.getGlobalBounds().height / 2 - 100);

    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setCharacterSize(30);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("pause");
    pauseText.setPosition(
        window.getSize().x / 2 - pauseText.getGlobalBounds().width / 2,
        window.getSize().y / 2 - pauseText.getGlobalBounds().height / 2);

    sf::Text gameoverText;
    gameoverText.setFont(font);
    gameoverText.setCharacterSize(30);
    gameoverText.setFillColor(sf::Color::White);
    gameoverText.setString("gameover");
    gameoverText.setPosition(
        window.getSize().x / 2 - gameoverText.getGlobalBounds().width / 2,
        window.getSize().y / 2 - gameoverText.getGlobalBounds().height / 2);

    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(20);
    menuText.setFillColor(sf::Color::White);
    menuText.setString(
        "[n] new game\r\n[p] pause\r\n[h] help");
    menuText.setPosition(
        window.getSize().x / 2 - menuText.getGlobalBounds().width / 2,
        window.getSize().y / 2 - menuText.getGlobalBounds().height / 2 + 30);

    char highscore[20];
    sf::Text highscoreText;
    highscoreText.setFont(font);
    highscoreText.setCharacterSize(20);
    highscoreText.setFillColor(sf::Color::White);
    highscoreText.setString("high score: ");
    highscoreText.setPosition(
        window.getSize().x / 2 - highscoreText.getGlobalBounds().width / 2,
        window.getSize().y / 2 - highscoreText.getGlobalBounds().height / 2 + 100);

    sf::Text helpText;
    helpText.setFont(font);
    helpText.setCharacterSize(12);
    helpText.setFillColor(sf::Color::White);
    helpText.setString(
        "[up] thurst  [left] rotate left  [right] rotate right  [space] shoot");
    helpText.setPosition(
        window.getSize().x / 2 - helpText.getGlobalBounds().width / 2,
        window.getSize().y - helpText.getGlobalBounds().height - 30);

    auto spawnSpacecraft =
        [&](int actor, float x, float y)
    {
        auto elm = unique_ptr<Spacecraft>(
            new Spacecraft(
                actor == Actor::PLAYER ? *textures["playerShip"] : *textures["enemyShip"], 5.0, 4.5, 0.99, 0.1));
        elm->setActor(actor);
        elm->setScale(sf::Vector2f(0.5, 0.5));
        elm->setPosition(sf::Vector2f(x, y));
        spacecrafts.push_back(std::move(elm));
    };

    auto spacecraftFire =
        [&](Spacecraft &s, float angle)
    {
        auto p = unique_ptr<Projectile>(new Projectile(
            s.getActor() == Actor::PLAYER ? *textures["greenLaser"] : *textures["redLaser"],
            7.5, angle, 500));
        p->setActor(s.getActor());
        p->setPosition(s.getPosition());
        p->setRotation(angle);
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

    auto gameover = [&]()
    {
        state.gameover = true;
        // TODO: play gameover sound
    };

    auto newgame = [&]()
    {
        clock.restart();

        state.hp = 3;
        state.score = 0;
        state.pause = false;
        state.gameover = false;

        state.hasPlayerSpawn = false;
        state.lastPlayerFireMS = 0;

        state.hasEnemySpawn = false;
        state.lastEnemyFireMS = 0;
        state.lastEnemySpawnMS = 0;

        spacecrafts.clear();
        projectiles.clear();
        asteroids.clear();
        effects.clear();
        spawnSpacecraft(
            Actor::PLAYER,
            window.getSize().x / 2,
            window.getSize().y / 2);
    };

    auto respawn = [&]()
    {
        auto spawnArea = findEmptyArea(300, 300);
        spawnSpacecraft(Actor::PLAYER, spawnArea.x + 150, spawnArea.y + 150);
    };

    //=========== game loop ============//
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
                case sf::Keyboard::Key::P:
                    if (!state.gameover)
                        state.pause = !state.pause;
                    break;
                case sf::Keyboard::Key::H:
                    state.showHelp = !state.showHelp;
                    break;
                case sf::Keyboard::Key::N:
                    newgame();
                    break;
                default:
                    break;
                }
            }
        }

        //======== update =======//

        if (!state.pause)
        {
            elapsed = clock.getElapsedTime().asMilliseconds();

            snprintf(playerScore, 20, "score: %d", state.score);
            scoreText.setString(playerScore);

            snprintf(
                playerHP, 20,
                "hp: %s%s%s",
                state.hp == 1 ? "|" : "",
                state.hp == 2 ? "||" : "",
                state.hp == 3 ? "|||" : "");
            hpText.setString(playerHP);

            // check for enemy spawn
            if (!state.gameover)
            {
                state.hasEnemySpawn = elapsed >
                                      state.lastEnemySpawnMS + settings.EnemySpawnRateMS;
            }
            if (state.hasEnemySpawn)
            {
                spawnSpacecraft(Actor::CPU, 0, randFloat(0, window.getSize().y));
                state.lastEnemySpawnMS = elapsed;
            }

            // spawn player
            if (state.hasPlayerSpawn && elapsed >
                                            state.lastPlayerSpawnMS + settings.PlayerSpawnDelayMS)
            {
                auto spawnArea = findEmptyArea(300, 300);
                spawnSpacecraft(Actor::PLAYER, spawnArea.x + 150, spawnArea.y + 150);
                state.hasPlayerSpawn = false;
            }

            // update spacecrafts
            for (auto i = spacecrafts.begin(); i != spacecrafts.end(); i++)
            {
                auto s = i->get();
                s->update();
                keepInWindow(*s, window);

                // spawn thrust dust
                if (s->hasThrust())
                    spawnDust(
                        2,
                        s->getPosition().x,
                        s->getPosition().y,
                        s->getRotation() + 180 - 10.f,
                        s->getRotation() + 180 + 10.f,
                        1.f, 4.f,
                        5, 7);

                if (s->getActor() == Actor::PLAYER)
                {
                    if (upArrow.isDown)
                        s->thrust();
                    if (upArrow.isUp)
                        s->reverseThrust();
                    if (leftArrow.isDown)
                        s->turnLeft();
                    if (rightArrow.isDown)
                        s->turnRight();
                    if (spacebar.isDown &&
                        elapsed - state.lastPlayerFireMS > settings.PlayerRateOfFire)
                    {
                        state.lastPlayerFireMS = elapsed;
                        spacecraftFire(*s, s->getRotation());
                    }
                }
                else
                {
                    s->thrust();
                    if (elapsed -
                            state.lastEnemyFireMS >
                        settings.EnemyRateOfFire)
                    {
                        state.lastEnemyFireMS = elapsed;
                        spacecraftFire(*s, randFloat(0.f, 360.f));
                    }
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
                auto p = i->get();
                p->update();

                if (p->getActor() == Actor::PLAYER)
                    keepInWindow(*p, window);

                if (p->isBeyondRange())
                {
                    i = projectiles.erase(i);
                    if (i == projectiles.end())
                        break;
                }
            }

            // update effects
            for (auto i = effects.begin(); i != effects.end(); i++)
            {
                auto e = i->get();
                e->update();
                if (e->getFule() <= 0)
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

                        if (p->getActor() == Actor::PLAYER &&
                            a->getHP() <= 0 &&
                            a->getMaxHP() == 1)
                        {
                            state.score += 10;
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

            // check projectile collision
            for (auto i = projectiles.begin(); i != projectiles.end(); i++)
            {
                auto p = i->get();
                bool hasCollision = false;

                for (auto j = spacecrafts.begin(); j != spacecrafts.end(); j++)
                {
                    auto s = j->get();
                    if (s->getActor() == p->getActor())
                        continue;
                    if (checkCollision(*s, *p))
                    {
                        spawnDust(
                            10,
                            p->getPosition().x,
                            p->getPosition().y,
                            0.f, 360.f,
                            1.f, 2.f,
                            5, 7);

                        if (s->getActor() == Actor::PLAYER)
                        {
                            state.hp--;
                            if (state.hp == 0)
                                gameover();
                            else
                            {

                                state.lastPlayerSpawnMS = elapsed;
                                state.hasPlayerSpawn = true;
                            }
                        }
                        else
                        {
                            state.score += 50;
                        }

                        spacecrafts.erase(j);
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
                auto a = i->get();

                for (auto j = spacecrafts.begin(); j != spacecrafts.end(); j++)
                {
                    auto s = j->get();
                    if (checkCollision(*s, *a))
                    {
                        // TODO: play crash sound

                        spawnDust(
                            20,
                            s->getPosition().x,
                            s->getPosition().y,
                            0.f, 360.f,
                            1.f, 2.f,
                            3, 5);

                        if (s->getActor() == Actor::PLAYER)
                        {
                            state.hp--;
                            if (state.hp == 0)
                                gameover();
                            else
                            {
                                state.lastPlayerSpawnMS = elapsed;
                                state.hasPlayerSpawn = true;
                            }
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
        }

        //=========== draw ==========//
        window.clear();

        for (auto i = effects.begin(); i != effects.end(); i++)
            window.draw(*i->get());

        for (auto i = projectiles.begin(); i != projectiles.end(); i++)
            window.draw(*i->get());

        for (auto i = asteroids.begin(); i != asteroids.end(); i++)
            window.draw(*i->get());

        for (auto i = spacecrafts.begin(); i != spacecrafts.end(); i++)
            window.draw(*i->get());

        if (state.pause)
        {
            window.draw(titleText);
            window.draw(pauseText);
        }
        if (state.showHelp)
        {
            window.draw(helpText);
        }
        if (state.gameover)
        {
            window.draw(titleText);
            window.draw(menuText);
            if (state.score > 0)
            {
                snprintf(highscore, 20, "high score: %d", state.score);
                highscoreText.setString(highscore);
                highscoreText.setPosition(
                    window.getSize().x / 2 - highscoreText.getGlobalBounds().width / 2,
                    window.getSize().y / 2 - highscoreText.getGlobalBounds().height / 2 + 100);
                window.draw(highscoreText);
            }
        }
        else
        {
            window.draw(hpText);
            window.draw(scoreText);
        }

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