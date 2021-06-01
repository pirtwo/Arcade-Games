#include "Projectile.h"
#include "Helper.h"
#include <iostream>
#include <math.h>

Projectile::Projectile(
    sf::Texture &texture,
    float speed,
    float angle,
    float range)
{
    _speed = speed;
    _angle = angle;
    _range = range;
    _velocity.x = cos(degreeToRadian(_angle)) * speed;
    _velocity.y = sin(degreeToRadian(_angle)) * speed;
    _traveledDist = 0;
    _sp = sf::Sprite(texture);
}

Projectile::~Projectile()
{
    std::cout << "~Projectile\r\n";
}

bool Projectile::isBeyondRange()
{
    return _traveledDist > _range;
}

void Projectile::update()
{
    _traveledDist += _speed;
    move(_velocity);
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, getTransform());
}