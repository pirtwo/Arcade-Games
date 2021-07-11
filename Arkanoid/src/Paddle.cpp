#include "Paddle.h"

Paddle::Paddle(sf::Texture &t, float speed)
{
    name = "paddle";
    _sp = sf::Sprite(t);
    _speed = speed;
}

Paddle::~Paddle()
{
    //
}

sf::FloatRect Paddle::getBounds()
{
    return getTransform().transformRect(_sp.getLocalBounds());
}

void Paddle::moveLeft()
{
    _vel.x = _speed * -1;
}

void Paddle::moveRight()
{
    _vel.x = _speed;
}

void Paddle::update()
{
    move(_vel);
}

void Paddle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, states.transform * getTransform());
}