#include "snake.h"

#include <windows.h>

int main()
{
    Snake snake;
    while (snake.IsGameRunning())
    {
        snake.Input();
        snake.Logic();
        snake.Print();
        Sleep(DelayMs);
    }
}