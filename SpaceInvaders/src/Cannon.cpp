#include "Cannon.h"
#include "Atlas.h"

Cannon::Cannon(const char *texture, Atlas &atlas)
{
    this->name = "cannon";
    this->owner = "";
    this->hp = 1;
    this->speed = 0;

    _sp = sf::Sprite(atlas.texture);
    _sp.setTextureRect(atlas.data[texture]);
    _sp.setOrigin(
        _sp.getLocalBounds().width / 2,
        _sp.getLocalBounds().height / 2);
}

Cannon::~Cannon()
{
    //
}

void Cannon::moveLeft()
{
    vel.x = speed * -1;
}

void Cannon::moveRight()
{
    vel.x = speed;
}

void Cannon::stop()
{
    vel.x = 0;
}

bool Cannon::hasDispose() const
{
    return hp == 0;
}

void Cannon::handleCollisions()
{
    for (auto &&i : collisions)
    {
        if (auto elm = i.lock())
        {
            if (elm->name == "alien" ||
                (elm->name == "projectile" && elm->owner != owner))
                hp--;
        }
    }

    collisions.clear();
}

void Cannon::update()
{
    move(vel.x, 0);
}
