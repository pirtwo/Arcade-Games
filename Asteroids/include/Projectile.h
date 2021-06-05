#ifndef PROJECTILE_H_INCLUDE
#define PROJECTILE_H_INCLUDE

#include <SFML/Graphics.hpp>

class Projectile : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Sprite _sp;
    sf::Vector2f _velocity;
    float _speed;
    float _angle;
    float _range;
    float _traveledDist;

public:
    Projectile(
        sf::Texture &texture,
        float speed,
        float angle,
        float range);
    ~Projectile();
    bool isBeyondRange();
    void update();
    void draw(
        sf::RenderTarget &target,
        sf::RenderStates states) const;
};

#endif