#include "Ball.h"
#include "Atlas.h"

Ball::Ball(Atlas &a)
{
    this->name = "ball";
    this->vel = sf::Vector2f(0, 0);
    _sp = sf::Sprite(a.texture);
    _sp.setTextureRect(a.data["ball.png"]);
    _sp.setOrigin(
        _sp.getLocalBounds().width / 2,
        _sp.getLocalBounds().width / 2);
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