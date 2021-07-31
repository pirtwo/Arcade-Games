#ifndef PROJECTILE_H_INCLUDE
#define PROJECTILE_H_INCLUDE

#include "Entity.h"

class Projectile : public Entity
{
public:
    sf::Vector2f vel;
    float speed;
    float angle;
    float range;
    float traveledDist;

public:
    Projectile(
        sf::Texture &texture,
        float speed,
        float angle,
        float range);

    ~Projectile();

    void update() override;

    void handleCollisions() override;
};

#endif