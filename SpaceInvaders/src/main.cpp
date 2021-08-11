#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Key.h"
#include "Atlas.h"
#include "Helper.h"
#include "Loader.h"
#include "Entity.h"
#include "Cannon.h"
#include "Alien.h"
#include "Projectile.h"

struct State
{
    int lastAlienMS = 0;
    int lastFireMS = 0;
} state;

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 700), "Space Invaders");
    window.setFramerateLimit(60);

    //====== load assets ======//
    auto atlas = std::make_shared<Atlas>();
    if (!atlas->texture.loadFromFile("./assets/texture.png") ||
        !loadAtlasData("./assets/texture.data", atlas->data))
    {
        return EXIT_FAILURE;
    }

    //====== initialize ======//
    sf::Clock clock;
    int elapsedMS = 0;

    sf::FloatRect bounds(
        sf::Vector2f(0, 0),
        sf::Vector2f(window.getSize().x, window.getSize().y));

    Key leftArrow, rightArrow, spacebar;

    std::vector<std::shared_ptr<Entity>> world;

    auto cannon = std::make_shared<Cannon>("cannon.png", *atlas);
    cannon->owner = "player";
    cannon->speed = 3.5;
    cannon->setScale(0.2, 0.2);
    cannon->setPosition(
        window.getSize().x / 2,
        window.getSize().y - 50);
    world.push_back(cannon);

    auto spawnAlien = [&]()
    {
        char key[50];
        snprintf(key, 50, "alien_%02d.png", randInt(1, 8));

        auto elm = std::make_shared<Alien>(key, *atlas);
        elm->owner = "cpu";
        elm->hp = 1;
        elm->vel.y = randFloat(1, 2);
        elm->setScale(0.2, 0.2);
        elm->setPosition(randFloat(50, window.getSize().x - 50), 0);
        world.push_back(elm);
    };

    auto spawnProjectile = [&](std::string owner, sf::Vector2f pos)
    {
        auto elm =
            owner == "player"
                ? std::make_shared<Projectile>("cannon_projectile.png", *atlas)
                : std::make_shared<Projectile>("alien_projectile.png", *atlas);
        elm->owner = owner;
        elm->vel.y = owner == "player" ? -2 : 2;
        elm->scale(0.2, 0.2);
        elm->setPosition(pos);
        world.push_back(elm);
    };

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::KeyPressed:
                if (e.key.code == sf::Keyboard::Key::Left)
                    leftArrow.onPress();
                if (e.key.code == sf::Keyboard::Key::Right)
                    rightArrow.onPress();
                if (e.key.code == sf::Keyboard::Key::Space)
                    spacebar.onPress();
                break;
            case sf::Event::KeyReleased:
                if (e.key.code == sf::Keyboard::Key::Left)
                    leftArrow.onRelease();
                if (e.key.code == sf::Keyboard::Key::Right)
                    rightArrow.onRelease();
                if (e.key.code == sf::Keyboard::Key::Space)
                    spacebar.onRelease();
                break;
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        //====== update =====//

        elapsedMS = clock.getElapsedTime().asMilliseconds();

        if (leftArrow.isDown)
            cannon->moveLeft();
        if (rightArrow.isDown)
            cannon->moveRight();
        if (!leftArrow.isDown && !rightArrow.isDown)
            cannon->stop();

        if (spacebar.isDown && elapsedMS - state.lastFireMS > 300)
        {
            spawnProjectile("player", cannon->getPosition());
            state.lastFireMS = elapsedMS;
        }

        if (elapsedMS - state.lastAlienMS > 500)
        {
            spawnAlien();
            state.lastAlienMS = elapsedMS;
        }

        for (auto &&i : world)
            i->update();

        for (auto i = world.begin(); i != world.end(); i++)
        {
            if (i->get()->hasDispose() || !bounds.contains(i->get()->getPosition()))
            {
                i = world.erase(i);
                if (i == world.end())
                    break;
            }
        }

        //====== draw =======//
        window.clear();

        for (auto &&i : world)
            window.draw(*i);

        window.display();
    }

    return EXIT_SUCCESS;
}