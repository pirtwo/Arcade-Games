#ifndef BALL_H_INCLUDE
#define BALL_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <math.h>

class Ball : public sf::Drawable, public sf::Transformable
{
protected:
    float _radius;
    sf::CircleShape _shape;

public:
    sf::Vector2f gravity;
    sf::Vector2f velocity;
    sf::Vector2f friction;

public:
    Ball(float radius)
    {
        gravity = sf::Vector2f(0, 1.5);
        velocity = sf::Vector2f(0, 0);
        friction = sf::Vector2f(0.95, 0.95);

        _radius = radius;
        _shape = sf::CircleShape(radius);
        _shape.setFillColor(sf::Color::Red);
        _shape.setOrigin(radius / 2, radius / 2);
    }

    virtual ~Ball()
    {
        //
    }
    sf::FloatRect bounds()
    {
        return sf::FloatRect(
            getPosition().x,
            getPosition().y,
            _radius * 2,
            _radius * 2);
    }

    float getRadius()
    {
        return _radius;
    }

    void bounceX()
    {
        velocity.x *= -1;
    }

    void bounceY()
    {
        velocity.y *= -1;
    }

    void applyForce(float magnitude, float angle)
    {
        sf::Vector2f force(0, 0);
        force.x = cos(angle) * magnitude;
        force.y = sin(angle) * magnitude;
        velocity.x += force.x;
        velocity.y += force.y;
    }

    void update(float delta)
    {
        velocity.x += gravity.x;
        velocity.y += gravity.y;
        velocity.x *= friction.x;
        velocity.y *= friction.y;
        move(velocity.x, velocity.y);
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(_shape, getTransform());
    }
};

#endif