#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include "Line.h"
#include "constant.h"

/*
    draws solid line from a to b
*/
Line::Line(sf::Vector2f a, sf::Vector2f b, int thikness, sf::Color color)
{
    float angle = atan2(b.y - a.y, b.x - a.x) * 180 / PI;
    float width = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));

    auto segment = sf::RectangleShape(sf::Vector2f(width, thikness));
    segment.setPosition(a);
    segment.setFillColor(color);
    segment.rotate(angle);
    _segments.push_back(segment);
}

/*
    draws dashed line from a to b
*/
Line::Line(sf::Vector2f a, sf::Vector2f b, int segments, float padding, int thikness, sf::Color color)
{
    float angle = atan2(b.y - a.y, b.x - a.x);
    float width = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    float segSize = width / segments;
    float segPadding = (segSize * padding) / 100;

    for (int i = 0; i < segments; i++)
    {
        float x = a.x + cos(angle) * i * segSize;
        float y = a.y + sin(angle) * i * segSize;

        auto segment = sf::RectangleShape(sf::Vector2f(segSize - segPadding, thikness));

        segment.rotate(angle * 180 / PI);
        segment.setPosition(x, y);
        segment.setFillColor(color);
        _segments.push_back(segment);
    }
}

Line::~Line()
{
    //
}

void Line::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto &&i : _segments)
    {
        target.draw(i);
    }
}