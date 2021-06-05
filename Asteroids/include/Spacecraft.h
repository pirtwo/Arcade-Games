#ifndef SHIP_H_INCLUDE
#define SHIP_H_INCLUDE

#include <SFML/Graphics.hpp>

class Spacecraft : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Sprite _sp;
    sf::Vector2f _velocity;
    sf::Vector2f _engRev;
    sf::Vector2f _engThr;
    float _maxSpeed;
    float _turnRate;
    float _friction;
    float _acceleration;
    float _hasThrust;

public:
    Spacecraft(
        sf::Texture &texture,
        float maxSpeed,
        float turnRate,
        float friction,
        float acceleration);
    ~Spacecraft();
    bool hasThrust();
    void update();
    void thrust();
    void reverseThrust();
    void turnLeft();
    void turnRight();
    void draw(
        sf::RenderTarget &target,
        sf::RenderStates states) const;
};

#endif