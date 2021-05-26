#include "TetrominoFactory.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "Tetromino.h"

using namespace std;

TetrominoFactory::TetrominoFactory(vector<shared_ptr<sf::Texture>> &textures)
{
    this->textures = &textures;
}

TetrominoFactory::~TetrominoFactory()
{
    cout << "~factory \r\n";
}

unique_ptr<Tetromino> TetrominoFactory::create(TetrominoType tetroType, float scale)
{
    unique_ptr<Tetromino> t;

    switch (tetroType)
    {
    case TetrominoType::Straight:
        t = unique_ptr<Tetromino>(
            new Tetromino(4, 1, scale, *textures->at(1)));
        t->_blocks[0] = TetrominoType::Straight;
        t->_blocks[1] = TetrominoType::Straight;
        t->_blocks[2] = TetrominoType::Straight;
        t->_blocks[3] = TetrominoType::Straight;
        return t;
    case TetrominoType::Square:
        t = unique_ptr<Tetromino>(
            new Tetromino(2, 2, scale, *textures->at(2)));
        t->_blocks[0] = TetrominoType::Square;
        t->_blocks[1] = TetrominoType::Square;
        t->_blocks[2] = TetrominoType::Square;
        t->_blocks[3] = TetrominoType::Square;
        return t;
    case TetrominoType::ShapeL:
        t = unique_ptr<Tetromino>(
            new Tetromino(3, 2, scale, *textures->at(3)));
        t->_blocks[0] = TetrominoType::ShapeL;
        t->_blocks[1] = TetrominoType::ShapeL;
        t->_blocks[2] = TetrominoType::ShapeL;
        t->_blocks[3] = TetrominoType::ShapeL;
        return t;
    case TetrominoType::ShapeT:
        t = unique_ptr<Tetromino>(
            new Tetromino(3, 2, scale, *textures->at(4)));
        t->_blocks[0] = TetrominoType::ShapeT;
        t->_blocks[1] = TetrominoType::ShapeT;
        t->_blocks[2] = TetrominoType::ShapeT;
        t->_blocks[4] = TetrominoType::ShapeT;
        return t;
    case TetrominoType::Skew:
        t = unique_ptr<Tetromino>(
            new Tetromino(3, 2, scale, *textures->at(5)));
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
