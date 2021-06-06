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
    setTexture(texture);
}

Projectile::~Projectile()
{
    std::cout << "~Projectile\r\n";
}

bool Projectile::isBeyondRange()
{
    return _traveledDist > _range;
}

int Projectile::getActor()
{
    return _actor;
}

void Projectile::setActor(int actor)
{
    _actor = actor;
}

void Projectile::update()
{
    _traveledDist += _speed;
    move(_velocity);
}
