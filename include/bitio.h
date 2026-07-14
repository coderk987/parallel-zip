#pragma once

#include <unistd.h>
#include <cstdint>

class BitReader{
    public:

    int idx;
    int fd;
    char c;
    int8_t byte;

    BitReader(int file);

    int readBit();
};

class BitWriter{
    public:
    //current bit idx
    //current char
    //current idx

    int idx;
    uint8_t byte;
    int fd;

    BitWriter(int file);

    void writeBit(int bit);
};