#ifndef PARTICLE_H_INCLUDE
#define PARTICLE_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

using namespace std;

class Particle : public sf::Drawable, public sf::Transformable
{
public:
    sf::Vertex ver;
    sf::Vector2f src;
    sf::Vector2f velocity;
    int fadeSpeed;

public:
    Particle()
    {
        ver = sf::Vertex(getPosition(), sf::Color::White);
    }

    ~Particle()
    {
        //
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(&ver, 1, sf::Points, getTransform());
    }
};

class Emitter : public sf::Drawable
{
protected:
    bool _loop;
    int _fuel;
    float _x;
    float _y;
    float _minAngle;
    float _maxAngle;
    float _minSpeed;
    float _maxSpeed;
    int _minFade;
    int _maxFade;
    list<unique_ptr<Particle>> _particles;

public:
    Emitter() {}
    Emitter(
        float x,
        float y,
        float minAngle,
        float maxAngle,
        float minSpeed,
        float maxSpeed,
        int minFade,
        int maxFade);
    void update();
    int getFule();
    void addFuel(int n);
    void setMinAngle(float value);
    void setMaxAngle(float value);
    void setPosition(sf::Vector2f value);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif