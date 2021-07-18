#ifndef BALL_H_INCLUDE
#define BALL_H_INCLUDE

#include "Entity.h"
class Atlas;

class Ball : public Entity
{
protected:
    sf::Sprite _sp;

public:
    sf::Vector2f vel;

public:
    Ball(Atlas &a);
    ~Ball();
    sf::FloatRect getBounds() override;
    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif