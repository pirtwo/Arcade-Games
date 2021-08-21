#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>

constexpr float PI = 3.14159;

int randInt(int m, int n)
{
    return m + rand() % (n - m + 1);
}

float randFloat(float m, float n)
{
    return m + (n - m) * (rand() / (RAND_MAX + 1.f));
}

float calcDistance(sf::Vector2f a, sf::Vector2f b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float degreeToRadian(float degree)
{
    return degree * PI / 180;
}
