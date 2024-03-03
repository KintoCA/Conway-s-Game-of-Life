
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>
#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_VER
#pragma warning(disable : 4996) // Ignore specific deprecation warnings
#endif
void usleep(__int64 usec) {
    Sleep((DWORD)(usec/1000)); // Windows version
}
#else
#include <unistd.h>
#endif

uint64_t bit = 0x8000000000000000;

bool is_alive(uint64_t board, uint8_t cId)
{
    return board & (bit >> cId);
}

int count_neighbors(uint64_t board, uint8_t cId)
{
    cId = (cId % 8) * 8 + cId / 8;
    int count = 0;
    count += is_alive(board, (cId + 64 + 1) % 64) != 0;
    count += is_alive(board, (cId + 64 - 1) % 64) != 0;
    count += is_alive(board, (cId + 64 + 8) % 64) != 0;
    count += is_alive(board, (cId + 64 - 8) % 64) != 0;
    count += is_alive(board, (cId + 64 + 9) % 64) != 0;
    count += is_alive(board, (cId + 64 - 9) % 64) != 0;
    count += is_alive(board, (cId + 64 + 7) % 64) != 0;
    count += is_alive(board, (cId + 64 - 7) % 64) != 0;
    return count;
}

void set_alive(uint64_t* board, uint8_t cid)
{
    *board |= (bit >> (cid % 64));
}

void set_dead(uint64_t* board, uint8_t cid)
{
    *board &= ~(bit >> (cid % 64));
}

void print_board(uint64_t board)
{
    for (int i = 1; i < 65; i++)
    {
        printf("%c ", board & (bit >> (i - 1)) ? 'X' : '.');
        if (!(i % 8))
            putchar(10);
    }
    putchar(10);
}

uint64_t update(uint64_t board)
{
    uint64_t new_board = 0;
    for (int i = 0; i < 64; i++)
    {
        int nc = ((i % 8) * 8 + i / 8) % 64;
        int live_count = count_neighbors(board, nc);
        if (is_alive(board, i))
        {
            if (live_count == 2 || live_count == 3)
            {
                set_alive(&new_board, i);
            }
        }
        else
        { // Cell is dead
            if (live_count == 3)
            {
                set_alive(&new_board, i);
            }
        }
    }
    return new_board;
}

void convertToUpper(char* str) {
    if (str == NULL) return; // Safety check

    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}
int main(int argc, char* argv[])
{
    const uint64_t bLinNkEr = 0x808080000;
    const uint64_t BEACON = 0x3020040c0000;
    const uint64_t TOAD = 0x1c380000;
    char mode[10] = "RANDOM"; // Default value, ensure the array is large enough
    time_t rawtime;
    time(&rawtime);
    uint64_t board = rawtime;

    if (argc > 1 && argv[1] != NULL) { // Check if there are command line arguments and they are not NULL
        char argUpper[100]; // Buffer for the uppercase argument, size should be sufficient for expected input

        strncpy(argUpper, argv[1], sizeof(argUpper)); // Safely copy the argument
        argUpper[sizeof(argUpper) - 1] = '\0'; // Ensure null-termination
        convertToUpper(argUpper); // Convert the copied argument to uppercase

        if (strcmp(argUpper, "BEACON") == 0) {
            strncpy(mode, "BEACON", sizeof(mode)); // Safe copy
            board = BEACON;
        }
        else if (strcmp(argUpper, "BLINKER") == 0) {
            strncpy(mode, "BLINKER", sizeof(mode)); // Safe copy
            board = bLinNkEr;
        }
        else if (strcmp(argUpper, "TOAD") == 0) {
            strncpy(mode, "TOAD", sizeof(mode)); // Safe copy
            board = TOAD;
        }
        // No need to check for "RANDOM" as it's already the default
        mode[sizeof(mode) - 1] = '\0'; // Ensure null-termination
    }

    printf("%s\n", mode);

    print_board(board);
    int counter = 1;
    while (counter++ ^ 4)
    {
        board = update(board);
        print_board(board);
        if (board == 0)
            break;
        usleep(200000);
    }
    return 0;
}
