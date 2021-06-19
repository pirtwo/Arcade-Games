#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 500), "Pinball");
    window.setFramerateLimit(60);

    //===== load assets ===//

    //===== initialize ====//

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        //====== update ====//

        //====== draw ======//
        window.clear();

        window.display();
    }

    return EXIT_SUCCESS;
}