#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "rtutils.h"

#include <iostream>

// Writes an input color in range [0,1] to an output stream with range [0,255]
void write_color(std::ostream &out, color c, int samples_per_pixel){
    auto r = c.x();
    auto g = c.y();
    auto b = c.z();

    double scale = 1.0 / samples_per_pixel;
    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);
    out <<  static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        <<  static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' ' 
        <<  static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif