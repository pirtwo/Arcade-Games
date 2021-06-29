#ifndef COLLISION_H_INCLUDE
#define COLLISION_H_INCLUDE

#include <SFML/Graphics.hpp>

bool AABB(
    float ax,
    float ay,
    float aWidth,
    float aHeight,
    float bx,
    float by,
    float bWidth,
    float bHeight);

std::vector<sf::Vector2f> rcGetCollision(
    sf::Vector2f rayA,
    sf::Vector2f rayB,
    sf::Vector2f boxPos,
    sf::Vector2f boxDim);

std::pair<bool, sf::Vector2f> lineSegCollision(
    sf::Vector2f a,
    sf::Vector2f b,
    sf::Vector2f c,
    sf::Vector2f d);

#endif