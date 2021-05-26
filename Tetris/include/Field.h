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
    int _offsetX;
    int _offsetY;
    int *_blocks;
    sf::Sprite *_sprites;

public:
    Field(
        int w,
        int h,
        int offsetX,
        int offsetY,
        float blockScale,
        vector<sf::Texture *> &textures);
    virtual ~Field();

    virtual int removeCompleteRows();
    virtual bool collisionX(Tetromino *t);
    virtual bool collisionY(Tetromino *t);
    virtual void clear();
    virtual void pushDown();
    virtual void addTetromino(Tetromino *t);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif