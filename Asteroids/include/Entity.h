#ifndef ENTITY_H_INCLUDE
#define ENTITY_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <memory>

class Entity : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Sprite _sp;

public:
    int hp;
    std::string name = "";
    std::string owner = "";
    std::vector<std::weak_ptr<Entity>> collisions;

    virtual ~Entity() {}

    virtual void update() = 0;

    virtual void handleCollisions() = 0;

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