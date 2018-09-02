#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#define BOARD_SIZE 15
#define WIN_SIZE 5

static int VALUES[] = {1, 10, 100, 1000, 1000000};
static int BITMASKS[] = {0x0, 0x3, 0xf, 0x3f, 0xff, 0x3ff, 0xfff, 0x3fff};

typedef int32_t row_t;
enum vals {empty=0x00, player1=0x01, player2=0x02, offboard=0x03};

#endif
