#ifndef TETROMINOFACTORY_H_INCLUDE
#define TETROMINOFACTORY_H_INCLUDE

class Tetromino;

enum TetrominoType
{
    Straight = 1,
    Square,
    ShapeT,
    ShapeL,
    Skew
};

class TetrominoFactory
{
public:
    Tetromino *create(TetrominoType tetroType);
};

#endif