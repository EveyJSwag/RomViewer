#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "RomReader.h"


void getRomData(std::string fileName, unsigned int romSize, void* romDataBuffer)
{
    int fileDesc = open(fileName.c_str(), O_RDWR);
    memset(romDataBuffer, 0, romSize); 
    read(fileDesc,romDataBuffer,romSize); 
}