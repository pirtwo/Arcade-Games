#include "Tetromino.h"
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

Tetromino::Tetromino(int w, int h)
{
    _w = w;
    _h = h;
    _blocks = new int[w * h]{0};
}

Tetromino::~Tetromino()
{
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

int Tetromino::getValue(int x, int y)
{
    if (x < 0 || x >= _w || y < 0 || y >= _h)
        return -1;
    return _blocks[y * _w + x];
}

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