#include "Ball.h"
#include "constant.h"
#include <SFML/Graphics.hpp>
#include <math.h>

Ball::Ball(float w, float h, sf::Color color)
{
    _speed = 0;
    _angle = 0;
    _sp = sf::RectangleShape(sf::Vector2f(w, h));
    _sp.setFillColor(color);
}

Ball::~Ball()
{
    //
}

sf::FloatRect Ball::getBody()
{
    return sf::FloatRect(
        getPosition().x,
        getPosition().y,
        _sp.getGlobalBounds().width,
        _sp.getGlobalBounds().height);
}

void Ball::bounceX()
{
    velocity.x *= -1;
}

void Ball::bounceY()
{
    velocity.y *= -1;
}

void Ball::setSpeed(float value)
{
    _speed = value;
    velocity.x = cos(_angle) * _speed;
    velocity.y = sin(_angle) * _speed;
}

void Ball::setAngle(float value)
{
    _angle = value * PI / 180;
    velocity.x = cos(_angle) * _speed;
    velocity.y = sin(_angle) * _speed;
}

void Ball::update()
{
    move(velocity.x, velocity.y);
}

void Ball::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, getTransform());
}