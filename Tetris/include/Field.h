#ifndef FIELD_H_INCLUDE
#define FIELD_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <list>
#include <vector>

using namespace std;

class Tetromino;

class Field : public sf::Drawable
{
protected:
    int _w;
    int _h;
    int *_blocks;
    sf::Sprite *_sprites;

public:
    sf::Vector2i offset;

public:
    Field(
        int w,
        int h,
        float blockScale,
        vector<sf::Texture *> &textures);
    virtual ~Field();
    virtual int removeCompleteRows();
    virtual bool collisionX(Tetromino &t);
    virtual bool collisionY(Tetromino &t);
    virtual void clear();
    virtual void pushDown();
    virtual void addTetromino(Tetromino &t);
    virtual void draw(
        sf::RenderTarget &target,
        sf::RenderStates states) const;
};

#endif