#include <SFML/Graphics.hpp>
#include <stdlib.h>

using namespace std;

const int FieldX = 11;
const int FieldY = 20;
const int FieldOffsetX = 100;
const int FieldOffsetY = 100;
const int TetrominoNum = 5; // number of Tetrominos in game
const int BlockSize = 20;   // size of each block in pixel
const int TextureSize = 88;

struct Key
{
    bool isDown = false;
} arrowLeft, arrowRight, arrowUp, arrowDown;

struct Tetromino
{
    sf::Vector2i pos;
    int value[4][4] = {0};
};

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
    Tetromino *tetros[TetrominoNum];
    Tetromino *curr;
    Tetromino *next;

    Tetromino straight;
    straight.value[1][0] = 1;
    straight.value[1][1] = 1;
    straight.value[1][2] = 1;
    straight.value[1][3] = 1;
    tetros[0] = &straight;

    Tetromino square;
    square.value[1][1] = 2;
    square.value[1][2] = 2;
    square.value[2][1] = 2;
    square.value[2][2] = 2;
    tetros[1] = &square;

    Tetromino shapeT;
    shapeT.value[1][0] = 3;
    shapeT.value[1][1] = 3;
    shapeT.value[1][2] = 3;
    shapeT.value[2][1] = 3;
    tetros[2] = &shapeT;

    Tetromino shapeL;
    shapeL.value[0][0] = 4;
    shapeL.value[1][1] = 4;
    shapeL.value[1][2] = 4;
    shapeL.value[1][0] = 4;
    tetros[3] = &shapeL;

    Tetromino skew;
    skew.value[0][0] = 5;
    skew.value[0][1] = 5;
    skew.value[1][1] = 5;
    skew.value[1][2] = 5;
    tetros[4] = &skew;

    curr = tetros[rand() % TetrominoNum];
    next = tetros[rand() % TetrominoNum];
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
                    curr->pos.x += curr->pos.x + 4 < FieldX ? 1 : 0;
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
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                switch (curr->value[y][x])
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
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (tetro->value[y][x] == 0)
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
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (tetro->value[y][x] == 0)
                continue;
            int blockX = tetro->pos.x + x;
            int blockY = tetro->pos.y + y;
            field[blockY][blockX] = tetro->value[y][x];
        }
    }
}