#include "TetrominoFactory.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tetromino.h"

using namespace std;

TetrominoFactory::TetrominoFactory(vector<sf::Texture *> &textures)
{
    this->textures = &textures;
}

Tetromino *TetrominoFactory::create(TetrominoType tetroType, float scale)
{
    Tetromino *t;

    switch (tetroType)
    {
    case TetrominoType::Straight:
        t = new Tetromino(4, 1, scale, *textures->at(1));
        t->_blocks[0] = TetrominoType::Straight;
        t->_blocks[1] = TetrominoType::Straight;
        t->_blocks[2] = TetrominoType::Straight;
        t->_blocks[3] = TetrominoType::Straight;
        return t;
    case TetrominoType::Square:
        t = new Tetromino(2, 2, scale, *textures->at(2));
        t->_blocks[0] = TetrominoType::Square;
        t->_blocks[1] = TetrominoType::Square;
        t->_blocks[2] = TetrominoType::Square;
        t->_blocks[3] = TetrominoType::Square;
        return t;
    case TetrominoType::ShapeL:
        t = new Tetromino(3, 2, scale, *textures->at(3));
        t->_blocks[0] = TetrominoType::ShapeL;
        t->_blocks[1] = TetrominoType::ShapeL;
        t->_blocks[2] = TetrominoType::ShapeL;
        t->_blocks[3] = TetrominoType::ShapeL;
        return t;
    case TetrominoType::ShapeT:
        t = new Tetromino(3, 2, scale, *textures->at(4));
        t->_blocks[0] = TetrominoType::ShapeT;
        t->_blocks[1] = TetrominoType::ShapeT;
        t->_blocks[2] = TetrominoType::ShapeT;
        t->_blocks[4] = TetrominoType::ShapeT;
        return t;
    case TetrominoType::Skew:
        t = new Tetromino(3, 2, scale, *textures->at(5));
        t->_blocks[0] = TetrominoType::Skew;
        t->_blocks[1] = TetrominoType::Skew;
        t->_blocks[4] = TetrominoType::Skew;
        t->_blocks[5] = TetrominoType::Skew;
        return t;
    default:
        break;
    }

    return t;
}
