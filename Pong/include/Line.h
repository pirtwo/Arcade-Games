#ifndef LINE_H_INCLUDE
#define LINE_H_INCLUDE

#include <SFML/Graphics.hpp>

class Line : public sf::Drawable
{
protected:
    std::vector<sf::RectangleShape> _segments;

public:
    Line(
        sf::Vector2f a,
        sf::Vector2f b,
        int thikness,
        sf::Color color);
    Line(
        sf::Vector2f a,
        sf::Vector2f b,
        int segments,
        float padding,
        int thikness,
        sf::Color color);
    ~Line();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif
