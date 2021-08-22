#include "Maze.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <chrono>

Maze::Maze(int rows, int cols)
{
    _rowNum = rows;
    _colNum = cols;

    for (int i = 0; i < _rowNum; i++)
        for (int j = 0; j < _colNum; j++)
        {
            cells.push_back(Cell{i, j, {1, 1, 1, 1}, false});
        }
}

Maze::~Maze()
{
    cells.clear();
}

int Maze::getRowNum()
{
    return _rowNum;
}

int Maze::getColNum()
{
    return _colNum;
}

void Maze::genPathDFS(int startRow, int startCol)
{
    auto getUnvisited =
        [&](Cell &c, int rows, int cols) -> std::vector<Cell *>
    {
        std::vector<Cell *> list;
        int index = 0;

        if (c.row - 1 >= 0)
        {
            index = (c.row - 1) * cols + c.col;
            if (!this->cells[index].visited)
                list.push_back(&this->cells[index]);
        }
        if (c.row + 1 < rows)
        {
            index = (c.row + 1) * cols + c.col;
            if (!this->cells[index].visited)
                list.push_back(&this->cells[index]);
        }
        if (c.col - 1 >= 0)
        {
            index = c.row * cols + (c.col - 1);
            if (!this->cells[index].visited)
                list.push_back(&this->cells[index]);
        }
        if (c.col + 1 < cols)
        {
            index = c.row * cols + (c.col + 1);
            if (!this->cells[index].visited)
                list.push_back(&this->cells[index]);
        }

        return list;
    };

    auto removeWall =
        [&](Cell &a, Cell &b)
    {
        if (a.row < b.row)
        {
            a.walls[1] = 0;
            b.walls[0] = 0;
        }
        else if (a.row > b.row)
        {
            a.walls[0] = 0;
            b.walls[1] = 0;
        }
        else if (a.col < b.col)
        {
            a.walls[3] = 0;
            b.walls[2] = 0;
        }
        else if (a.col > b.col)
        {
            a.walls[2] = 0;
            b.walls[3] = 0;
        }
    };

    std::vector<Cell *> list;
    list.push_back(&cells[startRow * _colNum + startCol]);
    std::mt19937 gen{std::random_device{}()};

    while (!list.empty())
    {
        auto curr = list.back();
        list.pop_back();

        curr->visited = true;

        auto unvisited = getUnvisited(*curr, _rowNum, _colNum);

        if (unvisited.size() > 0)
        {
            std::shuffle(unvisited.begin(), unvisited.end(), gen);
            auto next = unvisited[0];
            removeWall(*curr, *next);
            list.push_back(curr);
            list.push_back(next);
        }
    }
}
