#include <iostream>

#include "Colour.hpp"
#include "Vec3.hpp"

int main() {

  // Image

  const int imgWidth = 256;
  const int imgHeight = 256;

  // Render

  const double blue = 0.25;

  std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

  for (int j = imgHeight - 1; j >= 0; --j) {
    // progress indicator
    std::cerr << '\r' << "Scanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < imgWidth; ++i) {
      Colour pixel_colour( double(i) / (imgWidth - 1)
                         , double(j) / (imgHeight - 1)
                         , blue);
      write_colour(std::cout, pixel_colour);
    }
  }

  // end of progress indicator
  std::cerr << '\n' << "Done.\n";
}

