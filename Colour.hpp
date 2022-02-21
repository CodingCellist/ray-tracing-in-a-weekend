#ifndef COLOUR_H
#define COLOUR_H

#include "Vec3.hpp"

#include <iostream>

// Write the translated [0, 255] value of each colour component to the given
// output stream
void write_colour(std::ostream &out, Colour pixel_colour, int samples_per_pixel) {
  auto r = pixel_colour.x();
  auto g = pixel_colour.y();
  auto b = pixel_colour.z();

  // Divide the colour by the #samples and gamma-correct for gamma=2.0
  auto scale = 1.0 / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

  // Write the translated [0, 255] value of each colour component
  // (remember, Colour is a type alias for Vec3)
  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif
