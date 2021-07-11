#ifndef BALL_H_INCLUDE
#define BALL_H_INCLUDE

#include "Entity.h"

class Ball : public Entity
{
protected:
    sf::Sprite _sp;
    sf::Vector2f _vel;

public:
    Ball(sf::Texture &t);
    ~Ball();
    sf::Vector2f getVelocity() override;
    sf::FloatRect getBounds() override;
    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif