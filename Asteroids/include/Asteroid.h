#ifndef ASTEROID_H_INCLUDE
#define ASTEROID_H_INCLUDE

#include "Entity.h"

class Asteroid : public Entity
{
public:
    float speed;
    float angle;
    sf::Vector2f vel;

public:
    Asteroid(
        sf::Texture &texture,
        int hp,
        float speed,
        float angle);

    ~Asteroid();

    void update() override;

    void handleCollisions() override;
};

#endif