#ifndef RGB_H
#define RGB_H
#include <cstdint>
#include "../glm/glm/ext.hpp"


class Rgb
{
public:
    glm::vec3 color;
    static Rgb random();
    Rgb(float r, float g, float b);
};
#endif