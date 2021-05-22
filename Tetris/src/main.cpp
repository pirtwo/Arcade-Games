#include <SFML/Graphics.hpp>
#include <stdlib.h>
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

void update(Tetromino *tetro, int field[FieldY][FieldX]);
void copyTetro(Tetromino *tetro, int field[FieldY][FieldX]);
bool checkCollision(Tetromino *tetro, int field[FieldY][FieldX]);

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

    int field[FieldY][FieldX] = {0};
    Tetromino *curr;
    Tetromino *next;
    TetrominoFactory *tetroFactory = new TetrominoFactory();

    curr = tetroFactory->create(
        static_cast<TetrominoType>(rand() % TetrominoNum + 1));
    next = tetroFactory->create(
        static_cast<TetrominoType>(rand() % TetrominoNum + 1));
    curr->pos.x = 0;
    curr->pos.y = 0;

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyReleased)
            {
                if (e.key.code == sf::Keyboard::Key::Left)
                    curr->pos.x -= curr->pos.x > 0 ? 1 : 0;
                if (e.key.code == sf::Keyboard::Key::Right)
                    curr->pos.x += curr->pos.x + curr->getWidth() < FieldX ? 1 : 0;
            }
        }

        window.clear();

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
            update(curr, field);
            clock.restart();
        }

        window.display();
    }

    delete tetroFactory;

    return EXIT_SUCCESS;
}

void update(Tetromino *tetro, int field[FieldY][FieldX])
{
    if (checkCollision(tetro, field))
        copyTetro(tetro, field);
    else
        tetro->pos.y += 1;
}

bool checkCollision(Tetromino *tetro, int field[FieldY][FieldX])
{
    for (int y = 0; y < tetro->getHeight(); y++)
    {
        for (int x = 0; x < tetro->getWidth(); x++)
        {
            if (tetro->getValue(x, y) == 0)
                continue;
            int blockX = tetro->pos.x + x;
            int blockY = tetro->pos.y + y;
            if (blockY >= FieldY || field[blockY + 1][blockX] != 0)
                return true;
        }
    }
    return false;
}

void copyTetro(Tetromino *tetro, int field[FieldY][FieldX])
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