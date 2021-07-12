#include "Ball.h"

Ball::Ball(sf::Texture &t)
{
    this->name = "ball";
    this->vel = sf::Vector2f(0, 0);
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

void Ball::update()
{
    move(this->vel);
}

void Ball::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, states.transform * getTransform());
}