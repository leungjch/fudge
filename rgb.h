#ifndef RGB_H
#define RGB_H
#include <cstdint>



class Rgb
{
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    static Rgb random();
    Rgb(uint8_t r, uint8_t g, uint8_t b);
};
#endif