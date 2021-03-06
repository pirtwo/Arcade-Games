#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <map>
#include <vector>
#include <memory>
#include <math.h>
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
    const int EnemyRateOfFire = 600;
} settings;

struct State
{
    bool gameover = true;
    int score = 0;
    int lives = 3;
    int lastPlayerFireMS = 0;
    int lastEnemyFireMS = 0;
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

    auto playerSpacecraft = make_shared<Spacecraft>(
        *textures["playerShip"], 5.0, 4.5, 0.99, 0.1);
    playerSpacecraft->owner = "player";
    playerSpacecraft->setScale(sf::Vector2f(0.5, 0.5));
    auto enemySpacecraft = make_shared<Spacecraft>(
        *textures["enemyShip"], 3.0, 4.5, 0.99, 0.1);
    enemySpacecraft->hp = 0;
    enemySpacecraft->owner = "cpu";
    enemySpacecraft->setScale(sf::Vector2f(0.5, 0.5));

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
        "[n] new game\r\n[up] thurst\r\n[left] rotate left\r\n[right] rotate right\r\n[space] shoot");
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
        window.getSize().y / 2 - highscoreText.getGlobalBounds().height / 2 + 150);

    auto spacecraftFire =
        [&](std::string owner, float x, float y, float angle, sf::Texture &t)
    {
        auto elm = shared_ptr<Projectile>(new Projectile(t, 7.5, angle, 500));
        elm->owner = owner;
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
        [&](sf::Vector2f pos, float minAngle, float maxAngle, int hp)
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

        elm->setPosition(pos);
        elm->setRotation(randFloat(0.f, 360.f));
        world.push_back(elm);
    };

    auto spawnDust =
        [&](
            int num, sf::Vector2f pos,
            float minAngle, float maxAngle,
            float minSpeed, float maxSpeed,
            int minFade, int maxFade)
    {
        auto elm = shared_ptr<Emitter>(new Emitter(
            pos.x, pos.y, minAngle, maxAngle, minSpeed, maxSpeed, minFade, maxFade));
        elm->addFuel(num);
        effects.push_back(elm);
    };

    auto spawnEnemy =
        [&]()
    {
        int currEnemyNum = 0;
        for (auto &&i : world)
            if (i->name == "spacecraft" && i->owner == "cpu")
                currEnemyNum++;

        if (currEnemyNum == 0 &&
            state.score > 0 &&
            state.score % 200 == 0)
        {
            enemySpacecraft->hp = 1;
            enemySpacecraft->setRotation(randFloat(0, 1) > 0.5 ? 0 : 180);
            enemySpacecraft->setPosition(0, randFloat(100, window.getSize().y - 100));
            world.push_back(enemySpacecraft);
        }
    };

    auto spawnAsteroids =
        [&]()
    {
        int currAsterNum = 0;
        for (auto &&i : world)
            if (i->name == "asteroid")
                currAsterNum++;

        for (int i = 0; i < settings.MaxAstroidNum - currAsterNum; i++)
            spawnAsteroid(
                sf::Vector2f(randFloat(0, window.getSize().x), 0), -50, 360, randInt(1, 3));
    };

    auto newgame =
        [&]()
    {
        clock.restart();

        state.score = 0;
        state.lives = 3;
        state.gameover = false;

        playerSpacecraft->hp = 1;
        playerSpacecraft->setPosition(
            window.getSize().x / 2, window.getSize().y / 2);

        world.clear();
        world.push_back(playerSpacecraft);
    };

    auto respawn =
        [&]()
    {
        auto spawnArea = findEmptyArea(300, 300);
        playerSpacecraft->hp = 1;
        playerSpacecraft->setPosition(spawnArea.x + 150, spawnArea.y + 150);
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

        // update player spacecraft
        if (playerSpacecraft->hp > 0)
        {
            if (upArrow.isDown)
                playerSpacecraft->thrust();
            if (upArrow.isUp)
                playerSpacecraft->reverseThrust();
            if (leftArrow.isDown)
                playerSpacecraft->turnLeft();
            if (rightArrow.isDown)
                playerSpacecraft->turnRight();
            if (spacebar.isDown &&
                elapsed - state.lastPlayerFireMS > settings.PlayerRateOfFire)
            {
                state.lastPlayerFireMS = elapsed;
                spacecraftFire(
                    "player",
                    playerSpacecraft->getPosition().x,
                    playerSpacecraft->getPosition().y,
                    playerSpacecraft->getRotation(),
                    *textures["greenLaser"]);
            }
        }

        // update enemy spacecraft
        if (enemySpacecraft->hp > 0)
        {
            enemySpacecraft->thrust();
            if (elapsed - state.lastEnemyFireMS > settings.EnemyRateOfFire)
            {
                state.lastEnemyFireMS = elapsed;
                spacecraftFire(
                    "cpu",
                    enemySpacecraft->getPosition().x,
                    enemySpacecraft->getPosition().y,
                    randFloat(0, 360),
                    *textures["redLaser"]);
            }
        }

        // update world
        for (size_t i = 0; i < world.size(); i++)
        {
            auto elm = world[i].get();
            if (elm->hp == 0)
            {
                spawnDust(5, elm->getPosition(), 0, 360, 1.5, 2, 5, 7);

                if (elm->name == "asteroid")
                {
                    if (elm->maxHP == 1)
                    {
                        state.score += 10;
                    }
                    else if (elm->maxHP == 2)
                    {
                        spawnAsteroid(elm->getPosition(), 0.f, 360.f, 1);
                        spawnAsteroid(elm->getPosition(), 0.f, 360.f, 1);
                        spawnAsteroid(elm->getPosition(), 0.f, 360.f, 1);
                    }
                    else if (elm->maxHP == 3)
                    {
                        spawnAsteroid(elm->getPosition(), 0.f, 360.f, 2);
                        spawnAsteroid(elm->getPosition(), 0.f, 360.f, 2);
                    }
                }
                if (elm->name == "spacecraft" && elm->owner == "player")
                {
                    state.lives--;
                    if (state.lives != 0)
                        respawn();
                    else
                        state.gameover = true;
                }
                if (elm->name == "spacecraft" && elm->owner == "cpu")
                    state.score += 50;
            }
            else
            {
                elm->update();
                keepInWindow(*elm, window);
            }
        }

        // remove destroyed items
        for (auto i = world.begin(); i != world.end(); i++)
        {
            if (i->get()->hp == 0)
            {
                i = world.erase(i);
                if (i == world.end())
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

        // check collision
        for (size_t i = 0; i < world.size(); i++)
        {
            for (size_t j = 0; j < world.size(); j++)
            {
                if (i == j)
                    continue;
                if (world[i]->getBounds().intersects(world[j]->getBounds()))
                    world[i]->collisions.push_back(world[j]);
            }
        }

        // handle collisions
        for (auto &&i : world)
        {
            i->handleCollisions();
        }

        // update score
        snprintf(playerScore, 20, "score: %d", state.score);
        scoreText.setString(playerScore);

        // update player HP
        snprintf(
            playerHP, 20,
            "hp: %s%s%s",
            state.lives == 1 ? "|" : "",
            state.lives == 2 ? "||" : "",
            state.lives == 3 ? "|||" : "");
        hpText.setString(playerHP);

        spawnEnemy();
        spawnAsteroids();

        //=========== draw ==========//
        window.clear();

        for (auto i = world.begin(); i != world.end(); i++)
            window.draw(*i->get());

        for (auto i = effects.begin(); i != effects.end(); i++)
            window.draw(*i->get());

        if (state.gameover)
        {
            window.draw(titleText);
            window.draw(menuText);
            if (state.score >= 0)
            {
                snprintf(highscore, 20, "high score: %d", state.score);
                highscoreText.setString(highscore);
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
