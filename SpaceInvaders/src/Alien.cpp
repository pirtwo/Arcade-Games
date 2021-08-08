#include "Alien.h"
#include "Atlas.h"

Alien::Alien(const char *texture, Atlas &atlas)
{
    this->name = "alien";
    this->owner = "";
    this->hp = 0;
    this->speed = 0;

    _sp = sf::Sprite(atlas.texture);
    _sp.setTextureRect(atlas.data[texture]);
}

Alien::~Alien()
{
    //
}

bool Alien::hasDispose() const
{
    return hp < 0;
}

void Alien::handleCollisions()
{
    //
}

void Alien::update()
{
    move(0, speed);
}