#ifndef BALL_H_INCLUDE
#define BALL_H_INCLUDE

#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable, sf::Transformable
{
protected:
    sf::RectangleShape _sp;

public:
    float speed;
    float angle;
    sf::Vector2f velocity;

public:
    Ball(float w, float h, sf::Color color);
    ~Ball();
    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif