#include "Spacecraft.h"
#include "Particle.h"
#include "Helper.h"
#include <math.h>

Spacecraft::Spacecraft(
    sf::Texture &texture,
    float maxSpeed,
    float turnRate,
    float friction,
    float acceleration)
{
    this->hp = 1;
    this->name = "spacecraft";

    _maxSpeed = maxSpeed;
    _turnRate = turnRate;
    _friction = friction;
    _acceleration = acceleration;
    _hasThrust = false;
    _exhaust = Emitter(
        getPosition().x,
        getPosition().y,
        getRotation() + 180 - 10.f,
        getRotation() + 180 + 10.f,
        1.f, 4.f, 5, 7);

    _velocity =
        sf::Vector2f(0, 0);
    _engThr =
        sf::Vector2f(0, 0);
    _engRev =
        sf::Vector2f(1, 1);

    _sp = sf::Sprite(texture);
    _sp.setOrigin(
        _sp.getLocalBounds().width / 2,
        _sp.getLocalBounds().height / 2);
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
    _engThr.x = cos(degreeToRadian(getRotation())) * _acceleration;
    _engThr.y = sin(degreeToRadian(getRotation())) * _acceleration;
    _engRev.x = 1;
    _engRev.y = 1;
    _hasThrust = true;
    _exhaust.addFuel(2);
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

    _exhaust.update();
    _exhaust.setPosition(getPosition());
    _exhaust.setMinAngle(getRotation() + 180 - 10.f);
    _exhaust.setMaxAngle(getRotation() + 180 + 10.f);
}

void Spacecraft::handleCollisions()
{
    for (auto &&i : collisions)
    {
        auto elm = i.lock();
        if (!elm)
            continue;

        hp--;
    }

    collisions.clear();
}

void Spacecraft::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_exhaust);
    target.draw(_sp, states.transform * getTransform());
}
