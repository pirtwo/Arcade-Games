#ifndef TETROMINOFACTORY_H_INCLUDE
#define TETROMINOFACTORY_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

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
    vector<shared_ptr<sf::Texture>> *textures;

public:
    TetrominoFactory(vector<shared_ptr<sf::Texture>> &textures);
    ~TetrominoFactory();
    unique_ptr<Tetromino> create(TetrominoType tetroType, float scale);
};

#endif