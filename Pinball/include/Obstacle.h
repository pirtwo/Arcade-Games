#ifndef OBSTACLE_H_INCLUDE
#define OBSTACLE_H_INCLUDE

#include <SFML/Graphics.hpp>

class Ball;

class Obstacle : public sf::Drawable, public sf::Transformable
{
public:
    virtual ~Obstacle() {}
    virtual sf::FloatRect bounds() const = 0;
    virtual void onCollision(Ball &ball) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

#endif