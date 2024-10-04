#include "etc/color.h"
#include <stdlib.h>
#include <time.h>
#include <psapi.h>

static void leak() {
    int* ptr = (int*)malloc(sizeof(int) * 10000000);
    for (int j = 0; j < 100; ++j) ptr[j] = rand() % 100;
    char css[100];
    snprintf(css, sizeof(css), "Allocated memory: %p", (void*)ptr);
    char* colored = colorize(css, MAGENTA);
    printf("%s\n", colored);
    free(colored);
}

static void death() {
    exit(1);
}

int main() {
    int i;
    srand((unsigned int)time(NULL));
    printf("Welcome.\n");
    char max_leaks_str[11];
    printf("Enter the maximum number of memory leaks (default: 1000): ");
    fgets(max_leaks_str, sizeof(max_leaks_str), stdin);

    int max_leaks;
    if (sscanf_s(max_leaks_str, "%d", &max_leaks) != 1) {
        //fprintf(stderr, "Invalid input\n");
        //exit(1);
        max_leaks = 1000;
    }

    if (max_leaks <= 0 || max_leaks > INT_MAX/* || max_leaks == 1410065407 || max_leaks == 1000100010*/) {
        fprintf(stderr, "You can't do that :3\n");
        exit(1);
    }

    double stability = 100.0;
    double usage = 0.0;

    for (i=0; i < max_leaks; ++i) {
        leak();
        char cs[100];
        snprintf(cs, sizeof(cs), "Memory leaks: %d/%d", i + 1, max_leaks);
        char* colored1 = colorize(cs, BLUE);
        printf("%s\n", colored1);
        free(colored1);

        PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        usage = (double)pmc.WorkingSetSize / (double)pmc.PagefileUsage * 100.0;

        stability = 100.0 * (1 - (usage / 100.0));
        if (stability < 0) stability = 0;

        char css[100];
        snprintf(css, sizeof(css), "System stability: %.2f%%", stability);
        char* colored = colorize(css, RED_BG);

        if ((i+1) % 100 == 0) printf("%s\n", colored);
        free(colored);
    }

    if (i == max_leaks && max_leaks > 0) printf("Congrats! You won.\n"); else death();

    return 0;
}