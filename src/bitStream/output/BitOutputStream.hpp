#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

class BitOutputStream {
  private:
    ostream& out;          // reference to the output stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits have been writen to buf

  public:
    /**
     * TODO: Initializes member variables. In particular, the char array should
     * have size bufSize.
     */
    explicit BitOutputStream(ostream& os, unsigned int bufSize)
        : out(os), bufSize(bufSize), nbits(0), buf(new char[bufSize]) {
        fill(buf, buf + bufSize, '\0');
    }

    ~BitOutputStream();

    void flush();

    void writeBit(unsigned int i);
};

#endif
