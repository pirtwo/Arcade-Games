#ifndef TETROMINO_H_INCLUDE
#define TETROMINO_H_INCLUDE

#include <SFML/Graphics.hpp>

class Tetromino
{
protected:
    int _w, _h;
    int *_blocks;

public:
    friend class TetrominoFactory;
    sf::Vector2i pos;

    Tetromino(int w, int h);
    ~Tetromino();
    int getWidth();
    int getHeight();
    int getValue(int x, int y);
    void rotateRight();
    void rotateLeft();
    void miror();
};

#endif