#ifndef WALL_H_INCLUDE
#define WALL_H_INCLUDE

#include "Obstacle.h"
#include "Ball.h"
#include <SFML/Graphics.hpp>

class Wall : public Obstacle
{
protected:
    float _w;
    float _h;
    sf::RectangleShape _shape;

public:
    Wall(float w, float h, sf::Color color)
    {
        _shape = sf::RectangleShape(sf::Vector2f(w, h));
        _shape.setFillColor(color);
    }

    virtual ~Wall()
    {
        //
    }

    sf::FloatRect bounds() const override
    {
        return sf::FloatRect(
            getPosition().x,
            getPosition().y,
            _shape.getSize().x,
            _shape.getSize().y);
    }

    void onCollision(Ball &ball) override
    {
        //
    }

    void update() override
    {
        //
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(_shape, getTransform());
    }
};

#endif