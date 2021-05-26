#ifndef TETROMINOFACTORY_H_INCLUDE
#define TETROMINOFACTORY_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

class Tetromino;

enum TetrominoType
{
    Straight = 1,
    Square,
    ShapeL,
    ShapeT,
    Skew
};

class TetrominoFactory
{
public:
    vector<sf::Texture *> *textures;
    TetrominoFactory(vector<sf::Texture *> &textures);
    Tetromino *create(TetrominoType tetroType, float scale);
};

#endif