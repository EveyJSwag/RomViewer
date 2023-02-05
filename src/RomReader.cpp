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
//int main(int argc, char* argv[])
//{
//    bool foundFile = false;
//    std::string fileName = "";
//    const int ROM_OFFSET = 0x200;
//    while(*argv)
//    {
//        if (strcmp(*argv,"-f") == 0)
//        {
//            foundFile = true;
//        }
//        if (foundFile)
//        {
//            fileName = *argv;
//        }
//        argv++;
//    }
//    int fileDesc = open(fileName.c_str(), O_RDONLY);
//    char romDataBuffer[1049088];
//    memset(romDataBuffer, 0, 1049088);
//    read(fileDesc, romDataBuffer, 1049088);
//
//    int startIndex = 0x7FB0;
//    int realStartIndex = 0x81c0;
//    //for (int i = realStartIndex; i < 21 + realStartIndex; i++)
//    int romSize = 400 << (int)((unsigned char)romDataBuffer[0x7FD8+ROM_OFFSET]);
//    std::cout << romSize << std::endl;
//    std::cout << std::endl;
//    std::cout << std::hex << realStartIndex << std::endl;
//    std::cout << std::hex << realStartIndex - 0x7FC0 << std::endl;
//}
