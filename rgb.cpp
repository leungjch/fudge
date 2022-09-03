#include "rgb.h"
#include <random>

Rgb::Rgb(uint8_t r, uint8_t b, uint8_t g) : r{r}, g{g}, b{b} {}

Rgb Rgb::random() {
   int lower_bound = 0;
   int upper_bound = 255;
   std::uniform_int_distribution unif(lower_bound,upper_bound);
   std::default_random_engine re;
   uint8_t rand_r = unif(re);
   uint8_t rand_g = unif(re);
   uint8_t rand_b = unif(re);
    return Rgb(rand_r, rand_g, rand_b);
}
