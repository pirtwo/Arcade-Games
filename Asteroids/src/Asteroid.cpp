#include "Asteroid.h"
#include "Helper.h"
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

Asteroid::Asteroid(sf::Texture &texture, float radius, float speed, float angle)
{
    _radius = radius;
    _speed = speed;
    _angle = angle;
    _velocity.x = cos(degreeToRadian(_angle)) * speed;
    _velocity.y = sin(degreeToRadian(_angle)) * speed;

    _sp = sf::Sprite(texture);
}

Asteroid::~Asteroid()
{
    std::cout << "~Asteroid\r\n";
}

void Asteroid::update()
{
    move(_velocity);
}

void Asteroid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sp, getTransform());
}