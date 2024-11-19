#include <color.h>
#include <stdlib.h>
#include <time.h>
#include <psapi.h>
#include <limits.h>

static void leak()
{
    int* ptr = (int*)malloc(sizeof(int) * 10000000);
    if (ptr == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int j = 0; j < 100; ++j)
    {
        ptr[j] = rand() % 100;
    }
    char css[100];
    snprintf(css, sizeof(css), "Allocated memory: 0x%p", (void*)ptr);
    char* colored = colorize(css, MAGENTA);
    printf("%s\n", colored);
    free(colored);
}

static void printc(const char* message, ColorCode color)
{
    char* colored = colorize(message, color);
    printf("%s\n", colored);
    free(colored);
}

static double cpu()
{
    FILETIME idleTime, kernelTime, userTime;
    static FILETIME lastIdleTime = {0}, lastKernelTime = {0}, lastUserTime = {0};

    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;
    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;
    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;

    ULARGE_INTEGER lastIdle, lastKernel, lastUser;
    lastIdle.LowPart = lastIdleTime.dwLowDateTime;
    lastIdle.HighPart = lastIdleTime.dwHighDateTime;
    lastKernel.LowPart = lastKernelTime.dwLowDateTime;
    lastKernel.HighPart = lastKernelTime.dwHighDateTime;
    lastUser.LowPart = lastUserTime.dwLowDateTime;
    lastUser.HighPart = lastUserTime.dwHighDateTime;

    ULONGLONG idleDiff = idle.QuadPart - lastIdle.QuadPart;
    ULONGLONG kernelDiff = kernel.QuadPart - lastKernel.QuadPart;
    ULONGLONG userDiff = user.QuadPart - lastUser .QuadPart;

    lastIdleTime = idleTime;
    lastKernelTime = kernelTime;
    lastUserTime = userTime;

    ULONGLONG total = (kernelDiff + userDiff);
    if (total > 0)
    {
        return (1.0 - (double)idleDiff / total) * 100.0;
    }
    return 0.0;
}

int main()
{
    srand((unsigned int)time(NULL));
    char max_leaks_str[11];
    printf("Enter the maximum number of memory leaks (default: 1000): ");
    fgets(max_leaks_str, sizeof(max_leaks_str), stdin);

    int max_leaks;
    if (sscanf_s(max_leaks_str, "%d", &max_leaks) != 1)
    {
        max_leaks = 1000;
    }

    if (max_leaks <= 0 || max_leaks > INT_MAX)
    {
        fprintf(stderr, "You can't do that :3\n");
        exit(1);
    }

    double stability = 100.0;
    double usage = 0.0;

    for (int i = 0; i < max_leaks; ++i)
    {
        leak();
        char cs[100];
        snprintf(cs, sizeof(cs), "Memory leaks: %d/%d", i + 1, max_leaks);
        printc(cs, BLUE);

        usage = cpu();

        stability = 100.0 * (1 - (usage / 100.0));
        if (stability < 0)
        {
            stability = 0;
        }

        char css[100];
        snprintf(css, sizeof(css), "System stability: %.2f%%", stability);
        if ((i + 1) % 100 == 0)
        {
            printc(css, RED_BG);
        }
    }

    exit(1);

    return 0;
}