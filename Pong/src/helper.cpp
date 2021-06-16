#include "helper.h"
#include "Paddle.h"

void limitPaddleMove(Paddle &paddle, float minY, float maxY)
{
    if (paddle.getPosition().y < minY)
        paddle.setPosition(paddle.getPosition().x, minY);

    if (paddle.getPosition().y > maxY)
        paddle.setPosition(paddle.getPosition().x, maxY);
}

bool checkCollision(sf::FloatRect a, sf::FloatRect b)
{
    bool overlapX = false;
    bool overlapY = false;

    if (a.left < b.left)
        overlapX = b.left - a.left < a.width;
    else
        overlapX = a.left - b.left < b.width;

    if (a.top < b.top)
        overlapY = b.top - a.top < a.height;
    else
        overlapY = a.top - b.top < b.height;

    return overlapX && overlapY;
}

float randFloat(float n, float m)
{
    return n + (m - n) * (rand() / (RAND_MAX + 1.f));
}