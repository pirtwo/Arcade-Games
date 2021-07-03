#include "Paddle.h"

Paddle::Paddle(sf::Texture &texture)
{
    _vel = sf::Vector2f(0, 0);
    _sp = sf::Sprite(texture);
}

Paddle::~Paddle()
{
    //
}

float Paddle::getSpeed()
{
    return _speed;
}

void Paddle::setSpeed(float val)
{
    _speed = val;
}

void Paddle::stop()
{
    _vel.x = 0;
}

void Paddle::slideLeft()
{
    _vel.x = _speed * -1;
}

void Paddle::slideRight()
{
    _vel.x = _speed;
}

void Paddle::update(float delta)
{
    move(_vel);
}

void Paddle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, states.transform * getTransform());
}
