#ifndef ALIEN_H_INCLUDE
#define ALIEN_H_INCLUDE

#include "Entity.h"
class Atlas;

class Alien : public Entity
{
public:
    int hp;
    sf::Vector2f vel;

public:
    Alien(const char *texture, Atlas &atlas);

    ~Alien();

    bool hasDispose() const override;

    void handleCollisions() override;

    void update() override;
};

#endif