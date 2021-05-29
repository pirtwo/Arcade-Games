#include "Spacecraft.h"
#include <SFML/Graphics.hpp>
#include <math.h>

const float PI = 3.14;

Spacecraft::Spacecraft(sf::Texture &texture)
{
    turnRate = 4.5;
    maxSpeed = 4.0;

    _velocity =
        sf::Vector2f(0, 0);
    _acceleration =
        sf::Vector2f(0, 0);
    _friction =
        sf::Vector2f(1, 1);

    _sp = sf::Sprite(texture);
    _sp.setOrigin(sf::Vector2f(
        _sp.getGlobalBounds().width / 2,
        _sp.getGlobalBounds().height / 2));
}

Spacecraft::~Spacecraft()
{
    //
}

void Spacecraft::forward()
{
    _acceleration.x = 0.2;
    _acceleration.y = 0.2;
    _friction.x = 1;
    _friction.y = 1;
}

void Spacecraft::stop()
{
    _acceleration.x = 0;
    _acceleration.y = 0;
    _friction.x = 0.99;
    _friction.y = 0.99;
}

void Spacecraft::turnLeft()
{
    rotate(turnRate * -1);
}

void Spacecraft::turnRight()
{
    rotate(turnRate);
}

void Spacecraft::update()
{
    // calculate magnitude of the ship
    float m = sqrt(pow(_velocity.x, 2) + pow(_velocity.y, 2));

    // control ship max speed
    m = m >= maxSpeed ? maxSpeed : m;

    // apply ship rotation
    _velocity.x = m * cos(getRotation() * PI / 180);
    _velocity.y = m * sin(getRotation() * PI / 180);

    // apply acceleration and friction
    _velocity.x += _velocity.x > 0 ? _acceleration.x : -_acceleration.x;
    _velocity.y += _velocity.y > 0 ? _acceleration.y : -_acceleration.y;
    _velocity.x *= _friction.x;
    _velocity.y *= _friction.y;

    // move the ship
    move(_velocity.x, _velocity.y);
}

void Spacecraft::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, getTransform());
}