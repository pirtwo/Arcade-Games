#ifndef PARTICLE_H_INCLUDE
#define PARTICLE_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
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

    void initParticle(Particle &p);

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
        bool loop);
    ~ParticleSystem();
    int getFule();
    void addFule(int n);
    void update(float delta);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif