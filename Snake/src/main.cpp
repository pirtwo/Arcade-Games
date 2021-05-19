#include <SFML/Graphics.hpp>
#include <stdlib.h>

using namespace std;

// number of grid columns
int X = 30;

// number of grid rows
int Y = 30;

// size of the grid cell
int Size = 16;

// grid offset X from top left of window
int OffsetX = 0;

// grid offset Y from top left of window
int OffsetY = 0;

// snake current length
int SnakeLength = 3;

// snake current direction
int SnakeDir = 0;

struct Snake
{
    int x, y;
} snake[100];

struct Food
{
    int x, y;
} food;

void update();

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "SNAKE");
    window.setFramerateLimit(60);

    srand(time(NULL));

    float delay = 0.1;
    sf::Clock clock;

    sf::Font font;
    sf::Texture txt1, txt2, txt3;

    if (
        !font.loadFromFile("./assets/OpenSans-Regular.ttf") ||
        !txt1.loadFromFile("./assets/tileRed_01.png") ||
        !txt2.loadFromFile("./assets/tileGrey_01.png") ||
        !txt3.loadFromFile("./assets/tileGreen_01.png"))
    {
        return EXIT_FAILURE;
    }

    float Scale = 0.125;
    // sprite food
    sf::Sprite sp1;
    // sprite tile
    sf::Sprite sp2;
    // sprite snake
    sf::Sprite sp3;
    sp1.setTexture(txt1);
    sp2.setTexture(txt2);
    sp3.setTexture(txt3);
    sp1.setScale(Scale, Scale);
    sp2.setScale(Scale, Scale);
    sp3.setScale(Scale, Scale);

    snake[0].x = 3;
    snake[0].y = 1;
    snake[1].x = 2;
    snake[1].y = 1;
    snake[2].x = 1;
    snake[2].y = 1;

    food.x = 10;
    food.y = 10;

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            SnakeDir = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            SnakeDir = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            SnakeDir = 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            SnakeDir = 3;

        window.clear(sf::Color::Black);

        // draw tiles
        for (int i = 0; i < Y; i++)
        {
            for (int j = 0; j < X; j++)
            {
                sp2.setPosition(j * Size + OffsetX, i * Size + OffsetY);
                window.draw(sp2);
            }
        }

        // draw snake
        for (int i = 0; i < SnakeLength; i++)
        {
            sp3.setPosition(snake[i].x * Size + OffsetX, snake[i].y * Size + OffsetY);
            window.draw(sp3);
        }

        // draw food
        sp1.setPosition(food.x * Size, food.y * Size);
        window.draw(sp1);

        if (clock.getElapsedTime().asSeconds() > delay)
        {
            update();
            clock.restart();
        }

        window.display();
    }

    return EXIT_SUCCESS;
}

void update()
{
    if (snake[0].x == food.x && snake[0].y == food.y)
    {
        SnakeLength++;
        food.x = rand() % Size;
        food.y = rand() % Size;
    }

    for (int i = SnakeLength - 1; i > 0; i--)
    {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    switch (SnakeDir)
    {
    case 0:
        // move left
        snake[0].x = snake[0].x - 1 < 0 ? X - 1 : snake[0].x - 1;
        break;
    case 1:
        // move right
        snake[0].x = snake[0].x + 1 >= X ? 0 : snake[0].x + 1;
        break;
    case 2:
        // move up
        snake[0].y = snake[0].y - 1 < 0 ? Y - 1 : snake[0].y - 1;
        break;
    case 3:
        // move down
        snake[0].y = snake[0].y + 1 >= Y ? 0 : snake[0].y + 1;
        break;
    default:
        break;
    }
}