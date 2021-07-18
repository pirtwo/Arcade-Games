#include "Tile.h"
#include "Atlas.h"

Tile::Tile(Atlas &a, int hp)
{
    this->name = "tile";
    this->hp = hp;
    _sp = sf::Sprite(a.texture);
    _sp.setTextureRect(a.data["tileGreen_01.png"]);
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