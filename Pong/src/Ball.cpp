#include "Ball.h"
#include "constant.h"
#include <SFML/Graphics.hpp>
#include <math.h>

Ball::Ball(float w, float h, sf::Color color)
{
    speed = 0;
    angle = 0;
    _sp = sf::RectangleShape(sf::Vector2f(w, h));
    _sp.setFillColor(color);
}

Ball::~Ball()
{
    //
}

void Ball::update()
{
    velocity.x = cos(angle) * speed;
    velocity.y = sin(angle) * speed;
    move(velocity.x, velocity.y);
}

void Ball::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, getTransform());
}