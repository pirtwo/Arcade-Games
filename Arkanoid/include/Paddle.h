#ifndef PADDLE_H_INCLUDE
#define PADDLE_H_INCLUDE

#include "Entity.h"

class Paddle : public Entity
{
protected:
    float _speed;
    sf::Sprite _sp;
    sf::Vector2f _vel;

public:
    Paddle(sf::Texture &t, float speed);
    ~Paddle();
    sf::FloatRect getBounds() override;
    void moveLeft();
    void moveRight();
    void stop();
    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif