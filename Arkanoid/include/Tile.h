#ifndef TILE_H_INCLUDE
#define TILE_H_INCLUDE

#include <Entity.h>
class Atlas;

class Tile : public Entity
{
protected:
    sf::Sprite _sp;

public:
    int hp;

public:
    Tile(Atlas &a, int hp);
    ~Tile();
    sf::FloatRect getBounds() override;
    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

// TODO: add hit animation

#endif