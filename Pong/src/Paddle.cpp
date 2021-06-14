#include "Paddle.h"
#include <SFML/Graphics.hpp>

Paddle::Paddle(float w, float h, sf::Color color)
{
    _sp = sf::RectangleShape(sf::Vector2f(w, h));
    _sp.setFillColor(color);
}

Paddle::~Paddle()
{
    //
}

void Paddle::moveUp()
{
    velocity.y = -speed;
}

void Paddle::moveDown()
{
    velocity.y = speed;
}

void Paddle::stop()
{
    velocity.y = 0;
}

void Paddle::update()
{
    move(0, velocity.y);
}

void Paddle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, getTransform());
}