#ifndef ENTITY_H_INCLUDE
#define ENTITY_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <algorithm>

class Entity : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Sprite _sp;

public:
    int hp;
    std::vector<std::string> tags;

    virtual ~Entity() {}

    virtual void update() = 0;

    virtual bool checkTag(std::string tag)
    {
        return std::any_of(tags.begin(), tags.end(), [&](std::string i)
                           { return i == tag; });
    }

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