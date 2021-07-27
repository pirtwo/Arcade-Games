#ifndef ENTITY_H_INCLUDE
#define ENTITY_H_INCLUDE

#include <SFML/Graphics.hpp>

class Entity : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Sprite _sp;

public:
    std::string name;
    int hp;

    virtual ~Entity() {}

    virtual void update() = 0;

    virtual sf::FloatRect getBounds() const
    {
        return getTransform().transformRect(_sp.getGlobalBounds());
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(_sp, states.transform * getTransform());
    }
};

#endif