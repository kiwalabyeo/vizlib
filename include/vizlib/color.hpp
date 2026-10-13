#pragma once  // makes sure this file is only included once

namespace vizlib {

// chose a struct over a class because everything in it is public by default. Both work the same.
struct Color {

    // unsigned char runs from 0 to 255, whereas signed char runs from -128 to 127.
    // A colour channel only needs 0 to 255.
    // also char keeps the structure very small, less memory used, compared to ints.
    unsigned char r = 0;     // red: from 0 to 255, how much red there is in the mix. 255 = full bright red, 0 = no red at all.
    unsigned char g = 0;     // green: from 0 to 255, how much green there is in the mix. 255 = full bright green, 0 = no green at all.
    unsigned char b = 0;     // blue: from 0 to 255, how much blue there is in the mix. 255 = full bright blue, 0 = no blue at all.
    unsigned char a = 255;   // alpha: from 0 to 255, how opaque the pixel is. 255 = fully opaque, 0 = fully transparent.

};

}  // end of namespace vizlib