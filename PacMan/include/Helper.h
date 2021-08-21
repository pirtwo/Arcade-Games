#ifndef HELPER_H_INCLUDE
#define HELPER_H_INCLUDE

#include <SFML/Graphics.hpp>

int randInt(int m, int n);
float randFloat(float m, float n);
float calcDistance(sf::Vector2f a, sf::Vector2f b);
float degreeToRadian(float degree);

#endif