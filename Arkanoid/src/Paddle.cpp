#include "Paddle.h"
#include "Atlas.h"

Paddle::Paddle(Atlas &a, float speed)
{
    this->name = "paddle";
    this->speed = speed;
    _sp = sf::Sprite(a.texture);
    _sp.setTextureRect(a.data["paddle.png"]);
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
    vel.x = speed * -1;
}

void Paddle::moveRight()
{
    vel.x = speed;
}

void Paddle::stop()
{
    vel.x = 0;
}

void Paddle::update()
{
    move(vel);
}

void Paddle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, states.transform * getTransform());
}