#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;
#include <curses.h>
#include <string>
#include <numeric>

void loadScreen(std::vector<int> spaces, std::vector<std::pair<int, int>> snake, int size, int currentSpace)
{
    clear();
    for (int x = 0; x < size; ++x)
    {
        string output;

        for (int i = 0; i < size; ++i)
        {
            output += "  ";
        }

        for (int i = 0; i < snake.size(); i++)
        {
            bool isBlockAbove = false;
            for (int j = 0; j < snake.size(); j++)
            {
                if (std::get<0>(snake[j]) == std::get<0>(snake[i]) && std::get<1>(snake[i]) - 1 == std::get<1>(snake[j]))
                {
                    isBlockAbove = true;
                }
            }
            if (std::get<1>(snake[i]) == x && !isBlockAbove)
            {
                output[std::get<0>(snake[i]) * 2 + 1] = '_';
            }
        }

        printw(output.c_str());

        printw("\n");

        output = "";
        for (int i = 0; i < size; ++i)
        {
            output += " " + std::to_string(spaces[(x * size) + i]);
        }

        for (int y = 0; y < size; ++y)
        {
            for (int i = 0; i < snake.size(); i++)
            {
                if (std::get<1>(snake[i]) == x)
                {
                    if (std::get<0>(snake[i]) == y)
                    {
                        output[std::get<0>(snake[i]) * 2] = '|';
                        output[(std::get<0>(snake[i]) + 1) * 2] = '|';
                    }
                }
            }
        }

        printw(output.c_str());

        printw("\n");

        output = "";

        for (int i = 0; i < size; ++i)
        {
            output += "  ";
        }

        bool putDashes = false;

        for (int i = 0; i < snake.size(); i++)
        {
            if (std::get<1>(snake[i]) == x)
            {
                output[std::get<0>(snake[i]) * 2 + 1] = '-';
                putDashes = true;
            }
        }

        bool nextLineHasSquare = false;

        if (!putDashes)
        {
            for (int i = 0; i < snake.size(); i++)
            {
                if (std::get<1>(snake[i]) == x + 1)
                {
                    nextLineHasSquare = true;
                    break;
                }
            }
        }

        if (!nextLineHasSquare)
        {
            printw(output.c_str());
        }
    }
    refresh();
};

std::vector<int> generateSpaces(int size)
{
    std::vector<int> spaces(size * size);
    srand(time(0));
    int randomNum = (rand() % (size * size - 1)) + 1; // Picks a random square that doesn't include the first square
    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            if (x * size + y == randomNum)
            {
                spaces[x * size + y] = 1;
            }
            else
            {
                spaces[x * size + y] = 0;
            }
        }
    }
    return spaces;
}

std::vector<std::pair<int, int>> generateSnake(int snakeSize)
{
    std::vector<std::pair<int, int>> snake(snakeSize);

    for (int i = snakeSize - 1; i >= 0; i--)
    {
        snake[i] = std::make_pair(i, 0);
    }
    return snake;
}

std::vector<std::pair<int, int>> moveSnake(std::vector<std::pair<int, int>> snake, char direction, int size, std::vector<int> spaces)
{
    if (direction == 'l')
    {
        if (std::get<0>(snake[snake.size() - 1]) != 0)
        {
            std::pair<int, int> erasedObject = snake[0];
            snake.erase(snake.begin());
            snake.emplace_back(std::make_pair(std::get<0>(snake[snake.size() - 1]) - 1, std::get<1>(snake[snake.size()])));
            if (spaces[(size * std::get<1>(snake[snake.size() - 1])) + std::get<0>(snake[snake.size() - 1])] == 1)
            {
                snake.insert(snake.begin(), erasedObject);
            }
        }
    }
    else if (direction == 'r')
    {
        if (std::get<0>(snake[snake.size() - 1]) != size - 1)
        {
            std::pair<int, int> erasedObject = snake[0];
            snake.erase(snake.begin());
            snake.emplace_back(std::make_pair(std::get<0>(snake[snake.size() - 1]) + 1, std::get<1>(snake[snake.size()])));
            if (spaces[(size * std::get<1>(snake[snake.size() - 1])) + std::get<0>(snake[snake.size() - 1])] == 1)
            {
                snake.insert(snake.begin(), erasedObject);
            }
        }
    }
    else if (direction == 'u')
    {
        if (std::get<1>(snake[snake.size() - 1]) != 0)
        {
            std::pair<int, int> erasedObject = snake[0];
            snake.erase(snake.begin());
            snake.emplace_back(std::make_pair(std::get<0>(snake[snake.size() - 1]), std::get<1>(snake[snake.size()]) - 1));
            if (spaces[(size * std::get<1>(snake[snake.size() - 1])) + std::get<0>(snake[snake.size() - 1])] == 1)
            {
                snake.insert(snake.begin(), erasedObject);
            }
        }
    }
    else if (direction == 'd')
    {
        if (std::get<1>(snake[snake.size() - 1]) != size - 1)
        {
            std::pair<int, int> erasedObject = snake[0];
            snake.erase(snake.begin());
            snake.emplace_back(std::make_pair(std::get<0>(snake[snake.size() - 1]), std::get<1>(snake[snake.size()]) + 1));
            if (spaces[(size * std::get<1>(snake[snake.size() - 1])) + std::get<0>(snake[snake.size() - 1])] == 1)
            {
                snake.insert(snake.begin(), erasedObject);
            }
        }
    }

    return snake;
}

int main()
{
    int size = 9;
    int snakeStartingSize = 3;
    std::vector<std::pair<int, int>> snake = generateSnake(snakeStartingSize);

    std::vector<int> spaces = generateSpaces(size);

    initscr();
    cbreak();
    noecho();

    int currentSpace = 0;
    loadScreen(spaces, snake, size, currentSpace);

    nodelay(stdscr, TRUE);

    char key = getch();

    char direction = 'r';

    int loopNum = 0;

    do
    {
        bool updated = false;
        if ((key == 'w' || key == 'A') && (direction != 'd'))
        {
            direction = 'u';
        }
        else if ((key == 'd' || key == 'C') && (direction != 'l'))
        {
            direction = 'r';
        }
        else if ((key == 's' || key == 'B') && (direction != 'u'))
        {
            direction = 'd';
        }
        else if ((key == 'a' || key == 'D') && (direction != 'r'))
        {
            direction = 'l';
        }

        if (loopNum == 99)
        {
            std::vector<std::pair<int, int>> newSnake = moveSnake(snake, direction, size, spaces);

            if (newSnake != snake)
            {
                snake = newSnake;
                updated = true;
            }

            if (spaces[(size * std::get<1>(snake[snake.size() - 1])) + std::get<0>(snake[snake.size() - 1])] == 1)
            {
                spaces = generateSpaces(size);
            }

            loopNum = 0;
        }

        if (updated)
        {
            loadScreen(spaces, snake, size, currentSpace);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        loopNum++;

        key = getch();

    } while (key != 'q');
}