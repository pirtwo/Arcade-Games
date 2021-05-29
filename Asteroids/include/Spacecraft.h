#ifndef SHIP_H_INCLUDE
#define SHIP_H_INCLUDE

#include <SFML/Graphics.hpp>

class Spacecraft : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Sprite _sp;
    float _turnSpeed;
    sf::Vector2f _velocity;
    sf::Vector2f _acceleration;
    sf::Vector2f _friction;

public:
    float turnRate;
    float maxSpeed;

public:
    Spacecraft(sf::Texture &texture);
    virtual ~Spacecraft();
    virtual void update();
    virtual void forward();
    virtual void stop();
    virtual void turnLeft();
    virtual void turnRight();
    virtual void draw(
        sf::RenderTarget &target,
        sf::RenderStates states) const;
};

#endif