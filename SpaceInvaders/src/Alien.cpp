#include "Alien.h"
#include "Atlas.h"

Alien::Alien(const char *texture, Atlas &atlas)
{
    this->name = "alien";
    this->owner = "";
    this->hp = 0;

    _sp = sf::Sprite(atlas.texture);
    _sp.setTextureRect(atlas.data[texture]);
    _sp.setOrigin(
        _sp.getLocalBounds().width / 2,
        _sp.getLocalBounds().height / 2);
}

Alien::~Alien()
{
    //
}

bool Alien::hasDispose() const
{
    return hp == 0;
}

void Alien::handleCollisions()
{
    for (auto &&i : collisions)
    {
        if (auto elm = i.lock())
        {
            if (elm->name == "projectile" && elm->owner != owner)
                hp--;
        }
    }

    collisions.clear();
}

void Alien::update()
{
    move(0, vel.y);
}