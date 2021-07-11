#ifndef PADDLE_H_INCLUDE
#define PADDLE_H_INCLUDE

#include "Entity.h"

class Paddle : public Entity
{
public:
    Paddle();
    ~Paddle();
    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif