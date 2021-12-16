#ifndef COLOUR_H
#define COLOUR_H

#include "Vec3.hpp"

#include <iostream>

// Write the translated [0, 255] value of each colour component to the given
// output stream
void write_colour(std::ostream &out, Colour pixel_colour) {
  // (remember, Colour is a type alias for Vec3)
  out << static_cast<int>(255.999 * pixel_colour.x()) << ' '
      << static_cast<int>(255.999 * pixel_colour.y()) << ' '
      << static_cast<int>(255.999 * pixel_colour.z()) << '\n';
}

#endif
