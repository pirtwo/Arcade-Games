#include "Asteroid.h"
#include "Helper.h"
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

Asteroid::Asteroid(sf::Texture &texture, int maxHP, float speed, float angle)
{
    _hp = maxHP;
    _maxHP = maxHP;
    _speed = speed;
    _angle = angle;
    _velocity.x = cos(degreeToRadian(_angle)) * speed;
    _velocity.y = sin(degreeToRadian(_angle)) * speed;
    setTexture(texture);
    setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
}

Asteroid::~Asteroid()
{
    std::cout << "~Asteroid\r\n";
}

void Asteroid::takeHit()
{
    _hp--;
}

int Asteroid::getHP()
{
    return _hp;
}

int Asteroid::getMaxHP()
{
    return _maxHP;
}

float Asteroid::getAngle()
{
    return _angle;
}

void Asteroid::update()
{
    move(_velocity);
}
