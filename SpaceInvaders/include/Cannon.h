#ifndef CANNON_H_INCLUDE
#define CANNON_H_INCLUDE

#include "Entity.h"
class Atlas;

class Cannon : public Entity
{
public:
    int hp;
    float speed;
    sf::Vector2f vel;

public:
    Cannon(const char *texture, Atlas &atlas);

    ~Cannon();

    void moveLeft();

    void moveRight();

    void stop();

    bool hasDispose() const override;

    void handleCollisions() override;

    void update() override;
};

#endif