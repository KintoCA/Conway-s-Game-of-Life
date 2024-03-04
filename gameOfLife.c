/*
    Runs on Linux (gcc-13) and Windows (cl.exe 19.38)
    https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
*/


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
#pragma warning(disable : 4996)
#endif
void usleep(__int64 usec) {
    Sleep((DWORD)(usec/1000));
}
#else
#include <unistd.h>
#endif

uint64_t bit = 0x8000000000000000;

extern inline bool is_alive(uint64_t board, uint8_t cId);
extern inline int count_neighbors(uint64_t board, uint8_t cId);
extern inline void set_alive(uint64_t* board, uint8_t cid);
extern inline void set_dead(uint64_t* board, uint8_t cid);
extern inline void print_board(uint64_t board);
extern inline uint64_t update(uint64_t board);

inline bool is_alive(uint64_t board, uint8_t cId){
    return board & (bit >> cId);
}

inline int count_neighbors(uint64_t board, uint8_t cId){
    cId = (cId % 8) * 8 + cId / 8;
    int count = 0;
    int off[8] = { 1, -1, 8, -8, 9, -9, 7, -7 };
    for(int i = 0; i < 8; i++)
    {
        count += is_alive(board, (cId + 64 + off[i]) % 64) != 0;    
    }
    return count;
}

inline void set_alive(uint64_t* board, uint8_t cid){
    *board |= (bit >> (cid % 64));
}

inline void set_dead(uint64_t* board, uint8_t cid){
    *board &= ~(bit >> (cid % 64));
}

inline void print_board(uint64_t board){
    for (int i = 1; i < 65; i++){
        printf("%c ", board & (bit >> (i - 1)) ? 'X' : '.');
        if (!(i % 8))
            putchar(10);
    }
    putchar(10);
}

inline uint64_t update(uint64_t board){
    uint64_t new_board = 0;
    for (int i = 0; i < 64; i++){
        int nc = ((i % 8) * 8 + i / 8) % 64;
        int live_count = count_neighbors(board, nc);
        if (is_alive(board, i)){
            if (live_count == 2 || live_count == 3){
                set_alive(&new_board, i);
            }
        }
        else{
            if (live_count == 3){
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
int main(int argc, char* argv[]){

    const uint64_t bLinNkEr = 0x808080000;
    const uint64_t BEACON = 0x3020040c0000;
    const uint64_t TOAD = 0x1c380000;
    char mode[10] = "RANDOM";
    time_t rawtime;
    time(&rawtime);
    uint64_t board = rawtime;

    if (argc > 1 && argv[1] != NULL) {
        char argUpper[100];

        strncpy(argUpper, argv[1], sizeof(argUpper));
        argUpper[sizeof(argUpper) - 1] = '\0'; 
        convertToUpper(argUpper);

        if (strcmp(argUpper, "BEACON") == 0) {
            strncpy(mode, "BEACON", sizeof(mode));
            board = BEACON;
        }
        else if (strcmp(argUpper, "BLINKER") == 0) {
            strncpy(mode, "BLINKER", sizeof(mode));
            board = bLinNkEr;
        }
        else if (strcmp(argUpper, "TOAD") == 0) {
            strncpy(mode, "TOAD", sizeof(mode));
            board = TOAD;
        }
        mode[sizeof(mode) - 1] = '\0';
    }

    printf("%s\n", mode);

    print_board(board);
    int counter = 1;
    while (counter++ ^ 40000000)
    {
        board = update(board);
        print_board(board);
        if (board == 0)
            break;
        usleep(200000);
    }
    return 0;
}

