#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <vector>
using namespace std;

char getch()
{
    char buf = 0;
    struct termios old = {0};

    // Get the current terminal settings
    if (tcgetattr(STDIN_FILENO, &old) < 0)
    {
        perror("tcgetattr()");
    }

    // Disable canonical mode and echo
    old.c_lflag &= ~ICANON; // Disable buffering
    old.c_lflag &= ~ECHO;   // Disable echo
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old) < 0)
    {
        perror("tcsetattr ICANON");
    }

    // Read one character from standard input
    if (read(STDIN_FILENO, &buf, 1) < 0)
    {
        perror("read()");
    }

    // Restore the original terminal settings
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &old) < 0)
    {
        perror("tcsetattr ~ICANON");
    }

    return buf;
}

void clearScreen()
{
    cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear the screen and move the cursor to the top-left corner
}

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
        cout << output;
    }
    cout << "\n";
    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            if ((x * size) + y == currentSpace)
            {
                cout << "|" << spaces[(x * size) + y] << "|";
            }
            else if ((x * size) + y - 1 == currentSpace && y != 0) 
            {
                cout << spaces[(x * size) + y];
            }
            
            else
            {
                cout << " " << spaces[(x * size) + y];
            }
        }

        if (currentSpace >= (x + 1) * size && currentSpace < (x + 2) * size && currentSpace >= size)
        {
            cout << "\n";
            string output;
            for (int i = 0; i < currentSpace % size; ++i)
            {
                output += "  ";
            }
            output += " _";
            cout << "" << output + "\n";
        }
        else if (currentSpace >= x * size && currentSpace < (x + 1) * size)
        {
            cout << "\n";
            string output;
            for (int i = 0; i < currentSpace % size; ++i)
            {
                output += "  ";
            }
            output += " -";
            cout << "" << output + "\n";
        }
        else
        {
            cout << "\n\n";
        }
    }
    clearScreen();
};

std::vector<int> generateSpaces(int size)
{
    std::vector<int> spaces(size * size);
    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            spaces[x * size + y] = 0;
        }
    }
    return spaces;
}

int main()
{
    int size = 4;
    
    int currentSpace = 0;
    std::vector<int> spaces;
    spaces = generateSpaces(size);

    loadScreen(spaces, currentSpace, size);
    char key = getch();
    do
    {
        if (key == 'w' || key == 'A')
        {
            if (currentSpace > size - 1)
            {
                currentSpace = currentSpace - size;
            }
        }
        else if (key == 'd' || key == 'C')
        {
            if (currentSpace % size != size - 1)
            {
                currentSpace = currentSpace + 1;
            }
        }
        else if (key == 's' || key == 'B')
        {
            if (currentSpace < size * (size - 1))
            {
                currentSpace = currentSpace + size;
            }
        }
        else if (key == 'a' || key == 'D')
        {
            if (currentSpace % size != 0)
            {
                currentSpace = currentSpace - 1;
            }
        }
        loadScreen(spaces, currentSpace, size);
        key = getch();
    } while (key != 'q');
}