#ifndef PROJECTILE_H_INCLUDE
#define PROJECTILE_H_INCLUDE

#include "Entity.h"
class Atlas;

class Projectile : public Entity
{
public:
    int hp;
    float range;
    sf::Vector2f vel;

public:
    Projectile(const char *texture, Atlas &atlas);

    ~Projectile();

    bool hasDispose() const override;

    void handleCollisions() override;

    void update() override;
};

#endif