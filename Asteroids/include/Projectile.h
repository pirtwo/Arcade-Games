#ifndef PROJECTILE_H_INCLUDE
#define PROJECTILE_H_INCLUDE

#include <SFML/Graphics.hpp>

class Projectile : public sf::Sprite
{
protected:
    sf::Vector2f _velocity;
    float _speed;
    float _angle;
    float _range;
    float _traveledDist;
    int _actor;

public:
    Projectile(
        sf::Texture &texture,
        float speed,
        float angle,
        float range);
    ~Projectile();
    bool isBeyondRange();
    int getActor();
    void setActor(int actor);
    void update();
};

#endif