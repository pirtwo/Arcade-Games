#ifndef TILE_H_INCLUDE
#define TILE_H_INCLUDE

#include <Entity.h>

class Tile : public Entity
{
protected:
    sf::Sprite _sp;

public:
    int hp;

public:
    Tile(sf::Texture &t, int hp);
    ~Tile();
    sf::FloatRect getBounds() override;
    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

// TODO: add hit animation

#endif