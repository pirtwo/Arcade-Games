#ifndef ASTEROID_H_INCLUDE
#define ASTEROID_H_INCLUDE

#include <SFML/Graphics.hpp>

class Asteroid : public sf::Sprite
{
protected:
    sf::Vector2f _velocity;
    int _hp;
    int _maxHP;
    float _speed;
    float _angle;

public:
    Asteroid(
        sf::Texture &texture,
        int maxHP,
        float speed,
        float angle);
    ~Asteroid();
    int getHP();
    int getMaxHP();
    float getAngle();
    void takeHit();
    void update();
};

#endif