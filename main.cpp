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

void loadScreen(std::vector<int> spaces, int currentSpace, int size)
{
    if (currentSpace < size)
    {
        string output;
        for (int i = 0; i < currentSpace; ++i)
        {
            output += "  ";
        }
        output += " _";
        printw(output.c_str());
    }
    printw("\n");
    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            if ((x * size) + y == currentSpace)
            {
                printw(("|" + std::to_string(spaces[(x * size) + y]) + "|").c_str());
            }
            else if ((x * size) + y - 1 == currentSpace && y != 0)
            {
                printw(std::to_string(spaces[(x * size) + y]).c_str());
            }

            else
            {
                printw((" " + std::to_string(spaces[(x * size) + y])).c_str());
            }
        }

        if (currentSpace >= (x + 1) * size && currentSpace < (x + 2) * size && currentSpace >= size)
        {
            printw("\n");
            string output;
            for (int i = 0; i < currentSpace % size; ++i)
            {
                output += "  ";
            }
            output += " _";
            printw((output + "\n").c_str());
        }
        else if (currentSpace >= x * size && currentSpace < (x + 1) * size)
        {
            printw("\n");
            string output;
            for (int i = 0; i < currentSpace % size; ++i)
            {
                output += "  ";
            }
            output += " -";
            printw((output + "\n").c_str());
        }
        else
        {
            printw("\n\n");
        }
    }
    // clear();
    refresh();
};

std::vector<int> generateSpaces(int size, int randomNum)
{
    std::vector<int> spaces(size * size);
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

int main()
{
    int size = 9;
    int currentSpace = 0;
    std::vector<int> spaces;

    srand(time(0));
    int randomNum = (rand() % (size * size - 1)) + 1; // Picks a random square that doesn't include the first square

    spaces = generateSpaces(size, randomNum);

    initscr();
    cbreak();
    noecho();

    loadScreen(spaces, currentSpace, size);

    nodelay(stdscr, TRUE);

    char key = getch();
    clear();

    char direction = 'r';

    int loopNum = 0;
    int snakeSize = 1;
    do
    {
        if (key == 'w' || key == 'A')
        {
            direction = 'u';
        }
        else if (key == 'd' || key == 'C')
        {
            direction = 'r';
        }
        else if (key == 's' || key == 'B')
        {
            direction = 'd';
        }
        else if (key == 'a' || key == 'D')
        {
            direction = 'l';
        }

        if (loopNum == 4) {
            if (direction == 'l')
            {
                if (currentSpace % size != 0)
                {
                    currentSpace--;
                }
            }
            else if (direction == 'r')
            {
                if (currentSpace % size != size - 1)
                {
                    currentSpace++;
                }
            }
            else if (direction == 'u')
            {
                if (currentSpace > size - 1)
                {
                    currentSpace -= size;
                }
            }
            else if (direction == 'd')
            {
                if (currentSpace < size * (size - 1))
                {
                    currentSpace += size;
                }
            }
            loopNum = 0;
        }
        
        loadScreen(spaces, currentSpace, size);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        loopNum++;

        key = getch();
        clear();
    } while (key != 'q');
}