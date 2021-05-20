#include <SFML/Graphics.hpp>
#include <stdlib.h>

using namespace std;

// number of grid columns
const int X = 20;

// number of grid rows
const int Y = 20;

// size of the grid cell (pixel)
const int Size = 25;

// grid offset X from top left of window (pxel)
const int OffsetX = 100;

// grid offset Y from top left of window (pixel)
const int OffsetY = 100;

// player max health
const int HealthSlotNum = 5;

struct Player
{
    int score, health, boost;
} player;

struct Snake
{
    int len, dir;
    sf::Vector2i body[100];
} snake;

struct Food
{
    int x, y;
} food;

bool pause = false;
bool gameover = false;

void init();
void update();

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 700), "SNAKE");
    window.setFramerateLimit(60);

    srand(time(NULL));

    float delay = 0.1;
    sf::Clock clock;

    sf::Font font;
    sf::Texture txt1, txt2, txt3;

    if (
        !font.loadFromFile("./assets/OpenSans-Regular.ttf") ||
        !txt1.loadFromFile("./assets/tileRed_01.png") ||
        !txt2.loadFromFile("./assets/tileGreen_01.png") ||
        !txt3.loadFromFile("./assets/tileGrey_01.png"))
    {
        return EXIT_FAILURE;
    }

    float textureSize = 128;
    float scale = Size / textureSize;
    sf::Sprite sp1;
    sf::Sprite sp2;
    sf::Sprite sp3;
    sp1.setTexture(txt1);
    sp2.setTexture(txt2);
    sp3.setTexture(txt3);
    sp1.setScale(scale, scale);
    sp2.setScale(scale, scale);
    sp3.setScale(scale, scale);

    // field border
    sf::RectangleShape border(sf::Vector2f(X * Size, Y * Size));
    border.setFillColor(sf::Color(0, 0, 0, 0));
    border.setOutlineThickness(2);
    border.setOutlineColor(sf::Color(200, 200, 200, 255));
    border.setPosition(OffsetX, OffsetY);

    // pause banner
    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setCharacterSize(30);
    pauseText.setFillColor(sf::Color::Red);
    pauseText.setString("PAUSE");
    pauseText.setPosition(
        window.getSize().x / 2 - pauseText.getLocalBounds().width / 2,
        window.getSize().y / 2 - pauseText.getLocalBounds().height / 2);

    // gameover banner
    sf::Text gameoverText;
    gameoverText.setFont(font);
    gameoverText.setCharacterSize(30);
    gameoverText.setFillColor(sf::Color::Red);
    gameoverText.setString("GAME OVER");
    gameoverText.setPosition(
        window.getSize().x / 2 - gameoverText.getLocalBounds().width / 2,
        window.getSize().y / 2 - gameoverText.getLocalBounds().height / 2);

    // score text
    char scoreStr[100];
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(OffsetX, OffsetY - 60.f);

    // health text
    char healthStr[100];
    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setFillColor(sf::Color::Black);
    healthText.setPosition(X * Size + OffsetX - 130, OffsetY - 60.f);

    // game info text
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(12);
    infoText.setFillColor(sf::Color::Black);
    infoText.setString("[W-A-S-D]:Move  [P]:Pause   [R]:Restart");
    infoText.setPosition(
        window.getSize().x / 2 - infoText.getLocalBounds().width / 2,
        Y * Size + OffsetY + 40.f);

    init();

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyReleased)
            {
                if (e.key.code == sf::Keyboard::P)
                    pause = !pause;
                if (e.key.code == sf::Keyboard::R)
                    init();
            }
        }

        // check for keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            snake.dir = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            snake.dir = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            snake.dir = 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            snake.dir = 3;

        window.clear(sf::Color(200, 200, 200, 255));

        // show player score
        snprintf(scoreStr, 100, "SCORE: %d (x%d)", player.score, player.boost);
        scoreText.setString(scoreStr);
        window.draw(scoreText);

        // show player health
        snprintf(
            healthStr, 100, "HP: %s%s",
            string(player.health, '@').c_str(),
            string(HealthSlotNum - player.health, '-').c_str());
        healthText.setString(healthStr);
        window.draw(healthText);

        // show info
        window.draw(infoText);

        // draw field border
        window.draw(border);

        // draw tiles
        for (int i = 0; i < Y; i++)
        {
            for (int j = 0; j < X; j++)
            {
                sp3.setPosition(j * Size + OffsetX, i * Size + OffsetY);
                window.draw(sp3);
            }
        }

        // draw snake
        for (int i = 0; i < snake.len; i++)
        {
            sp2.setPosition(
                snake.body[i].x * Size + OffsetX,
                snake.body[i].y * Size + OffsetY);
            window.draw(sp2);
        }

        // draw food
        sp1.setPosition(food.x * Size + OffsetX, food.y * Size + OffsetY);
        window.draw(sp1);

        if (!pause &&
            !gameover &&
            clock.getElapsedTime().asSeconds() > delay)
        {
            update();
            clock.restart();
        }

        if (pause)
            window.draw(pauseText);
        if (gameover)
            window.draw(gameoverText);

        window.display();
    }

    return EXIT_SUCCESS;
}

void init()
{
    pause = false;
    gameover = false;

    // init player
    player.score = 0;
    player.boost = 1;
    player.health = HealthSlotNum;

    // init food
    food.x = 10;
    food.y = 10;

    // init snake
    snake.dir = 1;
    snake.len = 3;
    snake.body[0].x = 3;
    snake.body[0].y = 1;
    snake.body[1].x = 2;
    snake.body[1].y = 1;
    snake.body[2].x = 1;
    snake.body[2].y = 1;
}

void update()
{
    // check body collision
    for (int i = 1; i < snake.len; i++)
    {
        if (
            snake.body[i].x == snake.body[0].x &&
            snake.body[i].y == snake.body[0].y)
        {
            snake.len = i < 3 ? 3 : i;
            player.boost = snake.len / 10 + 1;
            player.health--;
            if (player.health == 0)
                gameover = true;
        }
    }

    // check food collision
    if (snake.body[0].x == food.x && snake.body[0].y == food.y)
    {
        snake.len += snake.len < 100 ? 1 : 0;

        // update player score
        player.boost = snake.len / 10 + 1;
        player.score += 10 * player.boost;

        // update food position
        food.x = rand() % X;
        food.y = rand() % Y;
    }

    // move snake forwrd
    for (int i = snake.len - 1; i > 0; i--)
    {
        snake.body[i].x = snake.body[i - 1].x;
        snake.body[i].y = snake.body[i - 1].y;
    }

    // update snake direction
    switch (snake.dir)
    {
    case 0:
        // move left
        snake.body[0].x =
            snake.body[0].x - 1 < 0 ? X - 1 : snake.body[0].x - 1;
        break;
    case 1:
        // move right
        snake.body[0].x =
            snake.body[0].x + 1 >= X ? 0 : snake.body[0].x + 1;
        break;
    case 2:
        // move up
        snake.body[0].y =
            snake.body[0].y - 1 < 0 ? Y - 1 : snake.body[0].y - 1;
        break;
    case 3:
        // move down
        snake.body[0].y =
            snake.body[0].y + 1 >= Y ? 0 : snake.body[0].y + 1;
        break;
    default:
        break;
    }
}