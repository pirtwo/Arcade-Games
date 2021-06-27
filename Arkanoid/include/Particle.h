#ifndef PARTICLE_H_INCLUDE
#define PARTICLE_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <memory>
#include <math.h>
#include "Random.h"

struct Particle : public sf::Transformable
{
    int spIndex;
    int fadeSpeed;
    sf::Color color;
    sf::Vector2f vel;
};

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
protected:
    Random _rg;
    float _minAngle;
    float _maxAngle;
    float _minSpeed;
    float _maxSpeed;
    float _minScale;
    float _maxScale;
    float _minRotation;
    float _maxRotation;
    int _minFade;
    int _maxFade;
    bool _loop;
    std::vector<std::unique_ptr<Particle>> _particles;
    std::vector<std::unique_ptr<sf::Sprite>> _sprites;

    void initParticle(Particle &p)
    {
        float speed = _rg.randFloat(_minSpeed, _maxSpeed);
        float angle = _rg.randFloat(_minAngle, _maxAngle);
        float scale = _rg.randFloat(_minScale, _maxScale);
        float rotation = _rg.randFloat(_minRotation, _maxRotation);

        p.spIndex = _rg.randInt(0, _sprites.size() - 1);
        p.color = sf::Color(255, 255, 255, 255);
        p.setPosition(getPosition());
        p.setScale(scale, scale);
        p.rotate(rotation);
        p.vel = sf::Vector2f(
            cos(angle * 3.1415 / 180) * speed,
            sin(angle * 3.1415 / 180) * speed);
        p.fadeSpeed = _rg.randInt(_minFade, _maxFade);
        p.color.a = 255;
    }

public:
    ParticleSystem(
        sf::Texture textures[],
        int textureNum,
        float minAngle,
        float maxAngle,
        float minSpeed,
        float maxSpeed,
        float minScale,
        float maxScale,
        float minRotation,
        float maxRotation,
        int minFade,
        int maxFade,
        bool loop)
    {
        _minAngle = minAngle;
        _maxAngle = maxAngle;
        _minSpeed = minSpeed;
        _maxSpeed = maxSpeed;
        _minScale = minScale;
        _maxScale = maxScale;
        _minRotation = minRotation;
        _maxRotation = maxRotation;
        _minFade = minFade;
        _maxFade = maxFade;
        _loop = loop;

        for (int i = 0; i < textureNum; i++)
            _sprites.push_back(std::make_unique<sf::Sprite>(textures[i]));
    }

    ~ParticleSystem()
    {
        //
    }

    void addFule(int n)
    {
        for (int i = 0; i < n; i++)
        {
            auto p = std::make_unique<Particle>();
            initParticle(*p);
            _particles.push_back(std::move(p));
        }
    }

    int getFule()
    {
        return _particles.size();
    }

    void update(float delta)
    {
        for (auto i = _particles.begin(); i != _particles.end(); i++)
        {
            auto p = i->get();
            p->move(p->vel);
            p->color.a = p->color.a - p->fadeSpeed <= 0 ? 0 : p->color.a - p->fadeSpeed;

            if (p->color.a <= 0)
            {
                if (_loop)
                {
                    initParticle(*p);
                }
                else
                {
                    i = _particles.erase(i);
                    if (i == _particles.end())
                        break;
                }
            }
        }
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        for (auto &&i : _particles)
        {
            auto sp = _sprites[i->spIndex].get();
            sp->setColor(i->color);
            target.draw(*sp, states.transform * i->getTransform());
        }
    }
};

#endif