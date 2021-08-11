#include "Projectile.h"
#include "Atlas.h"

Projectile::Projectile(const char *texture, Atlas &atlas)
{
    this->name = "projectile";
    this->owner = "";
    this->hp = 0;

    _sp = sf::Sprite(atlas.texture);
    _sp.setTextureRect(atlas.data[texture]);
    _sp.setOrigin(
        _sp.getLocalBounds().width / 2,
        _sp.getLocalBounds().height / 2);
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
