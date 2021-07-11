#ifndef COLLISION_H_INCLUDE
#define COLLISION_H_INCLUDE

#include <SFML/Graphics.hpp>

/*
    returns true if ray intersects with box. 
*/
bool rayVsBox(
    sf::Vector2f rayOri,
    sf::Vector2f rayDir,
    sf::Vector2f boxPos,
    sf::Vector2f boxSize,
    sf::Vector2f &contactPoint,
    sf::Vector2f &contactNormal);

#endif