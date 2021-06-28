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

#endif