#include "Ball.h"

Ball::Ball(sf::Texture &t)
{
    _sp = sf::Sprite(t);
}

Ball::~Ball()
{
    //
}

sf::FloatRect Ball::getBounds()
{
    return getTransform().transformRect(_sp.getLocalBounds());
}

sf::Vector2f Ball::getVelocity()
{
    return _vel;
}

void Ball::update()
{
    move(_vel);
}

void Ball::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, states.transform * getTransform());
}