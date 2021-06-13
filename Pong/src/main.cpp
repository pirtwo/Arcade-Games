#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Line.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 500), "Pong");
    window.setFramerateLimit(60);

    //========== load assets ===========//

    //========== initialize ===========//
    int lanethickness = 10;
    auto toplane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(0, 0),
        sf::Vector2f(window.getSize().x, 0),
        lanethickness, sf::Color::Red));

    auto midlane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(window.getSize().x / 2 - lanethickness / 2.f, 0),
        sf::Vector2f(window.getSize().x / 2 - lanethickness / 2.f, window.getSize().y),
        15, 35.f, lanethickness, sf::Color::Red));

    auto botlane = std::unique_ptr<Line>(new Line(
        sf::Vector2f(0, window.getSize().y - lanethickness),
        sf::Vector2f(window.getSize().x, window.getSize().y - lanethickness),
        lanethickness, sf::Color::Red));

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        //========== update ===========//
        //========== draw   ===========//
        window.clear();
        window.draw(*toplane);
        window.draw(*midlane);
        window.draw(*botlane);
        window.display();
    }

    return EXIT_SUCCESS;
}