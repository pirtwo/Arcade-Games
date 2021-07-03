#ifndef MARBLE_H_INCLUDE
#define MARBLE_H_INCLUDE

#include "SceneComponent.h"

class Marble : public SceneComponent
{
protected:
    sf::Sprite _sp;

public:
    Marble(sf::Texture &texture);
    ~Marble();
    void update(float delta) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif