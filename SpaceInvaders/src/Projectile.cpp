#include "Projectile.h"
#include "Atlas.h"

Projectile::Projectile(const char *texture, Atlas &atlas)
{
    this->name = "projectile";
    this->owner = "";
    this->hp = 0;
    this->range = 0;

    _sp = sf::Sprite(atlas.texture);
    _sp.setTextureRect(atlas.data[texture]);
}

Projectile::~Projectile()
{
    //
}

bool Projectile::hasDispose() const
{
    return false;
}

void Projectile::handleCollisions()
{
    //
}

void Projectile::update()
{
    move(0, vel.y);
}
