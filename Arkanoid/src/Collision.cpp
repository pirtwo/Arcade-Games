#include "Collision.h"
#include <SFML/Graphics.hpp>

bool AABB(
    sf::Vector2f aPos, sf::Vector2f aDim,
    sf::Vector2f bPos, sf::Vector2f bDim)
{
    return ((aPos.x < bPos.x && bPos.x < aPos.x + aDim.x) ||
            (aPos.x > bPos.x && aPos.x < bPos.x + bDim.x)) &&
           ((aPos.y < bPos.y && bPos.y < aPos.y + aDim.y) ||
            (aPos.y > bPos.y && aPos.y < bPos.y + bDim.y));
}

std::vector<std::pair<bool, sf::Vector2f>> rcCollision(
    sf::Vector2f rayA,
    sf::Vector2f rayB,
    sf::Vector2f boxPos,
    sf::Vector2f boxDim)
{
    std::vector<std::pair<bool, sf::Vector2f>> collisions;
    std::pair<bool, sf::Vector2f> res;

    // rect top
    res = lineSegCollision(
        rayA, rayB,
        sf::Vector2f(boxPos.x, boxPos.y),
        sf::Vector2f(boxPos.x + boxDim.x, boxPos.y));
    collisions.push_back(res);

    // rect left
    res = lineSegCollision(
        rayA, rayB,
        sf::Vector2f(boxPos.x, boxPos.y),
        sf::Vector2f(boxPos.x, boxPos.y + boxDim.y));
    collisions.push_back(res);

    // rect bottom
    res = lineSegCollision(
        rayA, rayB,
        sf::Vector2f(boxPos.x, boxPos.y + boxDim.y),
        sf::Vector2f(boxPos.x + boxDim.x, boxPos.y + boxDim.y));
    collisions.push_back(res);

    // rect right
    res = lineSegCollision(
        rayA, rayB,
        sf::Vector2f(boxPos.x + boxDim.x, boxPos.y),
        sf::Vector2f(boxPos.x + boxDim.x, boxPos.y + boxDim.y));
    collisions.push_back(res);

    return collisions;
}

std::pair<bool, sf::Vector2f> lineSegCollision(
    sf::Vector2f a,
    sf::Vector2f b,
    sf::Vector2f c,
    sf::Vector2f d)
{
    float t = ((a.x - c.x) * (c.y - d.y) - (a.y - c.y) * (c.x - d.x)) /
              ((a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x));

    float u = ((b.x - a.x) * (a.y - c.y) - (b.y - a.y) * (a.x - c.x)) /
              ((a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x));

    return std::pair<bool, sf::Vector2f>{
        t >= 0 && t <= 1 && u >= 0 && u <= 1,
        sf::Vector2f{a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)}};
}
