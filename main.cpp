#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
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

void loadScreen(int spaces[], int currentSpace)
{
    if (currentSpace < 9)
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
    for (int x = 0; x < 9; ++x)
    {

        cout << " ";
    
        for (int y = 0; y < 9; ++y)
        {
            if ((x * 9) + y == currentSpace)
            {
                cout << "|" << spaces[(x * 9) + y] << "|";
            }
            else if ((x * 9) + y - 1 == currentSpace && y != 0) 
            {
                cout << spaces[(x * 9) + y];
            }
            
            else
            {
                cout << " " << spaces[(x * 9) + y];
            }
        }

        if (currentSpace >= (x + 1) * 9 && currentSpace < (x + 2) * 9 && currentSpace >= 9)
        {
            cout << "\n";
            string output;
            for (int i = 0; i < currentSpace % 9; ++i)
            {
                output += "  ";
            }
            output += " _";
            cout << "  " << output + "\n";
        }
        else if (currentSpace >= x * 9 && currentSpace < (x + 1) * 9)
        {
            cout << "\n";
            string output;
            for (int i = 0; i < currentSpace % 9; ++i)
            {
                output += "  ";
            }
            output += " -" + std::to_string(currentSpace);
            cout << " " << output + "\n";
        }
        else
        {
            cout << "\n\n";
        }
    }
    clearScreen();
};

int main()
{
    int spaces[81];
    for (int x = 0; x < 9; ++x)
    {
        for (int y = 0; y < 9; ++y)
        {
            spaces[x * 9 + y] = 0;
        }
    }
    spaces[0] = 0;
    int currentSpace = 0;

    loadScreen(spaces, currentSpace);
    char key = getch();
    do
    {
        if (key == 'w' || key == 'A')
        {
            if (currentSpace > 8)
            {
                currentSpace = currentSpace - 9;
            }
        }
        else if (key == 'd' || key == 'C')
        {
            if (currentSpace % 9 != 8)
            {
                currentSpace = currentSpace + 1;
            }
        }
        else if (key == 's' || key == 'B')
        {
            if (currentSpace < 72)
            {
                currentSpace = currentSpace + 9;
            }
        }
        else if (key == 'a' || key == 'D')
        {
            if (currentSpace % 9 != 0)
            {
                currentSpace = currentSpace - 1;
            }
        }
        loadScreen(spaces, currentSpace);
        key = getch();
    } while (key != 'q');
}