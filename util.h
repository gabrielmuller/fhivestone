#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define BOARD_SIZE 15
#define WIN_SIZE 5
#define SPACE_RAUDIS 2
#define DEPTH 4

static int VALUES[] = {1, 10, 100, 1000, 1000000};
static int BITMASKS[] = {0x3ffff, 0x3fffc, 0x3fff0, 0x3ffc0, 0x3ff00,
                        0x3fc00, 0x3f000, 0x3c000, 0x30000, 0x0};

typedef int32_t row_t;
enum vals {empty=0x00, player1=0x01, player2=0x02, offboard=0x03};

int min (int x, int y);
int max (int x, int y);

#endif
