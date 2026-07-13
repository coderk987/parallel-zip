#include "bitio.h"

class BitReader{
    public:
    //current bit idx
    // current char => as int
    // current fd
    //if idx == 0 read new char else right shift and return 0 or 1

    int idx = -1;
    int fd;
    char c;
    uint8_t byte;

    BitReader(int file){
        fd = file;
    }

    int readBit(){
        if(idx==-1){
            if(read(fd, &c, 1) <= 0) return -1;
            byte = c;
            idx = 7;
        }
        return byte>>(idx--) & 1;
    }
}

class BitWriter{
    public:
    //current bit idx
    //current char
    //current idx

    int idx = 7;
    uint8_t byte = 0;
    int fd;

    BitWriter(int file){
        fd = file;
    }

    void writeBit(int bit){
        if(idx==-1){
            char c = byte;
            write(fd, &c, 1);
            byte = 0;
            idx = 7;
        }
        if(bit) byte |= (1<<idx);
        idx--;
    }
}