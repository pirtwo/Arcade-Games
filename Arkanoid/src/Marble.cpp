#include "Marble.h"

Marble::Marble(sf::Texture &texture)
{
    _sp = sf::Sprite(texture);
}

Marble::~Marble()
{
    //
}

void Marble::update(float delta)
{
    //
}

void Marble::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, states.transform * getTransform());
}