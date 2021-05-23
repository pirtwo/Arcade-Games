#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <list>
#include "Tetromino.h"
#include "TetrominoFactory.h"

using namespace std;

const int FieldX = 11;
const int FieldY = 20;
const int FieldOffsetX = 100;
const int FieldOffsetY = 100;
const int TetrominoNum = 5; // number of Tetrominos in game
const int BlockSize = 20;   // size of each block in pixel
const int TextureSize = 88;
const sf::Vector2i SpawnPoint(4, 0);

struct State
{
    bool pause = false;
    bool gameover = false;
    int score = 0;
} gameState;

int removeCompleteLines(int field[FieldY][FieldX]);
void pushFieldDown(int field[FieldY][FieldX]);
void addToField(Tetromino *tetro, int field[FieldY][FieldX]);
bool checkCollisionX(Tetromino *tetro, int field[FieldY][FieldX]);
bool checkCollisionY(Tetromino *tetro, int field[FieldY][FieldX]);
Tetromino *getRandTetromino(Tetromino *list[]);

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 700), "Tetris");
    window.setFramerateLimit(60);

    // load assets
    sf::Font font;
    sf::Texture tileGreyTxt,
        tileBlueTxt,
        tileGreenTxt,
        tileOrangeTxt,
        tileRedTxt,
        tileYellowTxt;
    if (!tileGreyTxt.loadFromFile("./assets/tileGrey.png") ||
        !tileBlueTxt.loadFromFile("./assets/tileBlue.png") ||
        !tileGreenTxt.loadFromFile("./assets/tileGreen.png") ||
        !tileOrangeTxt.loadFromFile("./assets/tileOrange.png") ||
        !tileRedTxt.loadFromFile("./assets/tileRed.png") ||
        !tileYellowTxt.loadFromFile("./assets/tileYellow.png"))
        return EXIT_FAILURE;

    float scale = float(BlockSize) / TextureSize;
    sf::Sprite greyBlock;
    sf::Sprite blueBlock;
    sf::Sprite greenBlock;
    sf::Sprite orangeBlock;
    sf::Sprite redBlock;
    sf::Sprite yellowBlock;

    greyBlock.setTexture(tileGreyTxt);
    blueBlock.setTexture(tileBlueTxt);
    greenBlock.setTexture(tileGreenTxt);
    orangeBlock.setTexture(tileOrangeTxt);
    redBlock.setTexture(tileRedTxt);
    yellowBlock.setTexture(tileYellowTxt);

    greyBlock.scale(scale, scale);
    blueBlock.scale(scale, scale);
    greenBlock.scale(scale, scale);
    orangeBlock.scale(scale, scale);
    redBlock.scale(scale, scale);
    yellowBlock.scale(scale, scale);

    srand(time(NULL));

    sf::Clock clock;
    float delay = 0.5;

    int completeLines = 0;
    int field[FieldY][FieldX]{0};
    Tetromino *curr;
    Tetromino *next;
    Tetromino *tl[5];
    TetrominoFactory *tf = new TetrominoFactory();
    tl[0] = tf->create(TetrominoType::Straight);
    tl[1] = tf->create(TetrominoType::Square);
    tl[2] = tf->create(TetrominoType::ShapeL);
    tl[3] = tf->create(TetrominoType::ShapeT);
    tl[4] = tf->create(TetrominoType::Skew);

    curr = getRandTetromino(tl);
    next = getRandTetromino(tl);
    curr->pos.x = SpawnPoint.x;
    curr->pos.y = SpawnPoint.y;

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Key::Down)
                    delay = 0.05;
            }

            if (e.type == sf::Event::KeyReleased)
            {
                if (e.key.code == sf::Keyboard::Key::Left)
                {
                    curr->pos.x -= 1;
                    if (checkCollisionX(curr, field))
                        curr->pos.x += 1;
                }
                if (e.key.code == sf::Keyboard::Key::Right)
                {
                    curr->pos.x += 1;
                    if (checkCollisionX(curr, field))
                        curr->pos.x -= 1;
                }
                if (e.key.code == sf::Keyboard::Key::Down)
                    delay = 0.5;
                if (e.key.code == sf::Keyboard::Key::Space)
                {
                    curr->rotateRight();
                    if (checkCollisionX(curr, field))
                        curr->rotateLeft();
                }
            }
        }

        if (gameState.pause || gameState.gameover)
            continue;

        window.clear();

        if (completeLines > 0)
            pushFieldDown(field);

        // draw field
        for (int y = 0; y < FieldY; y++)
        {
            for (int x = 0; x < FieldX; x++)
            {
                switch (field[y][x])
                {
                case 0:
                    greyBlock.setPosition(
                        x * BlockSize + FieldOffsetX,
                        y * BlockSize + FieldOffsetY);
                    window.draw(greyBlock);
                    break;
                case 1:
                    blueBlock.setPosition(
                        x * BlockSize + FieldOffsetX,
                        y * BlockSize + FieldOffsetY);
                    window.draw(blueBlock);
                    break;
                case 2:
                    greenBlock.setPosition(
                        x * BlockSize + FieldOffsetX,
                        y * BlockSize + FieldOffsetY);
                    window.draw(greenBlock);
                    break;
                case 3:
                    orangeBlock.setPosition(
                        x * BlockSize + FieldOffsetX,
                        y * BlockSize + FieldOffsetY);
                    window.draw(orangeBlock);
                    break;
                case 4:
                    redBlock.setPosition(
                        x * BlockSize + FieldOffsetX,
                        y * BlockSize + FieldOffsetY);
                    window.draw(redBlock);
                    break;
                case 5:
                    yellowBlock.setPosition(
                        x * BlockSize + FieldOffsetX,
                        y * BlockSize + FieldOffsetY);
                    window.draw(yellowBlock);
                    break;
                default:
                    break;
                }
            }
        }

        // draw current tetromino
        for (int y = 0; y < curr->getHeight(); y++)
        {
            for (int x = 0; x < curr->getWidth(); x++)
            {
                switch (curr->getValue(x, y))
                {
                case 1:
                    blueBlock.setPosition(
                        (x + curr->pos.x) * BlockSize + FieldOffsetX,
                        (y + curr->pos.y) * BlockSize + FieldOffsetY);
                    window.draw(blueBlock);
                    break;
                case 2:
                    greenBlock.setPosition(
                        (x + curr->pos.x) * BlockSize + FieldOffsetX,
                        (y + curr->pos.y) * BlockSize + FieldOffsetY);
                    window.draw(greenBlock);
                    break;
                case 3:
                    orangeBlock.setPosition(
                        (x + curr->pos.x) * BlockSize + FieldOffsetX,
                        (y + curr->pos.y) * BlockSize + FieldOffsetY);
                    window.draw(orangeBlock);
                    break;
                case 4:
                    redBlock.setPosition(
                        (x + curr->pos.x) * BlockSize + FieldOffsetX,
                        (y + curr->pos.y) * BlockSize + FieldOffsetY);
                    window.draw(redBlock);
                    break;
                case 5:
                    yellowBlock.setPosition(
                        (x + curr->pos.x) * BlockSize + FieldOffsetX,
                        (y + curr->pos.y) * BlockSize + FieldOffsetY);
                    window.draw(yellowBlock);
                    break;
                default:
                    break;
                }
            }
        }

        if (clock.getElapsedTime().asSeconds() > delay)
        {
            curr->pos.y += 1;
            if (checkCollisionY(curr, field))
            {
                curr->pos.y -= 1;
                addToField(curr, field);
                completeLines = removeCompleteLines(field);
                if (completeLines > 0)
                    gameState.score += completeLines * 100;

                curr = next;
                curr->pos.x = SpawnPoint.x;
                curr->pos.y = SpawnPoint.y;
                next = getRandTetromino(tl);
                gameState.gameover = checkCollisionY(curr, field);
            }
            clock.restart();
        }

        window.display();
    }

    delete tf;
    delete tl[0];
    delete tl[1];
    delete tl[2];
    delete tl[3];
    delete tl[4];

    return EXIT_SUCCESS;
}

Tetromino *getRandTetromino(Tetromino *list[])
{
    return list[rand() % TetrominoNum];
}

bool checkCollisionX(Tetromino *tetro, int field[FieldY][FieldX])
{
    for (int y = 0; y < tetro->getHeight(); y++)
    {
        for (int x = 0; x < tetro->getWidth(); x++)
        {
            if (tetro->getValue(x, y) == 0)
                continue;
            int blockX = tetro->pos.x + x;
            int blockY = tetro->pos.y + y;
            if (blockX < 0 || blockX >= FieldX)
                return true;
            if (field[blockY][blockX] != 0)
                return true;
        }
    }
    return false;
}

bool checkCollisionY(Tetromino *tetro, int field[FieldY][FieldX])
{
    for (int y = tetro->getHeight() - 1; y >= 0; y--)
    {
        for (int x = tetro->getWidth() - 1; x >= 0; x--)
        {
            if (tetro->getValue(x, y) == 0)
                continue;
            int blockX = tetro->pos.x + x;
            int blockY = tetro->pos.y + y;
            if (blockY >= FieldY || field[blockY][blockX] != 0)
                return true;
        }
    }
    return false;
}

void addToField(Tetromino *tetro, int field[FieldY][FieldX])
{
    for (int y = 0; y < tetro->getHeight(); y++)
    {
        for (int x = 0; x < tetro->getWidth(); x++)
        {
            if (tetro->getValue(x, y) == 0)
                continue;
            int blockX = tetro->pos.x + x;
            int blockY = tetro->pos.y + y;
            field[blockY][blockX] = tetro->getValue(x, y);
        }
    }
}

int removeCompleteLines(int field[FieldY][FieldX])
{
    int count = 0;
    for (int y = FieldY - 1; y >= 0; y--)
    {
        bool isComplete = true;
        for (int i = 0; i < FieldX; i++)
        {
            if (field[y][i] == 0)
            {
                isComplete = false;
                break;
            }
        }

        if (isComplete)
        {
            for (int i = 0; i < FieldX; i++)
                field[y][i] = 0;
            count++;
        }
    }
    return count;
}

void pushFieldDown(int field[FieldY][FieldX])
{
    list<int> emptyRows;
    for (int y = FieldY - 1; y >= 0; y--)
    {
        bool isEmpty = true;
        for (int i = 0; i < FieldX; i++)
        {
            if (field[y][i] != 0)
            {
                isEmpty = false;
                break;
            }
        }

        if (isEmpty)
        {
            emptyRows.push_back(y);
        }
        else if (emptyRows.size() > 0)
        {
            int row = emptyRows.front();
            emptyRows.pop_front();

            // copy current row to first empty row
            // and clear current row
            for (int i = 0; i < FieldX; i++)
            {
                field[row][i] = field[y][i];
                field[y][i] = 0;
            }
        }
    }
}