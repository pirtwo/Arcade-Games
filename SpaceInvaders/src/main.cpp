#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Atlas.h"
#include "Loader.h"
#include "Entity.h"
#include "Alien.h"
#include "Helper.h"

struct State
{
    int lastAlienMS = 0;
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

    std::vector<std::shared_ptr<Entity>> world;

    auto spawnAlien = [&]()
    {
        char key[50];
        snprintf(key, 50, "alien_%02d.png", randInt(1, 8));

        auto elm = std::make_shared<Alien>(key, *atlas);
        elm->owner = "cpu";
        elm->hp = 1;
        elm->vel.y = randFloat(1, 2);
        elm->setScale(0.2, 0.2);
        elm->setPosition(randFloat(0, window.getSize().x - 50), 0);
        world.push_back(elm);
    };

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

        //====== update =====//

        elapsedMS = clock.getElapsedTime().asMilliseconds();

        if (elapsedMS - state.lastAlienMS > 500)
        {
            spawnAlien();
            state.lastAlienMS = elapsedMS;
        }

        for (auto &&i : world)
            i->update();

        //====== draw =======//
        window.clear();

        for (auto &&i : world)
            window.draw(*i);

        window.display();
    }

    return EXIT_SUCCESS;
}