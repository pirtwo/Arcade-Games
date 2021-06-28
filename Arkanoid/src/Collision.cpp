#include "Collision.h"
#include <SFML/Graphics.hpp>

bool AABB(
    float ax,
    float ay,
    float aWidth,
    float aHeight,
    float bx,
    float by,
    float bWidth,
    float bHeight)
{
    return ((ax < bx && bx < ax + aWidth) ||
            (ax > bx && ax < bx + bWidth)) &&
           ((ay < by && by < ay + aHeight) ||
            (ay > by && ay < by + bHeight));
}