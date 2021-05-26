#ifndef TETROMINO_H_INCLUDE
#define TETROMINO_H_INCLUDE

#include <SFML/Graphics.hpp>

class Tetromino : public sf::Drawable
{
protected:
    int _w;
    int _h;
    int *_blocks;
    mutable sf::Sprite *_sp;

public:
    sf::Vector2i offset;

public:
    friend class TetrominoFactory;
    sf::Vector2i pos;

    Tetromino(
        int w,
        int h,
        float blockScale,
        sf::Texture &texture);
    virtual ~Tetromino();
    virtual int getWidth();
    virtual int getHeight();
    virtual int getValue(int x, int y) const;
    virtual void rotateRight();
    virtual void rotateLeft();
    virtual void mirrorX();
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif