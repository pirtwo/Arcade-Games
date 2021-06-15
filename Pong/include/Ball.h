#ifndef BALL_H_INCLUDE
#define BALL_H_INCLUDE

#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable, public sf::Transformable
{
protected:
    float _speed;
    float _angle;
    sf::RectangleShape _sp;

public:
    sf::Vector2f velocity;

public:
    Ball(float w, float h, sf::Color color);
    ~Ball();
    sf::FloatRect getBody();
    void bounceX();
    void bounceY();
    void setSpeed(float value);
    void setAngle(float value);
    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif