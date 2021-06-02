#ifndef ASTEROID_H_INCLUDE
#define ASTEROID_H_INCLUDE

#include <SFML/Graphics.hpp>

class Asteroid : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Vector2f _velocity;
    sf::Sprite _sp;
    float _radius;
    float _speed;
    float _angle;

public:
    Asteroid(
        sf::Texture &texture,
        float radius,
        float speed,
        float angle);
    ~Asteroid();
    void update();
    virtual void draw(
        sf::RenderTarget &target,
        sf::RenderStates states) const;
};

#endif