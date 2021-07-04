#ifndef SCENECOMPONENT_H_INCLUDE
#define SCENECOMPONENT_H_INCLUDE

#include <SFML/Graphics.hpp>

class SceneComponent : public sf::Drawable, public sf::Transformable
{
public:
    virtual ~SceneComponent() {}
    virtual void update(float delta) = 0;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

#endif