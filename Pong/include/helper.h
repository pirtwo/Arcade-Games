#ifndef HELPER_H_INCLUDE
#define HELPER_H_INCLUDE

#include <SFML/Graphics.hpp>
class Paddle;

float randFloat(float n, float m);
void limitPaddleMove(Paddle &paddle, float minY, float maxY);
bool checkCollision(sf::FloatRect a, sf::FloatRect b);

#endif