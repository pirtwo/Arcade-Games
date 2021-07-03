#ifndef PADDLE_H_INCLUDE
#define PADDLE_H_INCLUDE

#include "SceneComponent.h"

class Paddle : public SceneComponent
{
protected:
    float _speed;
    sf::Vector2f _vel;
    sf::Sprite _sp;

public:
    Paddle(sf::Texture &texture);
    ~Paddle();
    float getSpeed();
    void setSpeed(float val);
    void stop();
    void slideLeft();
    void slideRight();
    void update(float delta) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif