#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

namespace ColorUtils
{
    unsigned char getShortRedPixel(unsigned short colorShort);
    unsigned char getShortGreenPixel(unsigned short colorShort);
    unsigned char getShortBluePixel(unsigned short colorShort);

    unsigned char makeShortPixelLow(unsigned char redChar, unsigned char blueChar, unsigned char greenChar);
    unsigned char makeShortPixelHigh(unsigned char redChar, unsigned char blueChar, unsigned char greenChar);
}


#endif