#include "Tile.h"

Tile::Tile(sf::Texture &t, int hp)
{
    this->name = "tile";
    this->hp = hp;
    _sp = sf::Sprite(t);
}

Tile::~Tile()
{
    //
}

sf::FloatRect Tile::getBounds()
{
    return getTransform().transformRect(_sp.getLocalBounds());
}

void Tile::update()
{
    //
}

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, states.transform * getTransform());
}