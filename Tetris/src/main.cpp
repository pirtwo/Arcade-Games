#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <vector>
#include <memory>
#include "Field.h"
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
const sf::Vector2i NextShowPoint(420, 300);
float Scale = float(BlockSize) / TextureSize;

struct State
{
    bool pause = false;
    bool gameover = false;
    int score = 0;
    int lines = 0;
} gameState;

unique_ptr<Tetromino> getRandTetromino(TetrominoFactory &factory);

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 700), "Tetris");
    window.setFramerateLimit(60);

    // load assets
    sf::Font font;
    vector<sf::Texture *> textures;
    textures.push_back(new sf::Texture());
    textures.push_back(new sf::Texture());
    textures.push_back(new sf::Texture());
    textures.push_back(new sf::Texture());
    textures.push_back(new sf::Texture());
    textures.push_back(new sf::Texture());

    if (!font.loadFromFile("./assets/FredokaOne-Regular.ttf") ||
        !textures[0]->loadFromFile("./assets/tileGrey.png") ||
        !textures[1]->loadFromFile("./assets/tileBlue.png") ||
        !textures[2]->loadFromFile("./assets/tileGreen.png") ||
        !textures[3]->loadFromFile("./assets/tileOrange.png") ||
        !textures[4]->loadFromFile("./assets/tileRed.png") ||
        !textures[5]->loadFromFile("./assets/tileYellow.png"))
        return EXIT_FAILURE;

    srand(time(NULL));
    sf::Clock clock;
    float delay = 0.5;

    int completeLines = 0;
    unique_ptr<Field> field(new Field(11, 20, Scale, textures));
    unique_ptr<TetrominoFactory> tf(new TetrominoFactory(textures));
    unique_ptr<Tetromino> currTetromino;
    unique_ptr<Tetromino> nextTetromino;

    currTetromino = getRandTetromino(*tf);
    nextTetromino = getRandTetromino(*tf);
    currTetromino->pos.x = SpawnPoint.x;
    currTetromino->pos.y = SpawnPoint.y;
    nextTetromino->pos.x = 0;
    nextTetromino->pos.y = 0;
    nextTetromino->offset = NextShowPoint;

    sf::Color textColor(238, 238, 238, 255);
    // player stats text
    char playerStat[100];
    sf::Text statText;
    statText.setFont(font);
    statText.setCharacterSize(20);
    statText.setFillColor(textColor);
    statText.setPosition(420, 50);

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
                    currTetromino->pos.x -= 1;
                    if (field->collisionX(*currTetromino))
                        currTetromino->pos.x += 1;
                }
                if (e.key.code == sf::Keyboard::Key::Right)
                {
                    currTetromino->pos.x += 1;
                    if (field->collisionX(*currTetromino))
                        currTetromino->pos.x -= 1;
                }
                if (e.key.code == sf::Keyboard::Key::Down)
                {
                    delay = 0.5;
                }
                if (e.key.code == sf::Keyboard::Key::Space)
                {
                    currTetromino->rotateRight();
                    if (field->collisionX(*currTetromino))
                        currTetromino->rotateLeft();
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

                    currTetromino = getRandTetromino(*tf);
                    currTetromino->pos.x = SpawnPoint.x;
                    currTetromino->pos.y = SpawnPoint.y;

                    nextTetromino = getRandTetromino(*tf);
                    nextTetromino->pos.x = 0;
                    nextTetromino->pos.y = 0;
                    nextTetromino->offset = NextShowPoint;

                    field->clear();
                }
            }
        }

        // ======= update ========
        snprintf(
            playerStat, 100,
            "Statistics:\r\nScore: %d\r\nLines: %d\r\n%s\r\n%s",
            gameState.score,
            gameState.lines,
            gameState.pause ? "PAUSE" : "",
            gameState.gameover ? "GAMEOVER" : "");
        statText.setString(playerStat);

        if (completeLines > 0)
            field->pushDown();

        if (!gameState.pause &&
            !gameState.gameover &&
            clock.getElapsedTime().asSeconds() > delay)
        {
            currTetromino->pos.y += 1;
            if (field->collisionY(*currTetromino))
            {
                currTetromino->pos.y -= 1;
                field->addTetromino(*currTetromino);
                completeLines = field->removeCompleteRows();
                if (completeLines > 0)
                {
                    gameState.score += completeLines * 100;
                    gameState.lines += completeLines;
                }

                currTetromino = move(nextTetromino);
                currTetromino->pos.x = SpawnPoint.x;
                currTetromino->pos.y = SpawnPoint.y;
                currTetromino->offset = sf::Vector2i(0, 0);

                nextTetromino = getRandTetromino(*tf);
                nextTetromino->pos.x = 0;
                nextTetromino->pos.y = 0;
                nextTetromino->offset = NextShowPoint;

                gameState.gameover = field->collisionY(*currTetromino);
            }
            clock.restart();
        }

        // ======= draw ========
        window.clear(sf::Color(57, 62, 70, 255));
        window.draw(statText);
        window.draw(helpText);
        window.draw(*field);
        window.draw(*currTetromino);
        window.draw(*nextTetromino);
        window.display();
    }

    for (auto texture : textures)
        delete texture;

    return EXIT_SUCCESS;
}

/*
    returns a random Tetromino
*/
unique_ptr<Tetromino> getRandTetromino(TetrominoFactory &factory)
{
    auto t = factory.create(
        static_cast<TetrominoType>(rand() % TetrominoNum + 1), Scale);

    // mirror Tetromino with 50% chance
    if (rand() <= RAND_MAX / 2)
        t->mirrorX();

    return t;
}
