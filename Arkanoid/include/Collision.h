#ifndef COLLISION_H_INCLUDE
#define COLLISION_H_INCLUDE

#include <SFML/Graphics.hpp>

/*
    axis aligned bounding box collision detection
*/
bool AABB(
    float ax,
    float ay,
    float aWidth,
    float aHeight,
    float bx,
    float by,
    float bWidth,
    float bHeight);

/*
    returns the result of collision of the ray against 
    the sides of the box (result order: top, left, bottom, right)
*/
std::vector<std::pair<bool, sf::Vector2f>> rcCollision(
    sf::Vector2f rayA,
    sf::Vector2f rayB,
    sf::Vector2f boxPos,
    sf::Vector2f boxDim);

/*
    line segment intersection detection, returns <true, collision point> 
    if two line segment intersects
*/
std::pair<bool, sf::Vector2f> lineSegCollision(
    sf::Vector2f a,
    sf::Vector2f b,
    sf::Vector2f c,
    sf::Vector2f d);

#endif