#ifndef PADDLE_H_INCLUDE
#define PADDLE_H_INCLUDE

#include <SFML/Graphics.hpp>

class Paddle : public sf::Drawable, public sf::Transformable
{

protected:
    sf::RectangleShape _sp;

public:
    float speed;
    sf::Vector2f velocity;

public:
    Paddle(float w, float h, sf::Color color);
    ~Paddle();
    sf::FloatRect getBody();
    void moveUp();
    void moveDown();
    void stop();
    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif