#include "rgb.h"
#include <random>
#include <time.h> /* time */
#include "../glm/glm/ext.hpp"

Rgb::Rgb(float r, float b, float g) : color{glm::vec3(r,g,b)} {}

Rgb Rgb::random()
{
    srand(time(NULL));
    float lower_bound = 0.0f;
    float upper_bound = 1.0f;
    std::random_device rd;  
    std::minstd_rand gen(rd()); 
    std::uniform_real_distribution<float> unif(lower_bound, upper_bound);
    float rand_x = unif(rd);
    float rand_y = unif(rd);
    float rand_z = unif(rd);
    return Rgb((rand_x+1.0f)/2.0, (rand_y+1.0f)/2.0, (rand_z+1.0)/2.0);
}
