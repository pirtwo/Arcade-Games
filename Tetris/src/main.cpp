#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <list>
#include "Tetromino.h"
#include "TetrominoFactory.h"

using namespace std;

const int FieldX = 11;
const int FieldY = 20;
const int FieldOffsetX = 0;
const int FieldOffsetY = 0;
const int TetrominoNum = 5; // number of Tetrominos in game
const int BlockSize = 35;   // size of each block in pixel
const int TextureSize = 88;
const sf::Vector2i SpawnPoint(4, 0);

struct State
{
    bool pause = false;
    bool gameover = false;
    int score = 0;
    int lines = 0;
} gameState;

int removeCompleteLines(int field[FieldY][FieldX]);
void pushFieldDown(int field[FieldY][FieldX]);
void addToField(Tetromino *tetro, int field[FieldY][FieldX]);
bool checkCollisionX(Tetromino *tetro, int field[FieldY][FieldX]);
bool checkCollisionY(Tetromino *tetro, int field[FieldY][FieldX]);
Tetromino *getRandTetromino(TetrominoFactory *factory);
void clearField(int field[FieldY][FieldX]);

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 700), "Tetris");
    window.setFramerateLimit(60);

    // load assets
    sf::Font font;
    sf::Texture tileGreyTxt,
        tileBlueTxt,
        tileGreenTxt,
        tileOrangeTxt,
        tileRedTxt,
        tileYellowTxt;
    if (!font.loadFromFile("./assets/FredokaOne-Regular.ttf") ||
        !tileGreyTxt.loadFromFile("./assets/tileGrey.png") ||
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
    TetrominoFactory *tf = new TetrominoFactory();

    curr = getRandTetromino(tf);
    next = getRandTetromino(tf);
    curr->pos.x = SpawnPoint.x;
    curr->pos.y = SpawnPoint.y;
    next->pos.x = 0;
    next->pos.y = 0;

    sf::Color textColor(238, 238, 238, 255);
    // player stats text
    char playerStat[100];
    sf::Text statText;
    statText.setFont(font);
    statText.setCharacterSize(20);
    statText.setFillColor(textColor);
    statText.setPosition(420, 50);

    // game pause text
    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setCharacterSize(20);
    pauseText.setFillColor(textColor);
    pauseText.setString("PAUSE");
    pauseText.setPosition(420, 125);

    // gameover text
    sf::Text gameoverText;
    gameoverText.setFont(font);
    gameoverText.setCharacterSize(20);
    gameoverText.setFillColor(textColor);
    gameoverText.setString("GAMEOVER");
    gameoverText.setPosition(420, 125);

    // game keys help text
    sf::Text helpText;
    helpText.setFont(font);
    helpText.setFillColor(textColor);
    helpText.setString(
        "Keys:\r\n[P] Pause\r\n[R] Restart\r\n[Space] Rotate\r\n[Arrows] Move");
    helpText.setCharacterSize(20);
    helpText.setPosition(420, 550);

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
                {
                    delay = 0.5;
                }
                if (e.key.code == sf::Keyboard::Key::Space)
                {
                    curr->rotateRight();
                    if (checkCollisionX(curr, field))
                        curr->rotateLeft();
                }
                if (e.key.code == sf::Keyboard::Key::P)
                {
                    gameState.pause = !gameState.pause;
                }
                if (e.key.code == sf::Keyboard::Key::R)
                {
                    gameState.pause = false;
                    gameState.gameover = false;
                    gameState.score = 0;

                    curr = getRandTetromino(tf);
                    curr->pos.x = SpawnPoint.x;
                    curr->pos.y = SpawnPoint.y;

                    next = getRandTetromino(tf);
                    next->pos.x = 0;
                    next->pos.y = 0;

                    clearField(field);
                }
            }
        }

        window.clear(sf::Color(57, 62, 70, 255));

        // show player statistics
        snprintf(
            playerStat, 100,
            "Statistics:\r\nScore: %d\r\nLines: %d",
            gameState.score,
            gameState.lines);
        statText.setString(playerStat);
        window.draw(statText);

        // show game status text
        if (gameState.pause)
            window.draw(pauseText);
        if (gameState.gameover)
            window.draw(gameoverText);

        window.draw(helpText);

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

        // draw current Tetromino
        if (!gameState.gameover)
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

        // draw next Tetromino
        for (int y = 0; y < next->getHeight(); y++)
        {
            for (int x = 0; x < next->getWidth(); x++)
            {
                switch (next->getValue(x, y))
                {
                case 1:
                    blueBlock.setPosition(
                        (x + next->pos.x) * BlockSize + 420,
                        (y + next->pos.y) * BlockSize + 300);
                    window.draw(blueBlock);
                    break;
                case 2:
                    greenBlock.setPosition(
                        (x + next->pos.x) * BlockSize + 420,
                        (y + next->pos.y) * BlockSize + 300);
                    window.draw(greenBlock);
                    break;
                case 3:
                    orangeBlock.setPosition(
                        (x + next->pos.x) * BlockSize + 420,
                        (y + next->pos.y) * BlockSize + 300);
                    window.draw(orangeBlock);
                    break;
                case 4:
                    redBlock.setPosition(
                        (x + next->pos.x) * BlockSize + 420,
                        (y + next->pos.y) * BlockSize + 300);
                    window.draw(redBlock);
                    break;
                case 5:
                    yellowBlock.setPosition(
                        (x + next->pos.x) * BlockSize + 420,
                        (y + next->pos.y) * BlockSize + 300);
                    window.draw(yellowBlock);
                    break;
                default:
                    break;
                }
            }
        }

        if (!gameState.pause &&
            !gameState.gameover &&
            clock.getElapsedTime().asSeconds() > delay)
        {
            curr->pos.y += 1;
            if (checkCollisionY(curr, field))
            {
                curr->pos.y -= 1;
                addToField(curr, field);
                completeLines = removeCompleteLines(field);
                if (completeLines > 0)
                {
                    gameState.score += completeLines * 100;
                    gameState.lines += completeLines;
                }

                delete curr;
                curr = next;
                curr->pos.x = SpawnPoint.x;
                curr->pos.y = SpawnPoint.y;

                next = getRandTetromino(tf);
                next->pos.x = 0;
                next->pos.y = 0;

                gameState.gameover = checkCollisionY(curr, field);
            }
            clock.restart();
        }

        window.display();
    }

    delete tf;
    delete curr;
    delete next;

    return EXIT_SUCCESS;
}

/*
    returns a random Tetromino
*/
Tetromino *getRandTetromino(TetrominoFactory *factory)
{
    auto t = factory->create(
        static_cast<TetrominoType>(rand() % TetrominoNum + 1));

    // mirror Tetromino with 50% chance
    if (rand() <= RAND_MAX / 2)
        t->mirrorX();

    return t;
}

/*
    checks for collision on X axis
*/
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

/*
    checks for collision on Y axis
*/
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

/*
    Add the Tetromino to the field
*/
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

/*
    Removes complete rows from the field and 
    returns the number of deleted rows
*/
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

/*
    fills empty rows with non empty rows above (if exist)
*/
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

            for (int i = 0; i < FieldX; i++)
            {
                field[row][i] = field[y][i];
                field[y][i] = 0;
            }
        }
    }
}

/*
    sets all blocks of the field to zero
*/
void clearField(int field[FieldY][FieldX])
{
    for (int y = 0; y < FieldY; y++)
    {
        for (int x = 0; x < FieldX; x++)
        {
            field[y][x] = 0;
        }
    }
}
