#include "BitOutputStream.hpp"
#include <iostream>

using namespace std;

BitOutputStream::~BitOutputStream() { delete[] buf; }
/**
 * TODO: Write the part of the buffer that was written by the user to the output
 * stream, and then clear the buffer to allow further use. You may use fill() to
 * zero-fill the buffer.
 *
 * Note: don’t flush the ostream here, as it is an extremely slow operation that
 * may cause a timeout.
 */
void BitOutputStream::flush() {
    unsigned int flushSize = nbits / 8;
    if (nbits % 8 != 0) {
        out.write(buf, flushSize + 1);
    } else {
        out.write(buf, flushSize);
    }
    // reset nbits to 0 to indicate buffer is flushed
    nbits = 0;
    // clear buffer
    // delete []buf;
    // buf = new char[bufSize];
    fill(buf, buf + bufSize, '\0');
}

/**
 * TODO: Write the least significant bit of the given int to the bit buffer.
 * Flushes the buffer first if it is full (which means all the bits in the
 * buffer have already been set). You may assume the given int is either 0 or 1.
 */
void BitOutputStream::writeBit(unsigned int i) {
    // check if buffer is full
    if (nbits == bufSize * 8) {
        flush();
    }
    unsigned int lsb = 1;
    lsb &= i;
    // how many bits to shift over
    int shift = nbits % 8;
    shift = 7 - shift;
    lsb = lsb << shift;
    // nbits / bufSize to get which char to access
    buf[nbits / 8] |= lsb;
    // increment nbits
    nbits++;
}
