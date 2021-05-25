#include "Field.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include "Tetromino.h"

using namespace std;

Field::Field(
    int x,
    int y,
    int offsetX,
    int offsetY,
    vector<sf::Texture> &textures)
{
    _x = x;
    _y = y;
    _offsetX = offsetX;
    _offsetY = offsetY;
    _blocks = new int[_x * _y]{0};
    _sprites = new sf::Sprite[textures.size()];

    for (int i = 0; i < textures.size(); i++)
    {
        _sprites[i].setTexture(textures[i]);
    }
}

Field::~Field()
{
    delete[] _blocks;
    delete[] _sprites;
}

/*
    draws field on screen
*/
void Field::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (int i = 0; i < _x * _y; i++)
    {
        int blockY = i / _y;
        int blockX = i - blockY * _x;

        auto sp = _sprites[_blocks[i]];
        sp.setPosition(blockX, blockY);
        target.draw(sp);
    }
}

/*
    adds Tetromino to the field
*/
void Field::addTetromino(Tetromino *t)
{
    for (int y = 0; y < t->getHeight(); y++)
    {
        for (int x = 0; x < t->getWidth(); x++)
        {
            if (t->getValue(x, y) == 0)
                continue;
            int blockX = t->pos.x + x;
            int blockY = t->pos.y + y;
            _blocks[blockY * _x + blockX] = t->getValue(x, y);
        }
    }
}

/*
    clears all blocks from field
*/
void Field::clear()
{
    for (int i = 0; i < _x * _y; i++)
        _blocks[i] = 0;
}

/*
    shifts non empty rows to empty spaces
*/
void Field::pushDown()
{
    list<int> emptyRows;
    for (int y = 0; y < _y; y++)
    {
        /*
            check row to see if it's empty or not
        */

        bool isEmpty = true;
        for (int x = 0; x < _x; x++)
        {
            if (_blocks[y * _x + x] != 0)
            {
                isEmpty = false;
                break;
            }
        }

        if (isEmpty)
        {
            /*
                row is empty, store the row index 
            */

            emptyRows.push_back(y);
        }
        else if (emptyRows.size() > 0)
        {
            /* 
                row is not empty and we have empty row below it,
                copy current row to first empty row we have
            */

            int rowIdx = emptyRows.front();
            emptyRows.pop_front();
            for (int x = 0; x < _x; x++)
                _blocks[rowIdx * _x + x] = _blocks[y * _x + x];
        }
    }
}

/*
    removes complete rows from field and returns number of deletes
*/
int Field::removeCompleteRows()
{
    int completeNum = 0;
    for (int y = 0; y < _y; y++)
    {
        /*
            check to see if row is complete
        */

        bool isComplete = true;
        for (int x = 0; x < _x; x++)
        {
            if (_blocks[y * _x + x] == 0)
            {
                isComplete = false;
                break;
            }
        }

        if (isComplete)
        {
            /*
                row is complete, remove it and
                add one to complete row counter
            */

            for (int x = 0; x < _x; x++)
            {
                _blocks[y * _x + x] = 0;
                completeNum++;
            }
        }
    }

    return completeNum;
}

/*
    checks for Tetromino collision with bounds or non zero blocks in X axis
*/
bool Field::collisionX(Tetromino *t)
{
    for (int y = 0; y < t->getHeight(); y++)
    {
        for (int x = 0; x < t->getWidth(); x++)
        {
            if (t->getValue(x, y) == 0)
                continue;

            int blockX = t->pos.x + x;
            int blockY = t->pos.y + y;
            if (blockX < 0 || blockX >= _x || _blocks[blockY * _x + blockX] != 0)
                return true;
        }
    }

    return false;
}

/*
    checks for Tetromino collision with bounds or non zero blocks in Y axis
*/
bool Field::collisionY(Tetromino *t)
{
    for (int y = t->getHeight() - 1; y >= 0; y--)
    {
        for (int x = t->getWidth() - 1; x >= 0; x--)
        {
            if (t->getValue(x, y) == 0)
                continue;

            int blockX = t->pos.x + x;
            int blockY = t->pos.y + y;
            if (blockY < 0 || blockY >= _y || _blocks[blockY * _x + blockX] != 0)
                return true;
        }
    }

    return false;
}
