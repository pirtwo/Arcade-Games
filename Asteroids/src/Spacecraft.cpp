#include "Spacecraft.h"
#include <SFML/Graphics.hpp>
#include <math.h>

const float PI = 3.14;

Spacecraft::Spacecraft(sf::Texture &texture)
{
    _velocity =
        sf::Vector2f(0, 0);
    _acceleration =
        sf::Vector2f(1, 1);
    _friction =
        sf::Vector2f(1, 1);
    _turnSpeed = 0.f;

    _sp = sf::Sprite(texture);
}

Spacecraft::~Spacecraft()
{
    //
}

void Spacecraft::forward()
{
    _velocity.x = cos(getRotation() * PI / 180);
    _velocity.y = sin(getRotation() * PI / 180);
    _acceleration.x = 1.2;
    _acceleration.y = 1.2;
    _friction.x = 1.f;
    _friction.y = 1.f;
}

void Spacecraft::stop()
{
    _acceleration.x = 1.f;
    _acceleration.y = 1.f;
    _friction.x = 0.2;
    _friction.y = 0.2;
}

void Spacecraft::turnLeft()
{
    _turnSpeed = 5.f;
}

void Spacecraft::turnRight()
{
    _turnSpeed = -5.f;
}

void Spacecraft::update()
{
    _velocity.x *= _acceleration.x * _friction.x;
    _velocity.y *= _acceleration.y * _friction.y;
    rotate(_turnSpeed);
    move(_velocity.x, _velocity.y);
}

void Spacecraft::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, getTransform());
}