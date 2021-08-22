#ifndef MAZE_H_INCLUDE
#define MAZE_H_INCLUDE

#include <vector>

struct Cell
{
    int row = 0, col = 0;
    int walls[4] = {1, 1, 1, 1};
    bool visited = false;
};

class Maze
{
protected:
    int _rowNum, _colNum;

public:
    Maze(int rows, int cols);

    ~Maze();

    int getRowNum();

    int getColNum();

    void genPathDFS(int startRow = 0, int startCol = 0);

    std::vector<Cell> cells;
};

#endif