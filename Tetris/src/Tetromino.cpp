#include "Tetromino.h"
#include <SFML/Graphics.hpp>
#include <iterator>
#include <algorithm>

using namespace std;

Tetromino::Tetromino(
    int w,
    int h,
    float blockScale,
    sf::Texture &texture)
{
    offset.x = 0;
    offset.y = 0;

    _w = w;
    _h = h;
    _blocks = new int[_w * _h]{0};
    _sp = new sf::Sprite(texture);
    _sp->setScale(blockScale, blockScale);
}

Tetromino::~Tetromino()
{
    delete _sp;
    delete[] _blocks;
}

int Tetromino::getWidth()
{
    return _w;
}

int Tetromino::getHeight()
{
    return _h;
}

/*
    returns value at block(x,y) of the current Tetromino 
*/
int Tetromino::getValue(int x, int y) const
{
    if (x < 0 || x >= _w || y < 0 || y >= _h)
        return -1;
    return _blocks[y * _w + x];
}

/*
    rotates Tetromino clockwise
*/
void Tetromino::rotateRight()
{
    int temp[_w * _h] = {0};
    copy(_blocks, _blocks + (_w * _h), temp);

    for (int y = 0; y < _h; y++)
    {
        for (int x = 0; x < _w; x++)
        {
            int desX = _h - y - 1;
            int desY = x;
            _blocks[desY * _h + desX] = temp[y * _w + x];
        }
    }

    int tempW = _w;
    _w = _h;
    _h = tempW;
}

/*
    rotate Tetromino counter clockwise
*/
void Tetromino::rotateLeft()
{
    int temp[_w * _h] = {0};
    copy(_blocks, _blocks + (_w * _h), temp);

    for (int y = 0; y < _h; y++)
    {
        for (int x = 0; x < _w; x++)
        {
            int desX = y;
            int desY = _w - x - 1;
            _blocks[desY * _h + desX] = temp[y * _w + x];
        }
    }

    int tempW = _w;
    _w = _h;
    _h = tempW;
}

/*
    mirrors the Tetrominos shape in X axis
*/
void Tetromino::mirrorX()
{
    for (int y = 0; y < _h; y++)
    {
        int temp[_w]{0};
        for (int x = 0; x < _w; x++)
        {
            temp[_w - x - 1] = _blocks[y * _w + x];
        }

        for (int x = 0; x < _w; x++)
        {
            _blocks[y * _w + x] = temp[x];
        }
    }
}

/*
    draw Tetromino into a screen
*/
void Tetromino::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (int y = 0; y < _h; y++)
    {
        for (int x = 0; x < _w; x++)
        {
            if (getValue(x, y) == 0)
                continue;

            int blockX = pos.x + x;
            int blockY = pos.y + y;
            _sp->setPosition(
                blockX * _sp->getGlobalBounds().width + offset.x,
                blockY * _sp->getGlobalBounds().height + offset.y);
            target.draw(*_sp);
        }
    }
}
