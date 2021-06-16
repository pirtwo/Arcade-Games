#ifndef KEYBOARD_H_INCLUDE
#define KEYBOARD_H_INCLUDE

#include <SFML/Graphics.hpp>

struct Config
{
    int laneThickness = 10;
    sf::Color laneColor = sf::Color(155, 155, 155, 255);
    float ballSize = 17.f;
    float ballMinSpeed = 5.f;
    float ballMaxSpeed = 11.5f;
    sf::Color ballColor = sf::Color::Red;
    float paddleSpeed = 8.5;
    float paddleWid = 12.f;
    float paddleHig = 85.f;
    float paddleMargin = 20.f;
    sf::Color paddleColor = sf::Color(216, 216, 216, 255);
    sf::Color scoreColor = sf::Color(155, 155, 155, 255);
};

#endif