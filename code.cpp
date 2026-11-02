#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>

#define CELL_SIZE 20
#define X_SIZE 40
#define Y_SIZE 30
#define WINDOW_WIDTH (CELL_SIZE * X_SIZE)
#define WINDOW_HEIGHT (CELL_SIZE * Y_SIZE)

// dir
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

int dir_table[4][2] = {
    {-1, 0}, // LEFT
    {1, 0},  // RIGHT
    {0, -1}, // UP
    {0, 1}   // DOWN
};

typedef sf::Vector2i Point;

class Snake;

class Food
{
public:
    Point GetPos() const;

    void CreateFood();

    void DrawFood(sf::RenderWindow &window);

    bool CheckIsOnSnake(const Snake &snake);

private:
    Point pos;
};

class Snake
{
public:
    Snake();

    std::vector<Point> GetSnakeBody() const;

    void Move();

    void DrawSnake(sf::RenderWindow &window);

    void EatFood();

    void SetDir(int d);

    int GetDir() const;

    void ChangeDir(sf::Event event);

    void SetCanChangeDir();

private:
    std::vector<Point> body;
    int dir;
    int next_dir;
    bool can_change_dir;
};

Snake snake;
Food food;
Point snake_tail;
sf::Texture snake_texture;
sf::Sprite snake_sprite;
sf::Texture food_texture;
sf::Sprite food_sprite;

void InitGame()
{
    food.CreateFood();

    if (!snake_texture.loadFromFile("../snake.bmp"))
    {
        std::cout << "Unable load bitmap." << std::endl;
    }

    snake_sprite.setTexture(snake_texture);
    snake_sprite.setScale(CELL_SIZE * 1.0f / 48, CELL_SIZE * 1.0f / 48);

    if (!food_texture.loadFromFile("../food.bmp"))
    {
        std::cout << "Unable load bitmap." << std::endl;
    }

    food_sprite.setTexture(food_texture);
    food_sprite.setScale(CELL_SIZE * 1.0f / 48, CELL_SIZE * 1.0f / 48);
}

void Food::CreateFood()
{
    do
    {
        pos.x = std::rand() % X_SIZE;
        pos.y = std::rand() % Y_SIZE;
    } while (CheckIsOnSnake(snake));
}

Point Food::GetPos() const
{
    return pos;
}

void Food::DrawFood(sf::RenderWindow &window)
{
    food_sprite.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
    window.draw(food_sprite);
}

std::vector<Point> Snake::GetSnakeBody() const
{
    return body;
}

bool Food::CheckIsOnSnake(const Snake &snake)
{
    auto snake_body = snake.GetSnakeBody();
    for (auto &item : snake_body)
    {
        if (item.x == pos.x && item.y == pos.y)
        {
            return true;
        }
    }

    return false;
}

Snake::Snake()
{
    dir = RIGHT;
    next_dir = RIGHT;
    can_change_dir = true;
    body.push_back((Point){X_SIZE / 2 + 1, Y_SIZE / 2});
    body.push_back((Point){X_SIZE / 2, Y_SIZE / 2});
    body.push_back((Point){X_SIZE / 2 - 1, Y_SIZE / 2});
}

void Snake::DrawSnake(sf::RenderWindow &window)
{
    for (auto &item : body)
    {
        snake_sprite.setPosition(item.x * CELL_SIZE, item.y * CELL_SIZE);
        window.draw(snake_sprite);
    }
}

void DrawGameBorad(sf::RenderWindow &window)
{
    snake.DrawSnake(window);
    food.DrawFood(window);
}

void Snake::Move()
{
    dir = next_dir;
    snake_tail.x = body.back().x;
    snake_tail.y = body.back().y;

    for (int i = body.size() - 1; i >= 1; i--)
    {
        body[i].x = body[i - 1].x;
        body[i].y = body[i - 1].y;
    }

    body[0].x += dir_table[dir][0];
    body[0].y += dir_table[dir][1];
}

void Snake::SetCanChangeDir()
{
    can_change_dir = true;
}

void Snake::EatFood()
{
    Point food_pos = food.GetPos();
    if (food_pos == body[0])
    {
        body.push_back(snake_tail);
        food.CreateFood();
    }
}

void Snake::SetDir(int d)
{
    dir = d;
}

int Snake::GetDir() const
{
    return dir;
}

void Snake::ChangeDir(sf::Event event)
{
    if (can_change_dir == false)
    {
        return;
    }

    switch (event.key.code)
    {
    case sf::Keyboard::Left:
    case sf::Keyboard::A:
        if (snake.dir != RIGHT)
        {
            next_dir = LEFT;
            can_change_dir = false;
        }
        break;
    case sf::Keyboard::Right:
    case sf::Keyboard::D:
        if (snake.dir != LEFT)
        {
            next_dir = RIGHT;
            can_change_dir = false;
        }
        break;
    case sf::Keyboard::Up:
    case sf::Keyboard::W:
        if (snake.dir != DOWN)
        {
            next_dir = UP;
            can_change_dir = false;
        }
        break;
    case sf::Keyboard::Down:
    case sf::Keyboard::S:
        if (snake.dir != UP)
        {
            next_dir = DOWN;
            can_change_dir = false;
        }
        break;

    default:
        break;
    }
}

bool CheckIfOver()
{
    // hit wall
    if (snake.GetSnakeBody()[0].x < 0 || snake.GetSnakeBody()[0].x >= X_SIZE ||
        snake.GetSnakeBody()[0].y < 0 || snake.GetSnakeBody()[0].y >= Y_SIZE)
    {
        return true;
    }

    // hit self
    auto snake_body = snake.GetSnakeBody();
    auto head = snake_body[0];

    for (int i = 1; i < snake_body.size(); i++)
    {
        if (head == snake_body[i])
        {
            return true;
        }
    }

    return false;
}

int main(int argc, char **argv)
{

    std::srand((unsigned)time(NULL));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake");

    window.setPosition(sf::Vector2i(100, 100));

    InitGame();

    sf::Clock clock;
    const sf::Time move_interval = sf::milliseconds(200);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                snake.ChangeDir(event);
            }
        }

        if (clock.getElapsedTime() >= move_interval)
        {
            clock.restart();
            snake.Move();

            snake.SetCanChangeDir();

            if (CheckIfOver())
            {
                window.close();
            }
            snake.EatFood();
        }

        // clear window
        window.clear(sf::Color::White);
        // draw something
        DrawGameBorad(window);
        window.display();
    }
}