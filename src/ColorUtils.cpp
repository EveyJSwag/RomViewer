#include "ColorUtils.h"

unsigned char ColorUtils::getShortGreenPixel(unsigned short colorShort)
{
    return (colorShort & 0b0000001111100000) >> 5;
}
unsigned char ColorUtils::getShortRedPixel(unsigned short colorShort)
{
    return colorShort & 0b0000000000011111;
}

unsigned char ColorUtils::getShortBluePixel(unsigned short colorShort)
{
    return (colorShort & 0b0111110000000000) >> 10;
}

unsigned char ColorUtils::makeShortPixelLow(unsigned char redChar, unsigned char blueChar, unsigned char greenChar)
{
    return blueChar |  ((greenChar & 0b111) << 5); 
}

unsigned char ColorUtils::makeShortPixelHigh(unsigned char redChar, unsigned char blueChar, unsigned char greenChar)
{
    return ((greenChar & 0b00011000) >> 3)| (redChar << 2);  
}


