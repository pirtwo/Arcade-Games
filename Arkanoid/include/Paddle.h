#ifndef PADDLE_H_INCLUDE
#define PADDLE_H_INCLUDE

#include "Entity.h"
class Atlas;

class Paddle : public Entity
{
protected:
    sf::Sprite _sp;

public:
    float speed;
    sf::Vector2f vel;

public:
    Paddle(Atlas &a, float speed);
    ~Paddle();
    sf::FloatRect getBounds() override;
    void moveLeft();
    void moveRight();
    void stop();
    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif