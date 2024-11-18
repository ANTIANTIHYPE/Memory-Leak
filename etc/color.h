// rewritten for C

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>

typedef enum
{
    RED_BG = 41,
    BLUE = 94,
    MAGENTA = 95
} ColorCode;

char* colorize(const char* text, ColorCode code)
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);
#endif
    size_t required_size = strlen(text) + 20;
    char* coloredText = (char*)malloc(required_size * sizeof(char));
    sprintf_s(coloredText, required_size, "\x1B[%dm%s\x1B[0m", code, text);

    return coloredText;
}