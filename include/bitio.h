#pragma once

#include <unistd.h>
#include <cstdint>

class BitReader{
    public:

    int idx = 0;
    int fd;
    char c;
    int8_t byte;

    BitReader(int file);

    int readBit();
};