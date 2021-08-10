#include "Cannon.h"
#include "Atlas.h"

Cannon::Cannon(const char *texture, Atlas &atlas)
{
    this->name = "cannon";
    this->owner = "";
    this->hp = 1;

    _sp = sf::Sprite(atlas.texture);
    _sp.setTextureRect(atlas.data[texture]);
}

Cannon::~Cannon()
{
    //
}

void Cannon::moveLeft()
{
    vel.x = vel.x <= 0 ? vel.x : vel.x * -1;
}

void Cannon::moveRight()
{
    vel.x = vel.x > 0 ? vel.x : vel.x * -1;
}

bool Cannon::hasDispose() const
{
    return false;
}

void Cannon::handleCollisions()
{
    //
}

void Cannon::update()
{
    move(vel.x, 0);
}
