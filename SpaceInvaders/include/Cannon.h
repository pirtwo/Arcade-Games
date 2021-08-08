#ifndef CANNON_H_INCLUDE
#define CANNON_H_INCLUDE

#include "Entity.h"

class Cannon : public Entity
{
public:
    Cannon();

    ~Cannon();

    bool hasDispose() const override;

    void handleCollisions() override;

    void update() override;
};

#endif