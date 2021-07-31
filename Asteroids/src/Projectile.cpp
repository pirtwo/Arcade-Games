#include "Projectile.h"
#include "Helper.h"
#include <math.h>

Projectile::Projectile(
    sf::Texture &texture,
    float speed,
    float angle,
    float range)
{
    this->hp = 1;
    this->name = "projectile";

    this->speed = speed;
    this->angle = angle;
    this->range = range;
    this->vel = sf::Vector2f(
        cos(degreeToRadian(this->angle)) * this->speed,
        sin(degreeToRadian(this->angle)) * this->speed);
    this->traveledDist = 0;
    _sp = sf::Sprite(texture);
    _sp.setOrigin(
        _sp.getLocalBounds().width / 2,
        _sp.getLocalBounds().height / 2);
}

Projectile::~Projectile()
{
    //
}

void Projectile::update()
{
    traveledDist += speed;
    if (traveledDist >= range)
        hp = 0;
    move(vel);
}

void Projectile::handleCollisions()
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
