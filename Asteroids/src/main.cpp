#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <map>
#include <vector>
#include <memory>
#include "Entity.h"
#include "Asteroid.h"
#include "Spacecraft.h"
#include "Particle.h"
#include "Projectile.h"
#include "Keyboard.h"
#include "Helper.h"

using namespace std;

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

    vector<shared_ptr<Entity>> world;
    vector<shared_ptr<Emitter>> effects;

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

    auto spawnEnemy =
        [&](float x, float y)
    {
        auto elm = shared_ptr<Spacecraft>(
            new Spacecraft(*textures["enemyShip"], 5.0, 4.5, 0.99, 0.1));
        elm->setScale(sf::Vector2f(0.5, 0.5));
        elm->setPosition(sf::Vector2f(x, y));
        world.push_back(elm);
    };

    auto spacecraftFire =
        [&](float x, float y, float angle, sf::Texture &t)
    {
        auto elm = shared_ptr<Projectile>(new Projectile(t, 7.5, angle, 500));
        elm->setPosition(x, y);
        elm->setRotation(angle);
        elm->setScale(0.5, 0.5);
        world.push_back(elm);
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
                for (auto i = world.begin(); i != world.end(); i++)
                {
                    if (mask.intersects(i->get()->getBounds()))
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
        auto elm = shared_ptr<Asteroid>(new Asteroid(
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
        world.push_back(elm);
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
        auto elm = shared_ptr<Emitter>(new Emitter(
            x, y, minAngle, maxAngle, minSpeed, maxSpeed, minFade, maxFade));
        elm->addFuel(num);
        effects.push_back(elm);
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

        for (auto i = world.begin(); i != world.end(); i++)
        {
            auto elm = i->get();

            if (elm->hp == 0)
            {
                if (elm->name == "player")
                {
                }
                else if (elm->name == "enemy")
                {
                }
                else if (elm->name == "asteroid")
                {
                }
                else if (elm->name == "projectile")
                {
                }

                i = world.erase(i);
                if (i == world.end())
                    break;
                else
                    continue;
            }

            elm->update();
            keepInWindow(*elm, window);
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

        // check collision
        for (auto i = world.begin(); i != world.end(); i++)
        {
            for (auto j = world.begin(); j != world.end(); j++)
            {
                auto elmA = i->get();
                auto elmB = i->get();
                if (i == j ||
                    (elmA->name == "asteroid" && elmA->name == "asteroid"))
                    continue;

                if (elmA->getBounds().intersects(elmB->getBounds()))
                {
                    elmA->hp--;
                    elmB->hp--;
                    spawnDust(
                        10, elmA->getPosition().x, elmA->getPosition().y,
                        0.f, 360.f, 1.f, 2.f, 5, 7);
                }
            }
        }

        //=========== draw ==========//
        window.clear();

        for (auto i = world.begin(); i != world.end(); i++)
            window.draw(*i->get());

        for (auto i = effects.begin(); i != effects.end(); i++)
            window.draw(*i->get());

        window.draw(hpText);
        window.draw(scoreText);

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
