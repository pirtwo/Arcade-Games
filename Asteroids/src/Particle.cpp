#include "Particle.h"
#include "Helper.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <math.h>

using namespace std;

Emitter::Emitter(
    float x,
    float y,
    float minAngle,
    float maxAngle,
    float minSpeed,
    float maxSpeed,
    int minFade,
    int maxFade)
{
    _x = x;
    _y = y;
    _minAngle = minAngle;
    _maxAngle = maxAngle;
    _minSpeed = minSpeed;
    _maxSpeed = maxSpeed;
    _minFade = minFade;
    _maxFade = maxFade;
}

int Emitter::getFule()
{
    return _fuel;
}

void Emitter::addFuel(int n)
{
    _fuel = n;
    for (int i = 0; i < _fuel; i++)
    {
        auto p = unique_ptr<Particle>(new Particle());
        float angle = randFloat(_minAngle, _maxAngle);
        float speed = randFloat(_minSpeed, _maxSpeed);

        p->setPosition(_x, _y);
        p->src = sf::Vector2f(_x, _y);
        p->velocity.x = cos(degreeToRadian(angle)) * speed;
        p->velocity.y = sin(degreeToRadian(angle)) * speed;
        p->fadeSpeed = randInt(_minFade, _maxFade);

        _particles.push_back(std::move(p));
    }
}

void Emitter::setMinAngle(float value)
{
    _minAngle = value;
}

void Emitter::setMaxAngle(float value)
{
    _maxAngle = value;
}

void Emitter::setPosition(sf::Vector2f value)
{
    _x = value.x;
    _y = value.y;
}

void Emitter::update()
{
    for (auto i = _particles.begin(); i != _particles.end(); i++)
    {
        auto p = i->get();
        p->move(p->velocity);
        p->ver.color.a =
            p->ver.color.a - p->fadeSpeed <= 0 ? 0 : p->ver.color.a - p->fadeSpeed;

        if (p->ver.color.a <= 0)
        {
            i = _particles.erase(i);
            if (i == _particles.end())
                break;
        }
    }
}

void Emitter::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto i = _particles.begin(); i != _particles.end(); i++)
        target.draw(*i->get());
}
