#include "Spacecraft.h"
#include <SFML/Graphics.hpp>
#include <math.h>

const float PI = 3.14;

Spacecraft::Spacecraft(
    sf::Texture &texture,
    float maxSpeed,
    float turnRate,
    float friction,
    float acceleration) : sf::Sprite()
{
    _maxSpeed = maxSpeed;
    _turnRate = turnRate;
    _friction = friction;
    _acceleration = acceleration;
    _hasThrust = false;

    _velocity =
        sf::Vector2f(0, 0);
    _engThr =
        sf::Vector2f(0, 0);
    _engRev =
        sf::Vector2f(1, 1);

    setTexture(texture);
    setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
}

Spacecraft::~Spacecraft()
{
    //
}

bool Spacecraft::hasThrust()
{
    return _hasThrust;
}

void Spacecraft::thrust()
{
    _engThr.x = cos(getRotation() * PI / 180) * _acceleration;
    _engThr.y = sin(getRotation() * PI / 180) * _acceleration;
    _engRev.x = 1;
    _engRev.y = 1;
    _hasThrust = true;
}

void Spacecraft::reverseThrust()
{
    _engThr.x = 0;
    _engThr.y = 0;
    _engRev.x = _friction;
    _engRev.y = _friction;
    _hasThrust = false;
}

void Spacecraft::turnLeft()
{
    rotate(_turnRate * -1);
}

void Spacecraft::turnRight()
{
    rotate(_turnRate);
}

void Spacecraft::update()
{
    _velocity.x += _engThr.x;
    _velocity.y += _engThr.y;

    // control ship speed
    float m = sqrt(pow(_velocity.x, 2) + pow(_velocity.y, 2));
    if (m > _maxSpeed)
    {
        float angle = atan2(_velocity.y, _velocity.x);
        _velocity.x = cos(angle) * _maxSpeed;
        _velocity.y = sin(angle) * _maxSpeed;
    }

    _velocity.x *= _engRev.x;
    _velocity.y *= _engRev.y;

    move(_velocity.x, _velocity.y);
}
