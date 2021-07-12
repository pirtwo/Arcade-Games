#ifndef ENTITY_H_INCLUDE
#define ENTITY_H_INCLUDE

#include <SFML/Graphics.hpp>

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    std::string name;
    virtual ~Entity() {}
    virtual sf::FloatRect getBounds() = 0;
    virtual void update() = 0;
    virtual void draw(
        sf::RenderTarget &target,
        sf::RenderStates states) const override = 0;
};

#endif