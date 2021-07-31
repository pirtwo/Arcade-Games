#include "Asteroid.h"
#include "Helper.h"
#include <math.h>

Asteroid::Asteroid(sf::Texture &texture, int hp, float speed, float angle)
{
    this->hp = hp;
    this->name = "asteroid";

    this->speed = speed;
    this->angle = angle;
    this->vel = sf::Vector2f(
        cos(degreeToRadian(this->angle)) * this->speed,
        sin(degreeToRadian(this->angle)) * this->speed);
    _sp = sf::Sprite(texture);
    _sp.setOrigin(
        _sp.getLocalBounds().width / 2,
        _sp.getLocalBounds().height / 2);
}

Asteroid::~Asteroid()
{
    //
}

void Asteroid::update()
{
    move(vel);
}

void Asteroid::handleCollisions()
{
    for (auto &&i : collisions)
    {
        auto elm = i.lock();
        if (!elm)
            continue;

        if (elm->name == "projectile" || elm->name == "spacecraft")
        {
            hp--;
        }
    }

    collisions.clear();
}
