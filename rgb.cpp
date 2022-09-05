#include "rgb.h"
#include <random>
#include <time.h>       /* time */

Rgb::Rgb(uint8_t r, uint8_t b, uint8_t g) : r{r}, g{g}, b{b} {}

Rgb Rgb::random() {
    srand(time(NULL));
   int lower_bound = 0;
   int upper_bound = 255;
   std::uniform_int_distribution unif(lower_bound,upper_bound);
    std::random_device rd;  
    std::mt19937 gen(rd()); 
   uint8_t rand_r = unif(rd);
   uint8_t rand_g = unif(rd);
   uint8_t rand_b = unif(rd);
    return Rgb((rand_r+255)/2, (rand_g+255)/2, (rand_b+255)/2);
}
