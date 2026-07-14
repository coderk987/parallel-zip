#include "bitio.h"

BitReader::BitReader(int file){
    fd = file;
    idx=-1;
}

int BitReader::readBit(){
    if(idx==-1){
        if(read(fd, &c, 1) <= 0) return -1;
        byte = c;
        idx = 7;
    }
    return byte>>(idx--) & 1;
}

BitWriter::BitWriter(int file){
    fd = file;
    byte = 0;
    idx = 7;
}

void BitWriter::writeBit(int bit){
    if(bit) byte |= (1<<idx);
    idx--;
    if(idx==-1){
        char c = byte;
        write(fd, &c, 1);
        byte = 0;
        idx = 7;
    }
}